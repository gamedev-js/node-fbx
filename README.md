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
  - [x] dump animation clip
  - [ ] dump skin
  - [ ] convert blend-shapes to meshes
  - [ ] triangulate mesh polygons
  - [x] split mesh data by normals, binormals, tangents, colors and uvs
  - [ ] divide mesh data to sub-meshes by material
  - [ ] calculate normals
  - [ ] calculate tangents from normals and uvs
  - [ ] compress binormals
  - [ ] support Windows (help wanted)
  - [ ] support Linux (help wanted)

## Reference

  - [FBX SDK documentation](http://help.autodesk.com/view/FBX/2016/ENU/)
  - [A quick tutorial about the FBX ASCII format](https://banexdevblog.wordpress.com/2014/06/23/a-quick-tutorial-about-the-fbx-ascii-format/)
  - [How to work with FBX SDK](http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/how-to-work-with-fbx-sdk-r3582)

## Reference Source Code

  - [lang1991/FBXExporter](https://github.com/lang1991/FBXExporter)
  - [NickHardeman/ofxFBX](https://github.com/NickHardeman/ofxFBX)
  - [libgdx/fbx-conv](https://github.com/libgdx/fbx-conv)
  - [guillaumeblanc/ozz-animation](https://github.com/guillaumeblanc/ozz-animation)
  - [FBX in Three.js](https://github.com/mrdoob/three.js/blob/dev/examples/js/loaders/FBXLoader.js)
  - [FBX in Babylon.js](https://github.com/BabylonJS/Babylon.js/tree/master/Exporters/FBX/BabylonFbxNative)

## Known Issues

1. Link the libfbxsdk.dylib

The fbxsdk use `@executable_path` as install path in MacOSX. We need to replace it with `@rpath` otherwise our
programme can not find the path to load the library. To do this, just run:

```bash
install_name_tool -id "@rpath/libfbxsdk.dylib" libfbxsdk.dylib
```

## License

MIT © 2016 Johnny Wu
