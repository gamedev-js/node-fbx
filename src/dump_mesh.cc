#include "./common.h"

#ifndef MIN
  #define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

void _extract_vec2 (FbxVector2 &_vec2, double *x, double *y) {
  // NOTE: it is possible in fbx contains NaN data
  *x = _vec2[0];
  if ( isnan(*x) ) {
    *x = 0.0;
  }

  *y = _vec2[1];
  if ( isnan(*y) ) {
    *y = 0.0;
  }
}

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

void _extract_color (FbxColor &_color, double *r, double *g, double *b, double *a) {
  *r = _color[0];
  *g = _color[1];
  *b = _color[2];
  *a = _color[3];
}

template <typename T>
v8::Local<v8::Array> _dump_element_vec4 ( FbxMesh *_fbxMesh, T *_element ) {
  FbxLayerElement::EMappingMode mappingMode = _element->GetMappingMode();
  FbxLayerElement::EReferenceMode refMode = _element->GetReferenceMode();

  int polygonVertexCount = _fbxMesh->GetPolygonVertexCount();
  int *polygonVertices = _fbxMesh->GetPolygonVertices();

  v8::Local<v8::Array> results = Nan::New<v8::Array>(polygonVertexCount);

  if ( mappingMode == FbxLayerElement::eByControlPoint ) {
    for ( int i = 0; i < polygonVertexCount; ++i ) {
      int pointIndex = polygonVertices[i];
      int index = -1;

      if ( refMode == FbxGeometryElement::eDirect ) {
        index = pointIndex;
      } else if ( refMode == FbxGeometryElement::eIndexToDirect ) {
        index = _element->GetIndexArray().GetAt(pointIndex);
      }

      FbxVector4 data = _element->GetDirectArray().GetAt(index);
      double x, y, z;
      _extract_vec4( data, &x, &y, &z );

      Nan::Set(results, 3*i,     Nan::New(x));
      Nan::Set(results, 3*i + 1, Nan::New(y));
      Nan::Set(results, 3*i + 2, Nan::New(z));
    }

  } else if ( mappingMode == FbxLayerElement::eByPolygonVertex ) {
    for ( int i = 0; i < polygonVertexCount; ++i ) {
      int index = -1;

      if ( refMode == FbxGeometryElement::eDirect ) {
        index = i;
      } else if ( refMode == FbxGeometryElement::eIndexToDirect ) {
        index = _element->GetIndexArray().GetAt(i);
      }

      FbxVector4 data = _element->GetDirectArray().GetAt(index);
      double x, y, z;
      _extract_vec4( data, &x, &y, &z );

      Nan::Set(results, 3*i,     Nan::New(x));
      Nan::Set(results, 3*i + 1, Nan::New(y));
      Nan::Set(results, 3*i + 2, Nan::New(z));
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
          index = _element->GetIndexArray().GetAt(pi);
        }

        FbxVector4 data = _element->GetDirectArray().GetAt(index);
        double x, y, z;
        _extract_vec4( data, &x, &y, &z );

        Nan::Set(results, 3*i,     Nan::New(x));
        Nan::Set(results, 3*i + 1, Nan::New(y));
        Nan::Set(results, 3*i + 2, Nan::New(z));

        i += 1;
      }
    }

  } else if ( mappingMode == FbxLayerElement::eByEdge ) {
    // TODO

  } else if ( mappingMode == FbxLayerElement::eAllSame ) {
    FbxVector4 data = _element->GetDirectArray().GetAt(0);
    double x, y, z;
    _extract_vec4( data, &x, &y, &z );

    for ( int i = 0; i < polygonVertexCount; ++i ) {
      Nan::Set(results, 3*i,     Nan::New(x));
      Nan::Set(results, 3*i + 1, Nan::New(y));
      Nan::Set(results, 3*i + 2, Nan::New(z));
    }

  }

  return results;
}

