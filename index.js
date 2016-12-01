'use strict';

const bindings = require('bindings');
const addon = bindings('addon');

function _calculateIndices ( mesh ) {
  let polygons = mesh.polygons;
  let polygonTopologyList = mesh.polygonTopologyList;
  let indices = [];

  let i = 0;
  let curPolyIdx = 0;

  while ( i < polygons.length ) {
    let stride = polygonTopologyList[curPolyIdx];

    if ( stride !== 3 && stride !== 4 ) {
      console.error(`Invalid polygon topology size: ${stride}`);

      i += stride;
      ++curPolyIdx;

      continue;
    }

    // quad
    // a - d
    // |   |
    // b - c
    //
    // [( a, b, c, d ), ...]
    // => [( a, b, c ), (a, c, d ), ...]
    if ( stride === 4 ) {
      indices.push( polygons[i] );
      indices.push( polygons[i+1] );
      indices.push( polygons[i+2] );

      indices.push( polygons[i] );
      indices.push( polygons[i+2] );
      indices.push( polygons[i+3] );
    } else {
      indices.push( polygons[i] );
      indices.push( polygons[i+1] );
      indices.push( polygons[i+2] );
    }

    i += stride;
    ++curPolyIdx;
  }

  return indices;
}

module.exports = {
  load: addon.load,
  calculateIndices: _calculateIndices,
};

/**
 * scene = {
 *   root: { name: '', children: [
 *     { name: 'a', type: 'mesh', meshID: 0 },
 *     { name: 'b', type: '', children: [ ... ] },
 *   ]},
 *   meshes: [...],
 *   materials: [...],
 *   animationClips: [...],
 * }
 *
 * node = {
 *   name: '',
 *   type: '',
 *   position: [1, 2, 3],
 *   rotation: [1, 2, 3, 4],
 *   scale: [1, 2, 3],
 *   children: [...],
 *
 *   // optional
 *   meshID: -1,
 * }
 *
 * mesh = {
 *   name: '',
 *   vertices: [...],
 *   polygons: [...],
 *   polygonTopologyList: [...],
 *   normals: [...],
 *   tangents: [...],
 *   colors: [...],
 *   uvs_0: [...],
 *   uvs_1: [...],
 *   materials: [...],
 * }
 *
 * material = {
 *   name: '',
 *   diffuse: [1, 2, 3, 4],
 *   ambient: [1, 2, 3, 4],
 *   texDiffuse: -1,
 *   texNormal: -1,
 * }
 */
