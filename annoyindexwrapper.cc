#include "annoyindexwrapper.h"
#include <vector>

using namespace v8;
using namespace Nan;

Nan::Persistent<v8::Function> AnnoyIndexWrapper::constructor;

AnnoyIndexWrapper::AnnoyIndexWrapper(int dimensions, const char *metricString) :
  annoyDimensions(dimensions) {

  if (strcmp(metricString, "Angular") == 0) {
    annoyIndex = new AnnoyIndex<int, float, Angular, RandRandom>(dimensions);
  }
  else {
    annoyIndex = new AnnoyIndex<int, float, Euclidean, RandRandom>(dimensions);    
  }
}

AnnoyIndexWrapper::~AnnoyIndexWrapper() {
  delete annoyIndex;
}

void AnnoyIndexWrapper::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Annoy").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(2);

  // Prototype
  // Nan::SetPrototypeMethod(tpl, "value", GetValue);
  // Nan::SetPrototypeMethod(tpl, "plusOne", PlusOne);
  // Nan::SetPrototypeMethod(tpl, "multiply", Multiply);
  Nan::SetPrototypeMethod(tpl, "addItem", AddItem);
  Nan::SetPrototypeMethod(tpl, "build", Build);
  Nan::SetPrototypeMethod(tpl, "save", Save);
  Nan::SetPrototypeMethod(tpl, "load", Load);
  Nan::SetPrototypeMethod(tpl, "getItem", GetItem);
  Nan::SetPrototypeMethod(tpl, "getNNsByVector", GetNNSByVector);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("Annoy").ToLocalChecked(), tpl->GetFunction());
}

void AnnoyIndexWrapper::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {

  if (info.IsConstructCall()) {
    // Invoked as constructor: `new AnnoyIndexWrapper(...)`
    double dimensions = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
    Local<String> metricString; 

    if (!info[1]->IsUndefined()) {
      Nan::MaybeLocal<String> s = Nan::To<String>(info[1]);
      if (!s.IsEmpty()) {
        metricString = s.ToLocalChecked();
      }
    }

    AnnoyIndexWrapper* obj = new AnnoyIndexWrapper(
      (int)dimensions, *String::Utf8Value(metricString)
    );
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }
}

void AnnoyIndexWrapper::AddItem(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  // Get out object.
  AnnoyIndexWrapper* obj = ObjectWrap::Unwrap<AnnoyIndexWrapper>(info.Holder());
  // Get out index.
  int index = info[0]->IsUndefined() ? 1 : info[0]->NumberValue();
  // Get out array.
  float vec[obj->getDimensions()];
  if (getFloatArrayParam(info, 1, vec)) {
    obj->annoyIndex->add_item(index, vec);
  }
}

void AnnoyIndexWrapper::Build(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  // Get out object.
  AnnoyIndexWrapper* obj = ObjectWrap::Unwrap<AnnoyIndexWrapper>(info.Holder());
  // Get out numberOfTrees.
  int numberOfTrees = info[0]->IsUndefined() ? 1 : info[0]->NumberValue();
  // printf("%s\n", "Calling build");
  obj->annoyIndex->build(numberOfTrees);
}

void AnnoyIndexWrapper::Save(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  // Get out object.
  AnnoyIndexWrapper* obj = ObjectWrap::Unwrap<AnnoyIndexWrapper>(info.Holder());
  // Get out file path.
  char *filePath = getStringParam(info, 0);
  if (filePath) {
    // printf("Calling save with %s\n", filePath);
    obj->annoyIndex->save(filePath);
  }
}

void AnnoyIndexWrapper::Load(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  bool result = false;
  // Get out object.
  AnnoyIndexWrapper* obj = ObjectWrap::Unwrap<AnnoyIndexWrapper>(info.Holder());
  // Get out file path.
  char *filePath = getStringParam(info, 0);
  if (filePath) {
    // printf("Calling load with %s\n", filePath);
    result = obj->annoyIndex->load(filePath);
  }
  info.GetReturnValue().Set(Nan::New(result));
}