template <typename T>
v8::Local<v8::Array> _dump_element_color ( FbxMesh *_fbxMesh, T *_element ) {
  FbxLayerElement::EMappingMode mappingMode = _element->GetMappingMode();
  FbxLayerElement::EReferenceMode refMode = _element->GetReferenceMode();

  int polygonVertexCount = _fbxMesh->GetPolygonVertexCount();
  int *polygonVertices = _fbxMesh->GetPolygonVertices();

  v8::Local<v8::Array> results = Nan::New<v8::Array>(polygonVertexCount);

  if ( mappingMode == FbxLayerElement::eByControlPoint ) {
    for ( int i = 0; i < polygonVertexCount; ++i ) {
      int pointIndex = polygonVertices[i];
      int index = -1;

      if ( refMode == FbxGeometryElement::eDirect ) {
        index = pointIndex;
      } else if ( refMode == FbxGeometryElement::eIndexToDirect ) {
        index = _element->GetIndexArray().GetAt(pointIndex);
      }

      FbxColor data = _element->GetDirectArray().GetAt(index);
      double r, g, b, a;
      _extract_color( data, &r, &g, &b, &a );

      Nan::Set(results, 4*i,     Nan::New(r));
      Nan::Set(results, 4*i + 1, Nan::New(g));
      Nan::Set(results, 4*i + 2, Nan::New(b));
      Nan::Set(results, 4*i + 3, Nan::New(a));
    }

  } else if ( mappingMode == FbxLayerElement::eByPolygonVertex ) {
    for ( int i = 0; i < polygonVertexCount; ++i ) {
      int index = -1;

      if ( refMode == FbxGeometryElement::eDirect ) {
        index = i;
      } else if ( refMode == FbxGeometryElement::eIndexToDirect ) {
        index = _element->GetIndexArray().GetAt(i);
      }

      FbxColor data = _element->GetDirectArray().GetAt(index);
      double r, g, b, a;
      _extract_color( data, &r, &g, &b, &a );

      Nan::Set(results, 4*i,     Nan::New(r));
      Nan::Set(results, 4*i + 1, Nan::New(g));
      Nan::Set(results, 4*i + 2, Nan::New(b));
      Nan::Set(results, 4*i + 3, Nan::New(a));
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
          index = _element->GetIndexArray().GetAt(pi);
        }

        FbxColor data = _element->GetDirectArray().GetAt(index);
        double r, g, b, a;
        _extract_color( data, &r, &g, &b, &a );

        Nan::Set(results, 4*i,     Nan::New(r));
        Nan::Set(results, 4*i + 1, Nan::New(g));
        Nan::Set(results, 4*i + 2, Nan::New(b));
        Nan::Set(results, 4*i + 3, Nan::New(a));

        i += 1;
      }
    }

  } else if ( mappingMode == FbxLayerElement::eByEdge ) {
    // TODO

  } else if ( mappingMode == FbxLayerElement::eAllSame ) {
    FbxColor data = _element->GetDirectArray().GetAt(0);
    double r, g, b, a;
    _extract_color( data, &r, &g, &b, &a );

    for ( int i = 0; i < polygonVertexCount; ++i ) {
      Nan::Set(results, 4*i,     Nan::New(r));
      Nan::Set(results, 4*i + 1, Nan::New(g));
      Nan::Set(results, 4*i + 2, Nan::New(b));
      Nan::Set(results, 4*i + 3, Nan::New(a));
    }

  }

  return results;
}

