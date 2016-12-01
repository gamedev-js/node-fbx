#include "./common.h"

std::vector<v8::Local<v8::Value>> _dumpedMeshes;
std::vector<FbxMesh *> _cachedFbxMeshes;

NAN_METHOD(load) {
  // handle arguments
  v8::Local<v8::String> arg0 = info[0].As<v8::String>();
  int len = arg0->Utf8Length();
  char *path = (char *)malloc((len+1) * sizeof(char));
  arg0->WriteUtf8(path);

  // init SDK
  FbxManager *fbxMgr = NULL;
  FbxScene *fbxScene = NULL;
  init_fbx_sdk(fbxMgr, fbxScene);

  // load and process scene
  load_scene(fbxMgr, fbxScene, path);
  v8::Local<v8::Object> ret = dump_scene(fbxScene);

  // deinit
  free(path);
  _dumpedMeshes.clear();
  _cachedFbxMeshes.clear();

  deinit_fbx_sdk(fbxMgr);

  //
  info.GetReturnValue().Set(ret);
}

NAN_MODULE_INIT(init) {
  NAN_EXPORT(target, load);
}

NODE_MODULE(addon, init)
