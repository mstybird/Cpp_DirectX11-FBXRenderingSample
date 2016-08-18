#include "DxFBXLoader.h"

void FBX_LOADER::CFBXLoader::InitializeSdkObjects(FbxManager *& pManager, FbxScene *& pScene)
{
	//FBX�}�l�[�W��
	pManager = FbxManager::Create();
	if (!pManager)
	{
		FBXSDK_printf("Error: FBX�}�l�[�W���̏��������s\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

	//IO�ݒ�
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	//�v���O�C���f�B���N�g���̓ǂݍ���(�I�v�V����)
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer());

	//�V�[���̍쐬
	pScene = FbxScene::Create(pManager, "My Scene");
	if (!pScene)
	{
		FBXSDK_printf("Error: �V�[���̍쐬�Ɏ��s���܂���\n");
		exit(1);
	}
}
