{
  "targets": [
    {
      "target_name": "addon",
      "sources": [
        "src/common.cc",
        "src/init.cc",
        "src/dump_scene.cc",
        "src/dump_node.cc",
        "src/dump_mesh.cc",
      ],
      "include_dirs": [
        "vendor/fbx-sdk/include",
        "<!(node -e \"require('nan')\")"
      ],
      "libraries": [
        "../vendor/fbx-sdk/lib/clang/release/libfbxsdk.a"
      ]
    }
  ]
}
