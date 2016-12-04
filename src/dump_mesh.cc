#include "./common.h"

void _extract_vec4 (FbxVector4 &_vec4, double *x, double *y, double *z) {
  // NOTE: it is possible in fbx contains NaN data
  *x = _vec4[0];
  if ( isnan(*x) ) {
    *x = 0.0;
  }

  *y = _vec4[1];
  if ( isnan(*y) ) {
    *y = 0.0;
  }

  *z = _vec4[2];
  if ( isnan(*z) ) {
    *z = 0.0;
  }
}

void _dump_points (v8::Local<v8::Object> _mesh, FbxMesh *_fbxMesh) {
  int count = _fbxMesh->GetControlPointsCount();
  FbxVector4 *fbxCtrlPoints = _fbxMesh->GetControlPoints();

  v8::Local<v8::Array> points = Nan::New<v8::Array>(count);

  for ( int i = 0; i < count; ++i ) {
    FbxVector4 &point = fbxCtrlPoints[i];
    double x, y, z;

    _extract_vec4( point, &x, &y, &z );

    Nan::Set(points, 3*i,     Nan::New(x));
    Nan::Set(points, 3*i + 1, Nan::New(y));
    Nan::Set(points, 3*i + 2, Nan::New(z));
  }

  // mesh.points = [...]
  Nan::Set(
    _mesh,
    Nan::New("points").ToLocalChecked(),
    points
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
  v8::Local<v8::Array> polygonSizes = Nan::New<v8::Array>(polygonCount);
  for ( int i = 0; i < polygonCount; ++i ) {
    Nan::Set(polygonSizes, i, Nan::New(_fbxMesh->GetPolygonSize(i)));
  }

  // mesh.polygonSizes = [...]
  Nan::Set(
    _mesh,
    Nan::New("polygonSizes").ToLocalChecked(),
    polygonSizes
  );
}


void _dump_normals (v8::Local<v8::Object> _mesh, FbxMesh *_fbxMesh) {
  FbxGeometryElementNormal *normalElement = _fbxMesh->GetElementNormal();

  if ( !normalElement ) {
    return;
  }

  FbxLayerElement::EMappingMode mappingMode = normalElement->GetMappingMode();
  FbxLayerElement::EReferenceMode refMode = normalElement->GetReferenceMode();

  int polygonVertexCount = _fbxMesh->GetPolygonVertexCount();
  int *polygonVertices = _fbxMesh->GetPolygonVertices();

  v8::Local<v8::Array> normals = Nan::New<v8::Array>(polygonVertexCount);

  if ( mappingMode == FbxLayerElement::eByControlPoint ) {
    for ( int i = 0; i < polygonVertexCount; ++i ) {
      int pointIndex = polygonVertices[i];
      int index = -1;

      if ( refMode == FbxGeometryElement::eDirect ) {
        index = pointIndex;
      } else if ( refMode == FbxGeometryElement::eIndexToDirect ) {
        index = normalElement->GetIndexArray().GetAt(pointIndex);
      }

      FbxVector4 normal = normalElement->GetDirectArray().GetAt(index);
      double x, y, z;
      _extract_vec4( normal, &x, &y, &z );

      Nan::Set(normals, 3*i,     Nan::New(x));
      Nan::Set(normals, 3*i + 1, Nan::New(y));
      Nan::Set(normals, 3*i + 2, Nan::New(z));
    }

  } else if ( mappingMode == FbxLayerElement::eByPolygonVertex ) {
    for ( int i = 0; i < polygonVertexCount; ++i ) {
      int index = -1;

      if ( refMode == FbxGeometryElement::eDirect ) {
        index = i;
      } else if ( refMode == FbxGeometryElement::eIndexToDirect ) {
        index = normalElement->GetIndexArray().GetAt(i);
      }

      FbxVector4 normal = normalElement->GetDirectArray().GetAt(index);
      double x, y, z;
      _extract_vec4( normal, &x, &y, &z );

      Nan::Set(normals, 3*i,     Nan::New(x));
      Nan::Set(normals, 3*i + 1, Nan::New(y));
      Nan::Set(normals, 3*i + 2, Nan::New(z));
    }

  } else if ( mappingMode == FbxLayerElement::eByPolygon ) {
    int i = 0;
    int polygonCount = _fbxMesh->GetPolygonCount();

    for ( int pi = 0; pi < polygonCount; ++pi ) {
      int polygonSize = _fbxMesh->GetPolygonSize(pi);

      for ( int pj = 0; pj < polygonSize; ++pj ) {
        int index = -1;

        if ( refMode == FbxGeometryElement::eDirect ) {
          index = pi;
        } else if ( refMode == FbxGeometryElement::eIndexToDirect ) {
          index = normalElement->GetIndexArray().GetAt(pi);
        }

        FbxVector4 normal = normalElement->GetDirectArray().GetAt(index);
        double x, y, z;
        _extract_vec4( normal, &x, &y, &z );

        Nan::Set(normals, 3*i,     Nan::New(x));
        Nan::Set(normals, 3*i + 1, Nan::New(y));
        Nan::Set(normals, 3*i + 2, Nan::New(z));

        i += 1;
      }
    }

  } else if ( mappingMode == FbxLayerElement::eByEdge ) {
    // TODO

  } else if ( mappingMode == FbxLayerElement::eAllSame ) {
    FbxVector4 normal = normalElement->GetDirectArray().GetAt(0);
    double x, y, z;
    _extract_vec4( normal, &x, &y, &z );

    for ( int i = 0; i < polygonVertexCount; ++i ) {
      Nan::Set(normals, 3*i,     Nan::New(x));
      Nan::Set(normals, 3*i + 1, Nan::New(y));
      Nan::Set(normals, 3*i + 2, Nan::New(z));
    }

  }

  // mesh.normals = [...]
  Nan::Set(
    _mesh,
    Nan::New("normals").ToLocalChecked(),
    normals
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

  // mesh.points = [...];
  _dump_points(mesh, fbxMesh);

  // mesh.polygons = [...];
  // mesh.polygonSizes = [...];
  _dump_polygons(mesh, fbxMesh);

  // mesh.normals = [...];
  _dump_normals(mesh, fbxMesh);

  return mesh;
}
