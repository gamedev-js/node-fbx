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
 *   animations: [...],
 *   TODO: materials: [...],
 * }
 *
 * node = {
 *   name: '',
 *   type: '',
 *   position: [x, y, z],
 *   rotation: [x, y, z, w],
 *   scale: [x, y, z],
 *   children: [...],
 *
 *   // optional (resource reference ID)
 *   refID: -1,
 * }
 *
 * mesh = {
 *   name: '',
 *   points: [...],
 *   polygons: [...],
 *   polygonSizes: [...],
 *   normals: [...],
 *   tangents: [...],
 *   binormals: [...],
 *   colors: [...],
 *   uvs0: [...],
 *   uvs1: [...],
 *   ...: [...],
 *   uvs7 [...],
 * }
 *
 * TODO:
 * material = {
 *   name: '',
 *   diffuse: [r, g, b, a],
 *   ambient: [r, g, b, a],
 *   texDiffuse: -1,
 *   texNormal: -1,
 * }
 */
