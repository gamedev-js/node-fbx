#include "./common.h"

NAN_METHOD(newAnim) {
  // handle arguments
  v8::Local<v8::String> arg0 = info[0].As<v8::String>();
  int len = arg0->Utf8Length();
  char *path = (char *)malloc((len+1) * sizeof(char));
  arg0->WriteUtf8(path);

  // init SDK
  FbxManager *fbxMgr = NULL;
  FbxScene *fbxScene = NULL;
  init_fbx_sdk(fbxMgr, fbxScene);

  // TODO
  save_scene(fbxMgr, fbxScene, path);

  // deinit
  free(path);

  deinit_fbx_sdk(fbxMgr);
}

NAN_MODULE_INIT(init) {
  NAN_EXPORT(target, newAnim);
}

NODE_MODULE(utils, init)
