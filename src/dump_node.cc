#include "./common.h"

v8::Local<v8::Value> dump_node (FbxNode *_fbxNode) {
  FbxNodeAttribute *fbxNodeAttr = _fbxNode->GetNodeAttribute();
  if (!fbxNodeAttr) {
    printf("Warning: NULL Node Attribute\n\n");
    return Nan::Null();
  }

  // var node = {};
  v8::Local<v8::Object> node = Nan::New<v8::Object>();

  // node.name = "...";
  Nan::Set(
   node,
   Nan::New("name").ToLocalChecked(),
   Nan::New(_fbxNode->GetNameWithoutNameSpacePrefix().Buffer()).ToLocalChecked()
  );

  // node.type = "...";
  const char *typeName = "";
  FbxNodeAttribute::EType fbxAttrType = fbxNodeAttr->GetAttributeType();
  switch (fbxAttrType) {
  default: break;
  case FbxNodeAttribute::eMarker: typeName = "marker"; break;
  case FbxNodeAttribute::eSkeleton: typeName = "skeleton"; break;
  case FbxNodeAttribute::eMesh: typeName = "mesh"; break;
  case FbxNodeAttribute::eNurbs: typeName = "nurbs"; break;
  case FbxNodeAttribute::ePatch: typeName = "patch"; break;
  case FbxNodeAttribute::eCamera: typeName = "camera"; break;
  case FbxNodeAttribute::eLight: typeName = "light"; break;
  case FbxNodeAttribute::eLODGroup: typeName = "lodgroup"; break;
  }
  Nan::Set(node, Nan::New("type").ToLocalChecked(), Nan::New(typeName).ToLocalChecked());

  // mesh
  if ( fbxAttrType == FbxNodeAttribute::eMesh ) {
    FbxMesh* fbxMesh = (FbxMesh*) _fbxNode->GetNodeAttribute ();

    // if we already dumped the mesh, skip it
    if ( fbxMesh && std::find(_cachedFbxMeshes.begin(), _cachedFbxMeshes.end(), fbxMesh) == _cachedFbxMeshes.end() ) {
      _cachedFbxMeshes.push_back(fbxMesh);
      _dumpedMeshes.push_back(dump_mesh(_fbxNode));
    }
  }

  // node.children = [a, b, c, ...];
  v8::Local<v8::Array> children = Nan::New<v8::Array>(_fbxNode->GetChildCount());
  for ( int i = 0; i < _fbxNode->GetChildCount(); ++i ) {
    Nan::Set(children, i, dump_node(_fbxNode->GetChild(i)));
  }
  Nan::Set(node, Nan::New("children").ToLocalChecked(), children);

  // return node;
  return node;
}
