NOTE: This project is still work in progress.

## Dependencies

  - The FBX SDK - http://www.autodesk.com/products/fbx/overview
  - node-gyp - https://github.com/nodejs/node-gyp
  - nan - https://github.com/nodejs/nan

## Install

  1. Download and install the FBX SDK to `vendor/fbx-sdk/`
  2. run `npm install`

## Usage

  TODO

## TODO

  - [x] dump node hierarchy
  - [x] dump mesh control-points
  - [x] dump mesh polygons
  - [x] dump mesh polygon sizes
  - [x] dump mesh normals
  - [x] dump mesh tangents and binormals
  - [x] dump mesh uvs, uvs1, uvs2, ...
  - [x] dump mesh colors
  - [ ] dump mesh blend shapes
  - [ ] dump animation clip
  - [ ] dump skin
  - [ ] convert blend-shapes to meshes
  - [ ] triangulate mesh polygons
  - [x] split mesh data by normals, binormals, tangents, colors and uvs
  - [ ] compress binormals
  - [ ] support Windows (help wanted)
  - [ ] support Linux (help wanted)

## Reference

  - [FBX SDK documentation](http://help.autodesk.com/view/FBX/2016/ENU/)
  - [A quick tutorial about the FBX ASCII format](https://banexdevblog.wordpress.com/2014/06/23/a-quick-tutorial-about-the-fbx-ascii-format/)
  - [FBX in Three.js](https://github.com/mrdoob/three.js/blob/dev/examples/js/loaders/FBXLoader.js)
  - [FBX in Babylon.js](https://github.com/BabylonJS/Babylon.js/tree/master/Exporters/FBX/BabylonFbxNative)

For people who is getting start to write native code for node.js:

  - https://github.com/nodejs/nan#api
  - https://github.com/fcanas/node-native-boilerplate/blob/master/functions.cc

## License

MIT © 2016 Johnny Wu
