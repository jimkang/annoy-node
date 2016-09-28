#ifndef ANNOYINDEXWRAPPER_H
#define ANNOYINDEXWRAPPER_H

#include <nan.h>
#include "annoylib.h"

class AnnoyIndexWrapper : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);

 private:
  explicit AnnoyIndexWrapper(double value = 0);
  ~AnnoyIndexWrapper();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void GetValue(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void PlusOne(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Multiply(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static Nan::Persistent<v8::Function> constructor;
  double value_;
};

#endif
