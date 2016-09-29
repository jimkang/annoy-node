#include "annoyindexwrapper.h"
#include <vector>

using namespace v8;

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

    AnnoyIndexWrapper* obj = new AnnoyIndexWrapper((int)dimensions, *String::Utf8Value(metricString));
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }
}

// void AnnoyIndexWrapper::GetValue(const Nan::FunctionCallbackInfo<v8::Value>& info) {
//   AnnoyIndexWrapper* obj = ObjectWrap::Unwrap<AnnoyIndexWrapper>(info.Holder());
//   info.GetReturnValue().Set(Nan::New(obj->value_));
// }

// void AnnoyIndexWrapper::AddItem(const Nan::FunctionCallbackInfo<v8::Value>& info) {
//   AnnoyIndexWrapper* obj = ObjectWrap::Unwrap<AnnoyIndexWrapper>(info.Holder());
//   obj->value_ += 1;
//   info.GetReturnValue().Set(Nan::New(obj->value_));
// }

// void AnnoyIndexWrapper::Multiply(const Nan::FunctionCallbackInfo<v8::Value>& info) {
//   AnnoyIndexWrapper* obj = ObjectWrap::Unwrap<AnnoyIndexWrapper>(info.Holder());
//   double multiple = info[0]->IsUndefined() ? 1 : info[0]->NumberValue();

//   v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);

//   const int argc = 1;
//   v8::Local<v8::Value> argv[argc] = { Nan::New(obj->value_ * multiple) };

//   info.GetReturnValue().Set(cons->NewInstance(argc, argv));
// }

void AnnoyIndexWrapper::AddItem(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  // Get out object.
  AnnoyIndexWrapper* obj = ObjectWrap::Unwrap<AnnoyIndexWrapper>(info.Holder());
  // Get out index.
  int index = info[0]->IsUndefined() ? 1 : info[0]->NumberValue();
  // Get out array.
  std::vector<float> vec(obj->getDimensions());
  Handle<Value> val;
  if (info[1]->IsArray()) {
    Handle<Array> jsArray = Handle<Array>::Cast(info[1]);
    for (unsigned int i = 0; i < jsArray->Length(); i++) {
      val = jsArray->Get(i);
      vec.push_back((float)val->NumberValue());
    }
    printf("%s\n", "Calling add_item");
    obj->annoyIndex->add_item(index, vec.data());
  }
}

void AnnoyIndexWrapper::Build(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  // Get out object.
  AnnoyIndexWrapper* obj = ObjectWrap::Unwrap<AnnoyIndexWrapper>(info.Holder());
  // Get out numberOfTrees.
  int numberOfTrees = info[0]->IsUndefined() ? 1 : info[0]->NumberValue();
  printf("%s\n", "Calling build");
  obj->annoyIndex->build(numberOfTrees);
}

int AnnoyIndexWrapper::getDimensions() {
  return annoyDimensions;
}