template <typename T>
v8::Local<v8::Array> _dump_element_vec2 ( FbxMesh *_fbxMesh, T *_element ) {
  FbxLayerElement::EMappingMode mappingMode = _element->GetMappingMode();
  FbxLayerElement::EReferenceMode refMode = _element->GetReferenceMode();

  int polygonVertexCount = _fbxMesh->GetPolygonVertexCount();
  int *polygonVertices = _fbxMesh->GetPolygonVertices();

  v8::Local<v8::Array> results = Nan::New<v8::Array>(polygonVertexCount);

  if ( mappingMode == FbxLayerElement::eByControlPoint ) {
    for ( int i = 0; i < polygonVertexCount; ++i ) {
      int pointIndex = polygonVertices[i];
      int index = -1;

      if ( refMode == FbxGeometryElement::eDirect ) {
        index = pointIndex;
      } else if ( refMode == FbxGeometryElement::eIndexToDirect ) {
        index = _element->GetIndexArray().GetAt(pointIndex);
      }

      FbxVector2 data = _element->GetDirectArray().GetAt(index);
      double x, y;
      _extract_vec2( data, &x, &y );

      Nan::Set(results, 2*i,     Nan::New(x));
      Nan::Set(results, 2*i + 1, Nan::New(y));
    }

  } else if ( mappingMode == FbxLayerElement::eByPolygonVertex ) {
    for ( int i = 0; i < polygonVertexCount; ++i ) {
      int index = -1;

      if ( refMode == FbxGeometryElement::eDirect ) {
        index = i;
      } else if ( refMode == FbxGeometryElement::eIndexToDirect ) {
        index = _element->GetIndexArray().GetAt(i);
      }

      FbxVector2 data = _element->GetDirectArray().GetAt(index);
      double x, y;
      _extract_vec2( data, &x, &y );

      Nan::Set(results, 2*i,     Nan::New(x));
      Nan::Set(results, 2*i + 1, Nan::New(y));
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
          index = _element->GetIndexArray().GetAt(pi);
        }

        FbxVector2 data = _element->GetDirectArray().GetAt(index);
        double x, y;
        _extract_vec2( data, &x, &y );

        Nan::Set(results, 2*i,     Nan::New(x));
        Nan::Set(results, 2*i + 1, Nan::New(y));

        i += 1;
      }
    }

  } else if ( mappingMode == FbxLayerElement::eByEdge ) {
    // TODO

  } else if ( mappingMode == FbxLayerElement::eAllSame ) {
    FbxVector2 data = _element->GetDirectArray().GetAt(0);
    double x, y;
    _extract_vec2( data, &x, &y );

    for ( int i = 0; i < polygonVertexCount; ++i ) {
      Nan::Set(results, 2*i,     Nan::New(x));
      Nan::Set(results, 2*i + 1, Nan::New(y));
    }

  }

  return results;
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
  FbxGeometryElementNormal *element = _fbxMesh->GetElementNormal();

  if ( !element ) {
    return;
  }

  v8::Local<v8::Array> results = _dump_element_vec4(_fbxMesh, element);

  // mesh.normals = [...]
  Nan::Set(
    _mesh,
    Nan::New("normals").ToLocalChecked(),
    results
  );
}

void _dump_tangents (v8::Local<v8::Object> _mesh, FbxMesh *_fbxMesh) {
  FbxGeometryElementTangent *element = _fbxMesh->GetElementTangent();

  if ( !element ) {
    return;
  }

  v8::Local<v8::Array> results = _dump_element_vec4(_fbxMesh, element);

  // mesh.tangents = [...]
  Nan::Set(
    _mesh,
    Nan::New("tangents").ToLocalChecked(),
    results
  );
}

void _dump_binormals (v8::Local<v8::Object> _mesh, FbxMesh *_fbxMesh) {
  FbxGeometryElementTangent *element = _fbxMesh->GetElementTangent();

  if ( !element ) {
    return;
  }

  v8::Local<v8::Array> results = _dump_element_vec4(_fbxMesh, element);

  // mesh.binormals = [...]
  Nan::Set(
    _mesh,
    Nan::New("binormals").ToLocalChecked(),
    results
  );
}

void _dump_colors (v8::Local<v8::Object> _mesh, FbxMesh *_fbxMesh) {
  FbxGeometryElementVertexColor *element = _fbxMesh->GetElementVertexColor();

  if ( !element ) {
    return;
  }

  v8::Local<v8::Array> results = _dump_element_color(_fbxMesh, element);

  // mesh.colors = [...]
  Nan::Set(
    _mesh,
    Nan::New("colors").ToLocalChecked(),
    results
  );
}

void _dump_uvs (v8::Local<v8::Object> _mesh, FbxMesh *_fbxMesh) {
  const char *nameList[] = {
    "uvs0",
    "uvs1",
    "uvs2",
    "uvs3",
    "uvs4",
    "uvs5",
    "uvs7",
    "uvs7",
  };
  int uvCount = _fbxMesh->GetElementUVCount();
  uvCount = MIN(uvCount, 8);

  for ( int l = 0; l < uvCount; ++l ) {
    FbxLayerElementUV *element = _fbxMesh->GetElementUV(l);

    if ( !element ) {
      return;
    }

    v8::Local<v8::Array> results = _dump_element_vec2(_fbxMesh, element);

    // mesh.uvs = [...]
    Nan::Set(
      _mesh,
      Nan::New(nameList[l]).ToLocalChecked(),
      results
    );
  }
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

  // mesh.tangents = [...];
  _dump_tangents(mesh, fbxMesh);

  // mesh.binormals = [...];
  _dump_binormals(mesh, fbxMesh);

  // mesh.colors = [...];
  _dump_colors(mesh, fbxMesh);

  // mesh.uvs = [...];
  // mesh.uvs1
  // mesh.uvs2
  // ...
  _dump_uvs(mesh, fbxMesh);

  return mesh;
}
