#include <napi.h>
#include "nodes.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return NODES::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)