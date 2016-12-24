#include "./common.h"

v8::Local<v8::Value> dump_node (FbxNode *_fbxNode) {
  // var node = {};
  v8::Local<v8::Object> node = Nan::New<v8::Object>();

  // node.name = "...";
  Nan::Set(
   node,
   Nan::New("name").ToLocalChecked(),
   Nan::New(_fbxNode->GetNameWithoutNameSpacePrefix().Buffer()).ToLocalChecked()
  );

  // get position, rotation and scale
  v8::Local<v8::Array> vec;
  FbxAMatrix& lclTransform = _fbxNode->EvaluateLocalTransform();

  // node.position = [...]
  FbxVector4 t = lclTransform.GetT();
  vec = Nan::New<v8::Array>(3);
  Nan::Set(vec, 0, Nan::New(t[0]));
  Nan::Set(vec, 1, Nan::New(t[1]));
  Nan::Set(vec, 2, Nan::New(t[2]));
  Nan::Set(node, Nan::New("position").ToLocalChecked(), vec);

  // node.rotation = [...]
  FbxQuaternion q = lclTransform.GetQ();
  vec = Nan::New<v8::Array>(4);
  Nan::Set(vec, 0, Nan::New(q[0]));
  Nan::Set(vec, 1, Nan::New(q[1]));
  Nan::Set(vec, 2, Nan::New(q[2]));
  Nan::Set(vec, 3, Nan::New(q[3]));
  Nan::Set(node, Nan::New("rotation").ToLocalChecked(), vec);

  // node.scale = [...]
  FbxVector4 s = lclTransform.GetS();
  vec = Nan::New<v8::Array>(3);
  Nan::Set(vec, 0, Nan::New(s[0]));
  Nan::Set(vec, 1, Nan::New(s[1]));
  Nan::Set(vec, 2, Nan::New(s[2]));
  Nan::Set(node, Nan::New("scale").ToLocalChecked(), vec);

  // TODO: node.properties = {...} (for user data)

  // node.type = "...";
  const char *typeName = "";

  FbxNodeAttribute *fbxNodeAttr = _fbxNode->GetNodeAttribute();
  if (fbxNodeAttr) {
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

      if ( fbxMesh ) {
        std::vector<FbxMesh *>::iterator it = std::find(_cachedFbxMeshes.begin(), _cachedFbxMeshes.end(), fbxMesh);
        int index = -1;

        // if we already dumped the mesh, skip it
        if ( it == _cachedFbxMeshes.end() ) {
          _cachedFbxMeshes.push_back(fbxMesh);
          index = _cachedFbxMeshes.size()-1;
        } else {
          index = it - _cachedFbxMeshes.begin();
        }

        Nan::Set(node, Nan::New("refID").ToLocalChecked(), Nan::New(index));
      }
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
