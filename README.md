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
  - [x] dump mesh vertices
  - [x] dump mesh polygons
  - [ ] dump mesh uv
  - [ ] dump mesh normals
  - [ ] dump animation clip
  - [ ] dump skin

## Reference

  - [FBX SDK Documentation](http://help.autodesk.com/view/FBX/2016/ENU/)
  - [A QUICK TUTORIAL ABOUT THE FBX ASCII FORMAT](https://banexdevblog.wordpress.com/2014/06/23/a-quick-tutorial-about-the-fbx-ascii-format/)
  - [FBXLoader in Three.js](https://github.com/mrdoob/three.js/blob/dev/examples/js/loaders/FBXLoader.js)
  - [BabylonFbxNative](https://github.com/BabylonJS/Babylon.js/tree/master/Exporters/FBX/BabylonFbxNative)

For people who is getting start to write native code for node.js:

  - https://github.com/nodejs/nan#api
  - https://github.com/fcanas/node-native-boilerplate/blob/master/functions.cc

## License

MIT © 2016 Johnny Wu
