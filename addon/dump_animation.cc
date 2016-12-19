#include "./common.h"

v8::Local<v8::Value> _dump_animation (FbxAnimStack* _fbxAnimStack, FbxNode *_fbxNode) {
  // var animation = {};
  v8::Local<v8::Object> animation = Nan::New<v8::Object>();

  // TODO
  printf("%s", _fbxAnimStack->GetName());

  return animation;
}

v8::Local<v8::Array> dump_animations (FbxScene *_fbxScene) {
  int count = _fbxScene->GetSrcObjectCount<FbxAnimStack>();
  v8::Local<v8::Array> animations = Nan::New<v8::Array>(count);

  for (int i = 0; i < count; ++i) {
    FbxAnimStack* pAnimStack = _fbxScene->GetSrcObject<FbxAnimStack>(i);

    Nan::Set(animations, i, _dump_animation(pAnimStack, _fbxScene->GetRootNode()));
  }

  return animations;
}
