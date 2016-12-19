'use strict';

const { v2_equals, v3_equals, v3_dot } = require('./utils');

// split the vector when it large than 1 radias
const vecDotDiff = Math.cos( Math.PI/180.0 );
const uvDiff = 0.001;
const colorDiff = 0.00001;
const vertexAttrs = [
  'normals', 'tangents', 'binormals', 'colors',
  'uvs0', 'uvs1', 'uvs2', 'uvs3',
  'uvs4', 'uvs5', 'uvs6', 'uvs7',
];

function _needSplit ( _srcMesh, _dstMesh, _srcIdx, _dstIdx ) {
  // check if split normal
  if ( _srcMesh.normals ) {
    if ( v3_dot(_srcMesh.normals[_srcIdx], _dstMesh.normals[_dstIdx]) < vecDotDiff ) {
      return true;
    }
  }

  // check if split tangent
  if ( _srcMesh.tangents ) {
    if ( v3_dot(_srcMesh.tangents[_srcIdx], _dstMesh.tangents[_dstIdx]) < vecDotDiff ) {
      return true;
    }
  }

  // check if split binormal
  if ( _srcMesh.binormals ) {
    if ( v3_dot(_srcMesh.binormals[_srcIdx], _dstMesh.binormals[_dstIdx]) < vecDotDiff ) {
      return true;
    }
  }

  // check if split colors
  if ( _srcMesh.colors ) {
    if ( !v3_equals(_srcMesh.colors[_srcIdx], _dstMesh.colors[_dstIdx], colorDiff) ) {
      return true;
    }
  }

  // check if split uv
  for ( let i = 0; i < 7; ++i ) {
    let uv = `uvs${i}`;
    if ( _srcMesh[uv] ) {
      if ( !v2_equals(_srcMesh[uv][_srcIdx], _dstMesh[uv][_dstIdx], uvDiff) ) {
        return true;
      }
    }
  }

  //
  return false;
}

function _copyVertex ( mesh, dstMesh, pointIndex, polygonIndex, dstPointIndex ) {
  dstMesh.points[dstPointIndex] = mesh.points[pointIndex];

  vertexAttrs.forEach(attr => {
    if ( mesh[attr] ) {
      dstMesh[attr][dstPointIndex] = mesh[attr][polygonIndex];
    }
  });
}

function _triangulatePolygons ( mesh ) {
  let polygons = mesh.polygons;
  let polygonSizes = mesh.polygonSizes;
  let indices = [];

  let i = 0;
  let curPolyIdx = 0;

  while ( i < polygons.length ) {
    let stride = polygonSizes[curPolyIdx];

    if ( stride === 3 ) {
      indices.push( polygons[i] );
      indices.push( polygons[i+1] );
      indices.push( polygons[i+2] );

    } else if ( stride === 4 ) {
      // quad
      // a - d
      // |   |
      // b - c
      //
      // [( a, b, c, d ), ...]
      // => [( a, b, c ), (a, c, d ), ...]

      indices.push( polygons[i] );
      indices.push( polygons[i+1] );
      indices.push( polygons[i+2] );

      indices.push( polygons[i] );
      indices.push( polygons[i+2] );
      indices.push( polygons[i+3] );
    } else {
      // TODO: use libtess here
      console.error(`Invalid polygon topology size: ${stride}, (Not implemented yet)`);
    }

    i += stride;
    ++curPolyIdx;
  }

  return indices;
}

function _split ( mesh ) {
  let dstMesh = {};

  dstMesh.name = mesh.name;
  dstMesh.polygons = new Array(mesh.polygons.length);
  dstMesh.polygonSizes = mesh.polygonSizes.slice();

  dstMesh.points = mesh.points.slice();

  vertexAttrs.forEach(attr => {
    if ( mesh[attr] ) {
      dstMesh[attr] = new Array(mesh.points.length);
    }
  });

  // store the splitted indices by point-id
  let possiblePoints = new Array(mesh.points.length);
  for ( let i = 0; i < possiblePoints.length; ++i ) {
    possiblePoints[i] = [];
  }

  let curVertIdx = 0;

  for ( let i = 0; i < mesh.polygons.length; ++i ) {
    let pointIndex = mesh.polygons[i];
    let dstIndices = possiblePoints[pointIndex];
    let resultIdx = -1;

    // check if we can reuse the points and its attributes
    for ( let j = 0; j < dstIndices.length; ++j ) {
      if ( !_needSplit( mesh, dstMesh, i, dstIndices[j]) ) {
        resultIdx = dstIndices[j];
      }
    }

    // if we don't find any points can be resued, push a new one
    if ( resultIdx === -1 ) {
      _copyVertex( mesh, dstMesh, pointIndex, i, curVertIdx );
      resultIdx = curVertIdx;

      dstIndices.push(resultIdx);
      ++curVertIdx;
    }

    dstMesh.polygons[i] = resultIdx;
  }

  return dstMesh;
}

module.exports = {
  triangulatePolygons: _triangulatePolygons,
  split: _split,
};
