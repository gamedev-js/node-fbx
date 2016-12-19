#include "./common.h"

#ifdef IOS_REF
  #undef IOS_REF
  #define IOS_REF (*(_fbxMgr->GetIOSettings()))
#endif

void init_fbx_sdk (FbxManager *&_fbxMgr, FbxScene *&_fbxScene) {
  // The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
  _fbxMgr = FbxManager::Create();
  if (!_fbxMgr) {
    printf("Error: Unable to create FBX Manager!\n");
    exit(1);
  }

  // DISABLE
  // printf("Autodesk FBX SDK version %s\n", _fbxMgr->GetVersion());

  // Create an IOSettings object. This object holds all import/export settings.
  FbxIOSettings* ios = FbxIOSettings::Create(_fbxMgr, IOSROOT);
  _fbxMgr->SetIOSettings(ios);

  // DISABLE
  // // Load plugins from the executable directory (optional)
  // FbxString path = FbxGetApplicationDirectory();
  // _fbxMgr->LoadPluginsDirectory(path.Buffer());

  // Create an FBX scene. This object holds most objects imported/exported from/to files.
  _fbxScene = FbxScene::Create(_fbxMgr, "My Scene");
  if (!_fbxScene) {
    printf("Error: Unable to create FBX scene!\n");
    exit(1);
  }
}

void deinit_fbx_sdk (FbxManager *&_fbxMgr) {
  if ( _fbxMgr ) {
    _fbxMgr->Destroy();
  }
}

bool save_scene (FbxManager *_fbxMgr, FbxDocument *_fbxScene, const char *_filename) {
  int major, minor, patch;
  bool status;

  // create an exporter
  FbxExporter *exporter = FbxExporter::Create(_fbxMgr, "");

  // Set the import states. By default, the import states are always set to
  // true. The code below shows how to change these states.
  IOS_REF.SetBoolProp(EXP_FBX_MATERIAL,        true);
  IOS_REF.SetBoolProp(EXP_FBX_TEXTURE,         true);
  IOS_REF.SetBoolProp(EXP_FBX_EMBEDDED,        false);
  IOS_REF.SetBoolProp(EXP_FBX_SHAPE,           true);
  IOS_REF.SetBoolProp(EXP_FBX_GOBO,            true);
  IOS_REF.SetBoolProp(EXP_FBX_ANIMATION,       true);
  IOS_REF.SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

  // Initialize the exporter by providing a filename.
  status = exporter->Initialize(_filename, -1, _fbxMgr->GetIOSettings());

  if (!status) {
    FbxString error = exporter->GetStatus().GetErrorString();
    printf("Call to FbxExporter::Initialize() failed.\n");
    printf("Error returned: %s\n\n", error.Buffer());

    return false;
  }

  FbxManager::GetFileFormatVersion(major, minor, patch);
  printf("FBX file format version %d.%d.%d\n\n", major, minor, patch);

  //
  status = exporter->Export(_fbxScene);

  // Destroy exporter
  exporter->Destroy();

  return status;
}
