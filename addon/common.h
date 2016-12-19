#ifndef _COMMON_H
#define _COMMON_H

#include <fbxsdk.h>
#include <nan.h>

#include <vector>

void init_fbx_sdk (FbxManager *&_fbxMgr, FbxScene *&_fbxScene);
void deinit_fbx_sdk (FbxManager *&_fbxMgr);

bool load_scene (FbxManager *_fbxMgr, FbxDocument *_fbxScene, const char *_filename);

v8::Local<v8::Object> dump_scene (FbxScene *_fbxScene);
v8::Local<v8::Value> dump_node (FbxNode *_fbxNode);
v8::Local<v8::Value> dump_mesh (FbxNode *_fbxNode);
v8::Local<v8::Array> dump_animations (FbxScene *_fbxScene);

extern std::vector<v8::Local<v8::Value>> _dumpedMeshes;
extern std::vector<FbxMesh *> _cachedFbxMeshes;

#endif // #ifndef _COMMON_H
