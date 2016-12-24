#include "./common.h"

v8::Local<v8::Object> dump_scene (FbxScene *_fbxScene, options_t *_opts) {
  v8::Local<v8::Object> scene = Nan::New<v8::Object>();

  FbxNode *fbxRoot = _fbxScene->GetRootNode();

  // scene.nodes = [...];
  v8::Local<v8::Array> nodes = Nan::New<v8::Array>(fbxRoot->GetChildCount());
  for ( int i = 0; i < fbxRoot->GetChildCount(); ++i ) {
    Nan::Set(nodes, i, dump_node(fbxRoot->GetChild(i)));
  }
  Nan::Set(scene, Nan::New("nodes").ToLocalChecked(), nodes);

  // if importMesh
  if ( _opts->importMesh ) {
    // scene.meshes = [...];
    v8::Local<v8::Array> meshes = Nan::New<v8::Array>(_cachedFbxMeshes.size());
    for ( uint i = 0; i < _cachedFbxMeshes.size(); ++i ) {
      Nan::Set(meshes, i, dump_mesh(_cachedFbxMeshes[i]));
    }
    Nan::Set(scene, Nan::New("meshes").ToLocalChecked(), meshes);
  }

  // if importAnimation
  if ( _opts->importAnimation ) {
    // scene.animations = [...];
    Nan::Set(
      scene,
      Nan::New("animations").ToLocalChecked(),
      dump_animations(_fbxScene, _opts->sampleRate)
    );
  }

  return scene;
}
