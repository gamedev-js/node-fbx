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

  // curves.translation_x = ...;
  fbxCurve = _fbxNode->LclTranslation.GetCurve( _fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X );
  if ( fbxCurve ) {
    Nan::Set(
      curves,
      Nan::New("translation_x").ToLocalChecked(),
      _dump_curve(fbxCurve)
    );
  }

  // curves.translation_y = ...;
  fbxCurve = _fbxNode->LclTranslation.GetCurve( _fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y );
  if ( fbxCurve ) {
    Nan::Set(
      curves,
      Nan::New("translation_y").ToLocalChecked(),
      _dump_curve(fbxCurve)
    );
  }

  // curves.translation_z = ...;
  fbxCurve = _fbxNode->LclTranslation.GetCurve( _fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z );
  if ( fbxCurve ) {
    Nan::Set(
      curves,
      Nan::New("translation_z").ToLocalChecked(),
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

v8::Local<v8::Object> _dump_tracks (FbxNode *_fbxNode, double _sampleRate, FbxTimeSpan _timeSpan) {
  // var tracks = {};
  v8::Local<v8::Object> tracks = Nan::New<v8::Object>();

  // tracks.times = [];
  v8::Local<v8::Array> times = Nan::New<v8::Array>();
  Nan::Set(tracks, Nan::New("times").ToLocalChecked(), times);

  // tracks.translations = [];
  v8::Local<v8::Array> translations = Nan::New<v8::Array>();
  Nan::Set(tracks, Nan::New("translations").ToLocalChecked(), translations);

  // tracks.scales = [];
  v8::Local<v8::Array> scales = Nan::New<v8::Array>();
  Nan::Set(tracks, Nan::New("scales").ToLocalChecked(), scales);

  // tracks.rotations = [];
  v8::Local<v8::Array> rotations = Nan::New<v8::Array>();
  Nan::Set(tracks, Nan::New("rotations").ToLocalChecked(), rotations);

  //
  bool loopAgain = true;
  double samplePeriod = 1.0/_sampleRate;
  double start = _timeSpan.GetStart().GetSecondDouble();
  double end = _timeSpan.GetStop().GetSecondDouble();
  double t = start;
  int i = 0;

  FbxAMatrix lastMatrix;

  while (loopAgain) {
    if ( t >= end ) {
      t = end;
      loopAgain = false;
    }

    FbxAMatrix &matrix = _fbxNode->GetParent() == NULL
      ? _fbxNode->EvaluateGlobalTransform(FbxTimeSeconds(t))
      : _fbxNode->EvaluateLocalTransform(FbxTimeSeconds(t))
      ;

    // skip same value keys
    if ( i != 0 ) {
      if ( matrix == lastMatrix ) {
        t += samplePeriod;
        ++i;
        continue;
      }
    }

    lastMatrix = matrix;
    v8::Local<v8::Array> tmpArray;

    // ===============
    // time
    // ===============

    double localTime = t - start;
    Nan::Set(times, i, Nan::New(localTime));

    // ===============
    // translation
    // ===============

    FbxVector4 fbxTrasnlation = matrix.GetT();

    // translations[i] = [x, y, z];
    tmpArray = Nan::New<v8::Array>(3);
    Nan::Set(tmpArray, 0, Nan::New(fbxTrasnlation[0]));
    Nan::Set(tmpArray, 1, Nan::New(fbxTrasnlation[1]));
    Nan::Set(tmpArray, 2, Nan::New(fbxTrasnlation[2]));

    Nan::Set(translations, i, tmpArray);

    // ===============
    // rotation
    // ===============

    FbxQuaternion fbxRotation = matrix.GetQ();

    // rotations[i] = [x, y, z, w];
    tmpArray = Nan::New<v8::Array>(4);
    Nan::Set(tmpArray, 0, Nan::New(fbxRotation[0]));
    Nan::Set(tmpArray, 1, Nan::New(fbxRotation[1]));
    Nan::Set(tmpArray, 2, Nan::New(fbxRotation[2]));
    Nan::Set(tmpArray, 3, Nan::New(fbxRotation[3]));

    Nan::Set(rotations, i, tmpArray);

    // ===============
    // scale
    // ===============

    FbxVector4 fbxScale = matrix.GetS();

    // scales[i] = [x, y, z];
    tmpArray = Nan::New<v8::Array>(3);
    Nan::Set(tmpArray, 0, Nan::New(fbxScale[0]));
    Nan::Set(tmpArray, 1, Nan::New(fbxScale[1]));
    Nan::Set(tmpArray, 2, Nan::New(fbxScale[2]));

    //
    Nan::Set(scales, i, tmpArray);

    //
    t += samplePeriod;
    ++i;
  }

  return tracks;
}

void _recurse_node (
  v8::Local<v8::Array> _nodes,
  const char *_path,
  FbxAnimLayer *_fbxAnimLayer,
  FbxNode *_fbxNode
) {
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
    _recurse_node( _nodes, path.c_str(), _fbxAnimLayer, _fbxNode->GetChild(i) );
  }
}

void _recurse_node_track (
  v8::Local<v8::Array> _nodes,
  const char *_path,
  FbxNode *_fbxNode,
  FbxTimeSpan _timeSpan,
  double _sampleRate
) {
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

  // node.tracks = {}
  Nan::Set(
    node,
    Nan::New("tracks").ToLocalChecked(),
    _dump_tracks( _fbxNode, _sampleRate, _timeSpan )
  );

  // nodes.push(node);
  Nan::Set(_nodes, _curNodeIdx, node);
  ++_curNodeIdx;

  for ( int i = 0; i < _fbxNode->GetChildCount(); ++i ) {
    _recurse_node_track( _nodes, path.c_str(), _fbxNode->GetChild(i), _timeSpan, _sampleRate );
  }
}

v8::Local<v8::Value> _dump_animation (FbxScene *_fbxScene, FbxAnimStack *_fbxAnimStack, double _sampleRate) {
  FbxNode *fbxRoot = _fbxScene->GetRootNode();
  _fbxScene->SetCurrentAnimationStack(_fbxAnimStack);

  // var animation = {};
  v8::Local<v8::Object> animation = Nan::New<v8::Object>();

  // animation.name = "...";
  Nan::Set(
    animation,
    Nan::New("name").ToLocalChecked(),
    Nan::New(_fbxAnimStack->GetName()).ToLocalChecked()
  );

  // animation.duration = ...;
  FbxTimeSpan timeSpan = _fbxAnimStack->GetLocalTimeSpan();
  double duration = timeSpan.GetDuration().GetSecondDouble();
  // NOTE: Animation duration could be 0 if it's just a pose. In this case we'll set a default 1s duration.
  if ( duration == 0.0 ) {
    duration = 1.0;
  }

  Nan::Set(
    animation,
    Nan::New("duration").ToLocalChecked(),
    Nan::New(timeSpan.GetDuration().GetSecondDouble())
  );

  // animation.sampleRate = ...;
  double sampleRate = _sampleRate;
  if ( sampleRate <= 0 ) {
    FbxTime::EMode mode = _fbxScene->GetGlobalSettings().GetTimeMode();
    if ( mode == FbxTime::eCustom ) {
      sampleRate = _fbxScene->GetGlobalSettings().GetCustomFrameRate();
    } else {
      sampleRate = FbxTime::GetFrameRate(mode);
    }
  }
  Nan::Set(
    animation,
    Nan::New("sampleRate").ToLocalChecked(),
    Nan::New(sampleRate)
  );

  // DISABLE:
  // animation.nodes = [...];
  // v8::Local<v8::Array> nodes = Nan::New<v8::Array>();
  // int layerCount = _fbxAnimStack->GetMemberCount<FbxAnimStack>();

  // // if we have multiple layers
  // if ( layerCount > 1 ) {
  //   // TODO: back the animations by layer ???
  // }

  // FbxAnimLayer *fbxAnimLayer = _fbxAnimStack->GetMember<FbxAnimLayer>(0);
  // for ( int i = 0; i < fbxRoot->GetChildCount(); ++i ) {
  //   _recurse_node( nodes, "", fbxAnimLayer, fbxRoot->GetChild(i) );
  // }

  // Nan::Set(
  //   animation,
  //   Nan::New("nodes").ToLocalChecked(),
  //   nodes
  // );
  // DISABLE: end

  //
  // animation.nodes = [...];
  v8::Local<v8::Array> nodes = Nan::New<v8::Array>();
  for ( int i = 0; i < fbxRoot->GetChildCount(); ++i ) {
    _recurse_node_track( nodes, "", fbxRoot->GetChild(i), timeSpan, sampleRate );
  }

  Nan::Set(
    animation,
    Nan::New("nodes").ToLocalChecked(),
    nodes
  );

  return animation;
}

v8::Local<v8::Array> dump_animations (FbxScene *_fbxScene, double _sampleRate) {
  int count = _fbxScene->GetSrcObjectCount<FbxAnimStack>();
  v8::Local<v8::Array> animations = Nan::New<v8::Array>(count);

  for (int i = 0; i < count; ++i) {
    FbxAnimStack* pAnimStack = _fbxScene->GetSrcObject<FbxAnimStack>(i);

    Nan::Set(animations, i, _dump_animation(_fbxScene, pAnimStack, _sampleRate));
  }

  return animations;
}
