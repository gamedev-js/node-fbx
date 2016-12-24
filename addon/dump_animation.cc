#include "./common.h"

int _curNodeIdx = 0;
static const char *interpolation[] = { "?", "constant", "linear", "cubic" };

static int _interpolationToIndex (FbxAnimCurveDef::EInterpolationType _type) {
  switch (_type) {
  case FbxAnimCurveDef::eInterpolationConstant: return 1;
  case FbxAnimCurveDef::eInterpolationLinear: return 2;
  case FbxAnimCurveDef::eInterpolationCubic: return 3;
  default: return 0;
  }
}

v8::Local<v8::Array> _dump_curve (FbxAnimCurve *_fbxCurve) {

  // var keys = [];
  int count = _fbxCurve->KeyGetCount();
  v8::Local<v8::Array> keys = Nan::New<v8::Array>(count);

  for ( int i = 0; i < count; ++i ) {
    FbxAnimCurveKey fbxKey = _fbxCurve->KeyGet(i);

    // var key = {};
    v8::Local<v8::Object> key = Nan::New<v8::Object>();

    // key.time = ...;
    Nan::Set(
      key,
      Nan::New("time").ToLocalChecked(),
      Nan::New(fbxKey.GetTime().GetSecondDouble())
    );

    // key.value = ...;
    Nan::Set(
      key,
      Nan::New("value").ToLocalChecked(),
      Nan::New(fbxKey.GetValue())
    );

    // key.interpolation = ...;
    int idx = _interpolationToIndex(fbxKey.GetInterpolation());
    Nan::Set(
      key,
      Nan::New("interpolation").ToLocalChecked(),
      Nan::New(interpolation[idx]).ToLocalChecked()
    );

    //
    if ( fbxKey.GetInterpolation() == FbxAnimCurveDef::eInterpolationCubic ) {
      Nan::Set(
        key,
        Nan::New("leftTangent").ToLocalChecked(),
        Nan::New(_fbxCurve->KeyGetLeftDerivative(i))
      );
      Nan::Set(
        key,
        Nan::New("rightTangent").ToLocalChecked(),
        Nan::New(_fbxCurve->KeyGetRightDerivative(i))
      );
    }

    // keys[i] = key;
    Nan::Set(keys, i, key);
  }

  return keys;
}

v8::Local<v8::Object> _dump_curves (FbxAnimLayer *_fbxAnimLayer, FbxNode *_fbxNode) {
  // var curves = {};
  v8::Local<v8::Object> curves = Nan::New<v8::Object>();

  FbxAnimCurve *fbxCurve;

  // curves.position_x = ...;
  fbxCurve = _fbxNode->LclTranslation.GetCurve( _fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X );
  if ( fbxCurve ) {
    Nan::Set(
      curves,
      Nan::New("position_x").ToLocalChecked(),
      _dump_curve(fbxCurve)
    );
  }

  // curves.position_y = ...;
  fbxCurve = _fbxNode->LclTranslation.GetCurve( _fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y );
  if ( fbxCurve ) {
    Nan::Set(
      curves,
      Nan::New("position_y").ToLocalChecked(),
      _dump_curve(fbxCurve)
    );
  }

  // curves.position_z = ...;
  fbxCurve = _fbxNode->LclTranslation.GetCurve( _fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z );
  if ( fbxCurve ) {
    Nan::Set(
      curves,
      Nan::New("position_z").ToLocalChecked(),
      _dump_curve(fbxCurve)
    );
  }

  // curves.scale_x = ...;
  fbxCurve = _fbxNode->LclScaling.GetCurve( _fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X );
  if ( fbxCurve ) {
    Nan::Set(
      curves,
      Nan::New("scale_x").ToLocalChecked(),
      _dump_curve(fbxCurve)
    );
  }

  // curves.scale_y = ...;
  fbxCurve = _fbxNode->LclScaling.GetCurve( _fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y );
  if ( fbxCurve ) {
    Nan::Set(
      curves,
      Nan::New("scale_y").ToLocalChecked(),
      _dump_curve(fbxCurve)
    );
  }

  // curves.scale_z = ...;
  fbxCurve = _fbxNode->LclScaling.GetCurve( _fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z );
  if ( fbxCurve ) {
    Nan::Set(
      curves,
      Nan::New("scale_z").ToLocalChecked(),
      _dump_curve(fbxCurve)
    );
  }

  // curves.rotation_x = ...;
  fbxCurve = _fbxNode->LclRotation.GetCurve( _fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X );
  if ( fbxCurve ) {
    Nan::Set(
      curves,
      Nan::New("rotation_x").ToLocalChecked(),
      _dump_curve(fbxCurve)
    );
  }

  // curves.rotation_y = ...;
  fbxCurve = _fbxNode->LclRotation.GetCurve( _fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y );
  if ( fbxCurve ) {
    Nan::Set(
      curves,
      Nan::New("rotation_y").ToLocalChecked(),
      _dump_curve(fbxCurve)
    );
  }

  // curves.rotation_z = ...;
  fbxCurve = _fbxNode->LclRotation.GetCurve( _fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z );
  if ( fbxCurve ) {
    Nan::Set(
      curves,
      Nan::New("rotation_z").ToLocalChecked(),
      _dump_curve(fbxCurve)
    );
  }

  return curves;
}

