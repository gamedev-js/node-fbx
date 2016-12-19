'use strict';

const addon = require('./build/Release/addon.node');
const mesh = require('./lib/mesh');

module.exports = {
  load: addon.load,
  triangulatePolygons: mesh.triangulatePolygons,
  split: mesh.split,
};

// TODO:
// compress binormals
// since we have normals, tangents and binormals, we compress binormals by:
//   vec3 binormal2 = cross(normal, tangent)
//   float dir = dot(binormal, binormal2)
// in this way, binormal is saved in one float.

/**
 * scene = {
 *   nodes: [...],
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
 *   points: [...],
 *   polygons: [...],
 *   polygonSizes: [...],
 *   normals: [...],
 *   tangents: [...],
 *   colors: [...],
 *   uv0: [...],
 *   uv1: [...],
 *   uv2: [...],
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
