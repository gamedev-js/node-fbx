#include "./common.h"

std::vector<FbxMesh *> _cachedFbxMeshes;

/*
 * load fbx file and dump it
 *
 * args[0] - the file path
 * args[1] - the opts
 *
 * returns the dumpped scene object
 */

NAN_METHOD(load) {
  options_t opts;

  // set default options
  opts.importMesh = true;
  opts.importAnimation = true;
  opts.sampleRate = 60;

  // read argument0, path
  v8::Local<v8::String> arg0 = info[0].As<v8::String>();
  int len = arg0->Utf8Length();
  char *path = (char *)malloc((len+1) * sizeof(char));
  arg0->WriteUtf8(path);

  // read argument1, opts
  v8::Local<v8::Object> arg1 = info[1].As<v8::Object>();
  if ( !arg1->IsNull() && !arg1->IsUndefined() && arg1->IsObject() ) {
    v8::Local<v8::Value> prop;

    prop = Nan::Get(arg1, Nan::New("importMesh").ToLocalChecked()).ToLocalChecked();;
    if ( prop->IsBoolean() ) {
      opts.importMesh = prop->BooleanValue();
    }

    prop = Nan::Get(arg1, Nan::New("importAnimation").ToLocalChecked()).ToLocalChecked();;
    if ( prop->IsBoolean() ) {
      opts.importAnimation = prop->BooleanValue();
    }

    prop = Nan::Get(arg1, Nan::New("sampleRate").ToLocalChecked()).ToLocalChecked();;
    if ( prop->IsNumber() ) {
      opts.sampleRate = prop->Int32Value();
    }
  }

  // init SDK
  FbxManager *fbxMgr = NULL;
  FbxScene *fbxScene = NULL;
  init_fbx_sdk(fbxMgr, fbxScene);

  // load and process scene
  load_scene(fbxMgr, fbxScene, path);

  // dump with opts
  v8::Local<v8::Object> ret = dump_scene(fbxScene, &opts);

  // deinit
  free(path);
  _cachedFbxMeshes.clear();

  deinit_fbx_sdk(fbxMgr);

  //
  info.GetReturnValue().Set(ret);
}

NAN_MODULE_INIT(init) {
  NAN_EXPORT(target, load);
}

NODE_MODULE(addon, init)