void AnnoyIndexWrapper::GetItem(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Nan::HandleScope scope;

  // Get out object.
  AnnoyIndexWrapper* obj = ObjectWrap::Unwrap<AnnoyIndexWrapper>(info.Holder());

  // Get out index.
  int index = info[0]->IsUndefined() ? 1 : info[0]->NumberValue();

  // Get the vector.
  int length = obj->getDimensions();
  float vec[length];
  obj->annoyIndex->get_item(index, vec);

  // Allocate the return array.
  Local<Array> results = Nan::New<Array>(length);
  for (int i = 0; i < length; ++i) {
    // printf("Adding to array: %f\n", vec[i]);
    Nan::Set(results, i, Nan::New<Number>(vec[i]));
  }

  info.GetReturnValue().Set(results);
}

void AnnoyIndexWrapper::GetNNSByVector(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Nan::HandleScope scope;

  // Get out object.
  AnnoyIndexWrapper* obj = ObjectWrap::Unwrap<AnnoyIndexWrapper>(info.Holder());
  // Get out array.
  float vec[obj->getDimensions()];
  if (!getFloatArrayParam(info, 0, vec)) {
    return;
  }

  // Get out number of neighbors.
  int numberOfNeighbors = info[1]->IsUndefined() ? 1 : info[1]->NumberValue();

  // Get out searchK.
  int searchK = info[2]->IsUndefined() ? -1 : info[2]->NumberValue();

  // Get out include distances flag.
  bool includeDistances = info[3]->IsUndefined() ? false : info[3]->BooleanValue();

  std::vector<int> nnIndexes;
  std::vector<float> distances;
  std::vector<float> *distancesPtr = nullptr;
  
  if (includeDistances) {
    distancesPtr = &distances;
  }

  // Make the call.
  obj->annoyIndex->get_nns_by_vector(
    vec, numberOfNeighbors, searchK, &nnIndexes, distancesPtr
  );

  Local<Array> *resultArrayPtr = nullptr;

  // Allocate the neighbors array.
  Local<Array> jsNNIndexes = Nan::New<Array>(numberOfNeighbors);
  for (int i = 0; i < numberOfNeighbors; ++i) {
    // printf("Adding to neighbors array: %d\n", nnIndexes[i]);
    Nan::Set(jsNNIndexes, i, Nan::New<Number>(nnIndexes[i]));
  }

  Local<Array> jsResultArray;
  Local<Array> jsDistancesArray;

  if (includeDistances) {
    // Allocate the distances array.
    jsDistancesArray = Nan::New<Array>(numberOfNeighbors);
    for (int i = 0; i < numberOfNeighbors; ++i) {
      // printf("Adding to distances array: %f\n", distances[i]);
      Nan::Set(jsDistancesArray, i, Nan::New<Number>(distances[i]));
    }

    jsResultArray = Nan::New<Array>(2);
    Nan::Set(jsResultArray, 0, jsNNIndexes);
    Nan::Set(jsResultArray, 1, jsDistancesArray);
    resultArrayPtr = &jsResultArray;
  }
  else {
    resultArrayPtr = &jsNNIndexes;
  }

  info.GetReturnValue().Set(*resultArrayPtr);
}

char *AnnoyIndexWrapper::getStringParam(
  const Nan::FunctionCallbackInfo<v8::Value>& info, int paramIndex) {

  char *stringParam = NULL;
  if (!info[0]->IsUndefined()) {
    Nan::MaybeLocal<String> maybeString = Nan::To<String>(info[0]);
    if (!maybeString.IsEmpty()) {
      Local<String> s = maybeString.ToLocalChecked();
      stringParam = *String::Utf8Value(s);
    }
  }
  return stringParam;
}

// Returns true if it was able to get items out of the array. false, if not.
bool AnnoyIndexWrapper::getFloatArrayParam(
  const Nan::FunctionCallbackInfo<v8::Value>& info, int paramIndex, float *vec) {

  bool succeeded = false;

  Local<Value> val;
  if (info[paramIndex]->IsArray()) {
    Handle<Array> jsArray = Handle<Array>::Cast(info[paramIndex]);
    Handle<Value> val;
    for (unsigned int i = 0; i < jsArray->Length(); i++) {
      val = jsArray->Get(i);
      // printf("Adding item to array: %f\n", (float)val->NumberValue());
      vec[i] = (float)val->NumberValue();
    }
    succeeded = true;
  }

  return succeeded;
}

int AnnoyIndexWrapper::getDimensions() {
  return annoyDimensions;
}

