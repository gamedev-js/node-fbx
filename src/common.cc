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

bool load_scene (FbxManager *_fbxMgr, FbxDocument *_fbxScene, const char *_filename) {
  int major, minor, patch;
  int sdkMajor, sdkMinor, sdkPatch;
  bool status;

  // get the file version number generate by the FBX SDK
  FbxManager::GetFileFormatVersion(sdkMajor, sdkMinor, sdkPatch);

  // create an importer
  FbxImporter *importer = FbxImporter::Create(_fbxMgr, "");

  // Initialize the importer by providing a filename.
  status = importer->Initialize(_filename, -1, _fbxMgr->GetIOSettings());
  importer->GetFileVersion(major, minor, patch);

  if (!status) {
    FbxString error = importer->GetStatus().GetErrorString();
    printf("Call to FbxImporter::Initialize() failed.\n");
    printf("Error returned: %s\n\n", error.Buffer());

    if (importer->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion) {
      printf("FBX file format version for this FBX SDK is %d.%d.%d\n", sdkMajor, sdkMinor, sdkPatch);
      printf("FBX file format version for file '%s' is %d.%d.%d\n\n", _filename, major, minor, patch);
    }

    return false;
  }

  if (!importer->IsFBX()) {
    printf("Error: This is not a FBX file.");

    return false;
  }

  // Set the import states. By default, the import states are always set to
  // true. The code below shows how to change these states.
  IOS_REF.SetBoolProp(IMP_FBX_MATERIAL,        true);
  IOS_REF.SetBoolProp(IMP_FBX_TEXTURE,         true);
  IOS_REF.SetBoolProp(IMP_FBX_LINK,            true);
  IOS_REF.SetBoolProp(IMP_FBX_SHAPE,           true);
  IOS_REF.SetBoolProp(IMP_FBX_GOBO,            true);
  IOS_REF.SetBoolProp(IMP_FBX_ANIMATION,       true);
  IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

  //
  status = importer->Import(_fbxScene);

  // Destroy importer
  importer->Destroy();

  return status;
}