void _dump_node_animation (v8::Local<v8::Array> _nodes, const char *_path, FbxAnimLayer *_fbxAnimLayer, FbxNode *_fbxNode) {
  // var node = {};
  v8::Local<v8::Object> node = Nan::New<v8::Object>();

  // node.path = '...'
  std::string path = _path;
  if ( path == "" ) {
    path = path + _fbxNode->GetName();
  } else {
    path = path + "/" + _fbxNode->GetName();
  }

  Nan::Set(
    node,
    Nan::New("path").ToLocalChecked(),
    Nan::New(path.c_str()).ToLocalChecked()
  );

  // node.curves = {}
  Nan::Set(
    node,
    Nan::New("curves").ToLocalChecked(),
    _dump_curves( _fbxAnimLayer, _fbxNode )
  );

  // nodes.push(node);
  Nan::Set(_nodes, _curNodeIdx, node);
  ++_curNodeIdx;

  for ( int i = 0; i < _fbxNode->GetChildCount(); ++i ) {
    _dump_node_animation( _nodes, path.c_str(), _fbxAnimLayer, _fbxNode->GetChild(i) );
  }
}

v8::Local<v8::Value> _dump_animation (FbxAnimStack *_fbxAnimStack, FbxNode *_fbxRoot) {
  // var animation = {};
  v8::Local<v8::Object> animation = Nan::New<v8::Object>();

  // animation.name = "...";
  Nan::Set(
    animation,
    Nan::New("name").ToLocalChecked(),
    Nan::New(_fbxAnimStack->GetName()).ToLocalChecked()
  );

  v8::Local<v8::Array> nodes = Nan::New<v8::Array>();
  int layerCount = _fbxAnimStack->GetMemberCount<FbxAnimStack>();

  // if we have multiple layers
  if ( layerCount > 1 ) {
    // TODO: back the animations by layer ???
  }

  FbxAnimLayer *fbxAnimLayer = _fbxAnimStack->GetMember<FbxAnimLayer>(0);
  for ( int i = 0; i < _fbxRoot->GetChildCount(); ++i ) {
    _dump_node_animation( nodes, "", fbxAnimLayer, _fbxRoot->GetChild(i) );
  }

  // animation.nodes = [...];
  Nan::Set(
    animation,
    Nan::New("nodes").ToLocalChecked(),
    nodes
  );

  return animation;
}

v8::Local<v8::Array> dump_animations (FbxScene *_fbxScene, int _sampleRate) {
  int count = _fbxScene->GetSrcObjectCount<FbxAnimStack>();
  v8::Local<v8::Array> animations = Nan::New<v8::Array>(count);

  for (int i = 0; i < count; ++i) {
    FbxAnimStack* pAnimStack = _fbxScene->GetSrcObject<FbxAnimStack>(i);

    Nan::Set(animations, i, _dump_animation(pAnimStack, _fbxScene->GetRootNode()));
  }

  return animations;
}
