#ifndef ANNOYINDEXWRAPPER_H
#define ANNOYINDEXWRAPPER_H

#include <nan.h>
#include "annoylib.h"

class AnnoyIndexWrapper : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  int getDimensions();
  AnnoyIndexInterface<int, float> *annoyIndex;

 private:
  explicit AnnoyIndexWrapper(int dimensions, const char *metricString);
  ~AnnoyIndexWrapper();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  // static void GetValue(const Nan::FunctionCallbackInfo<v8::Value>& info);
  // static void PlusOne(const Nan::FunctionCallbackInfo<v8::Value>& info);
  // static void Multiply(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void AddItem(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Build(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Save(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static Nan::Persistent<v8::Function> constructor;
  // double value_;
  int annoyDimensions;
};

#endif
