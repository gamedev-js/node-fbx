#include "./common.h"

void _dump_vertices (v8::Local<v8::Object> _mesh, FbxMesh *_fbxMesh) {
  int vertCount = _fbxMesh->GetControlPointsCount();
  FbxVector4 *fbxVertices = _fbxMesh->GetControlPoints();

  v8::Local<v8::Array> vertices = Nan::New<v8::Array>(3*vertCount);

  for ( int i = 0; i < vertCount; ++i ) {
    FbxVector4 &point = fbxVertices[i];

    // NOTE: it is possible in fbx contains NaN data
    double x = point[0];
    if ( isnan(x) ) {
      x = 0.0;
    }

    double y = point[1];
    if ( isnan(y) ) {
      y = 0.0;
    }

    double z = point[2];
    if ( isnan(z) ) {
      z = 0.0;
    }

    Nan::Set(vertices, 3*i,     Nan::New(x));
    Nan::Set(vertices, 3*i + 1, Nan::New(y));
    Nan::Set(vertices, 3*i + 2, Nan::New(z));
  }

  // mesh.vertices = [...]
  Nan::Set(
    _mesh,
    Nan::New("vertices").ToLocalChecked(),
    vertices
  );
}

void _dump_polygons (v8::Local<v8::Object> _mesh, FbxMesh *_fbxMesh) {
  int polygonVertexCount = _fbxMesh->GetPolygonVertexCount();
  int *fbxPolygonVertices = _fbxMesh->GetPolygonVertices();

  v8::Local<v8::Array> polygons = Nan::New<v8::Array>(polygonVertexCount);
  for ( int i = 0; i < polygonVertexCount; ++i ) {
    Nan::Set(polygons, i, Nan::New(fbxPolygonVertices[i]));
  }

  // mesh.polygons = [...]
  Nan::Set(
    _mesh,
    Nan::New("polygons").ToLocalChecked(),
    polygons
  );

  //
  int polygonCount = _fbxMesh->GetPolygonCount();
  v8::Local<v8::Array> polygonTopologyList = Nan::New<v8::Array>(polygonCount);
  for ( int i = 0; i < polygonCount; ++i ) {
    Nan::Set(polygonTopologyList, i, Nan::New(_fbxMesh->GetPolygonSize(i)));
  }

  // mesh.polygonTopologyList = [...]
  Nan::Set(
    _mesh,
    Nan::New("polygonTopologyList").ToLocalChecked(),
    polygonTopologyList
  );
}

v8::Local<v8::Value> dump_mesh (FbxNode *_fbxNode) {
  FbxMesh *fbxMesh = (FbxMesh *)_fbxNode->GetNodeAttribute ();

  // var mesh = {};
  v8::Local<v8::Object> mesh = Nan::New<v8::Object>();

  // mesh.name = "";
  Nan::Set(
    mesh,
    Nan::New("name").ToLocalChecked(),
    Nan::New(_fbxNode->GetNameWithoutNameSpacePrefix().Buffer()).ToLocalChecked()
  );

  // mesh.vertices = [...];
  _dump_vertices(mesh, fbxMesh);

  // mesh.polygons = [...];
  // mesh.polygonTopologyList = [...];
  _dump_polygons(mesh, fbxMesh);

  return mesh;
}
