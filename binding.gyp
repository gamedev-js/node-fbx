{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "addon/common.cc",
        "addon/init.cc",
        "addon/dump_scene.cc",
        "addon/dump_node.cc",
        "addon/dump_mesh.cc",
        "addon/dump_animation.cc",
      ],
      "include_dirs": [
        "vendor/fbx-sdk/include",
        "<!(node -e \"require('nan')\")"
      ],
      "xcode_settings": {
        "CLANG_CXX_LANGUAGE_STANDARD": "c++11",
        "CLANG_CXX_LIBRARY": "libc++",
        "OTHER_CPLUSPLUSFLAGS": [
          "-Wno-ignored-qualifiers",
        "-Wno-missing-field-initializers"
        ],
      },
      'conditions': [
        ["OS == 'mac'", {
          "libraries": [
            "../vendor/fbx-sdk/lib/clang/release/libfbxsdk.a",
            #DISABLE: "-rpath '@loader_path/../../vendor/fbx-sdk/lib/clang/release'"
          ],
        }]
      ],
    },
    {
      "target_name": "utils",
      "sources": [
        "test/utils/common.cc",
        "test/utils/init.cc",
      ],
      "include_dirs": [
        "vendor/fbx-sdk/include",
        "<!(node -e \"require('nan')\")"
      ],
      "libraries": [
        "../vendor/fbx-sdk/lib/clang/release/libfbxsdk.a",
      ],
      "xcode_settings": {
        "CLANG_CXX_LANGUAGE_STANDARD": "c++11",
        "CLANG_CXX_LIBRARY": "libc++",
        "OTHER_CPLUSPLUSFLAGS": [
          "-Wno-ignored-qualifiers",
          "-Wno-missing-field-initializers"
        ],
      },
    }
  ]
}
