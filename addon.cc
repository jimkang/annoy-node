#include <nan.h>
#include "annoyindexwrapper.h"

void InitAll(v8::Local<v8::Object> exports) {
  AnnoyIndexWrapper::Init(exports);
}

NODE_MODULE_INIT() {
    InitAll(exports);
}
