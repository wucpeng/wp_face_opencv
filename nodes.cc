#include "nodes.h"
#include "face.h"
using namespace std;


Napi::Value errorData(Napi::Env env, string msg) {
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(Napi::String::New(env, "errorMsg"), msg);
    return obj;
}

Napi::Object NODES::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "NODES", {
        InstanceMethod("cvFaceDetectUrl", &NODES::cvFaceDetectUrl),
        InstanceMethod("cnnFaceDetectUrl", &NODES::cnnFaceDetectUrl)
        }
    );
  exports.Set("NODES", func);
  return exports;
}

NODES::NODES(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NODES>(info) {
    Napi::Env env = info.Env();
    int length = info.Length();
    // if (length != 3) {
    //     Napi::TypeError::New(env, "input expected").ThrowAsJavaScriptException();
    //     return;
    // }
}

NODES::~NODES(){
    printf("NODES end\n");
}


Napi::Value NODES::cvFaceDetectUrl(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int length = info.Length();
    if (length != 3 ) { //|| !info[0].IsString() 
        Napi::TypeError::New(env, "input expected").ThrowAsJavaScriptException();
        return errorData(env, "input expected"); //Napi::Number::New(info.Env(), -1);
    }
    Napi::Object obj = Napi::Object::New(env);
    string url = info[0].As<Napi::String>().ToString();
    string key = info[1].As<Napi::String>().ToString();
    string output = info[2].As<Napi::String>().ToString();

    std::vector<cv::Rect> faces = cvFindFacesAndDrawSave(url, key, output);
    obj.Set(Napi::String::New(env, "faceNum"), faces.size());
    Napi::Array list = Napi::Array::New(env, faces.size());
    for(int i = 0; i < faces.size(); ++i) {
        cv::Rect face = faces[i];
        Napi::Object data = Napi::Object::New(env);
        data.Set(Napi::String::New(env, "left"), face.x); // x左
        data.Set(Napi::String::New(env, "top"), face.y); // y上
        data.Set(Napi::String::New(env, "width"), face.width); //x右
        data.Set(Napi::String::New(env, "height"), face.height); //y下
        list[i] = data;
    }
    obj.Set(Napi::String::New(env, "faceRects"), list);
    return obj;
}

Napi::Value NODES::cnnFaceDetectUrl(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int length = info.Length();
    if (length != 3) { //|| !info[0].IsString()
        Napi::TypeError::New(env, "input expected").ThrowAsJavaScriptException();
        return errorData(env, "input expected"); //Napi::Number::New(info.Env(), -1);
    }
    Napi::Object obj = Napi::Object::New(env);
    string url = info[0].As<Napi::String>().ToString();
    string key = info[1].As<Napi::String>().ToString();
    string output = info[2].As<Napi::String>().ToString();


    std::vector<cv::Rect> faces = cnnFindFacesAndDrawSave(url, key, output);
    obj.Set(Napi::String::New(env, "faceNum"), faces.size());
    Napi::Array list = Napi::Array::New(env, faces.size());
    for(int i = 0; i < faces.size(); ++i) {
        cv::Rect face = faces[i];
        Napi::Object data = Napi::Object::New(env);
        data.Set(Napi::String::New(env, "left"), face.x); // x左
        data.Set(Napi::String::New(env, "top"), face.y); // y上
        data.Set(Napi::String::New(env, "width"), face.width); //宽
        data.Set(Napi::String::New(env, "height"), face.height); //高
        list[i] = data;
    }
    obj.Set(Napi::String::New(env, "faceRects"), list);
    return obj;
}