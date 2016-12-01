#include "./common.h"

v8::Local<v8::Object> dump_scene (FbxScene *_fbxScene) {
  v8::Local<v8::Object> scene = Nan::New<v8::Object>();

  FbxNode *fbxRoot = _fbxScene->GetRootNode();

  // scene.nodes = [...];
  v8::Local<v8::Array> nodes = Nan::New<v8::Array>(fbxRoot->GetChildCount());
  for ( int i = 0; i < fbxRoot->GetChildCount(); ++i ) {
    Nan::Set(nodes, i, dump_node(fbxRoot->GetChild(i)));
  }
  Nan::Set(scene, Nan::New("nodes").ToLocalChecked(), nodes);

  // scene.meshes = [...];
  v8::Local<v8::Array> meshes = Nan::New<v8::Array>(_dumpedMeshes.size());
  for (uint i = 0; i < _dumpedMeshes.size(); ++i) {
    Nan::Set(meshes, i, _dumpedMeshes[i]);
  }
  Nan::Set(scene, Nan::New("meshes").ToLocalChecked(), meshes);

  return scene;
}
