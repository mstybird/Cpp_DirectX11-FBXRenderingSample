#include "DxFBXLoader.h"

void FBX_LOADER::CFBXLoader::InitializeSdkObjects(FbxManager *& pManager, FbxScene *& pScene)
{
	//FBXマネージャ
	pManager = FbxManager::Create();
	if (!pManager)
	{
		FBXSDK_printf("Error: FBXマネージャの初期化失敗\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

	//IO設定
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	//プラグインディレクトリの読み込み(オプション)
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer());

	//シーンの作成
	pScene = FbxScene::Create(pManager, "My Scene");
	if (!pScene)
	{
		FBXSDK_printf("Error: シーンの作成に失敗しました\n");
		exit(1);
	}
}
