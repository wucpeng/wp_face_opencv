#ifndef NODES_H
#define NODES_H

#include <napi.h>

class NODES : public Napi::ObjectWrap<NODES> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  NODES(const Napi::CallbackInfo& info);
   ~NODES();
  Napi::Value cvFaceDetectUrl(const Napi::CallbackInfo& info);
  Napi::Value cnnFaceDetectUrl(const Napi::CallbackInfo& info);
  // Napi::Value faceDetectUrlAndDraw(const Napi::CallbackInfo& info);

 private:
  
};

#endif