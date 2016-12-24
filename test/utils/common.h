#ifndef _COMMON_H
#define _COMMON_H

#include <fbxsdk.h>
#include <nan.h>

#include <vector>

void init_fbx_sdk (FbxManager *&_fbxMgr, FbxScene *&_fbxScene);
void deinit_fbx_sdk (FbxManager *&_fbxMgr);

bool save_scene (FbxManager *_fbxMgr, FbxDocument *_fbxScene, const char *_filename, bool _ascii = false );

#endif // #ifndef _COMMON_H
