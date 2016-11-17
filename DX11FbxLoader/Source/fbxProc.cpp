#include"DX11FbxLoader.h"
#include<Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#pragma warning(disable : 4800)

#define SAFE_RELEASE(x) if(x){(x)->Release(); (x)=NULL;}

void GetFolderPath(std::string & pDstString, const std::string & pFilePath)
{

	size_t index = pFilePath.find_last_of('\\');
	if (index == std::string::npos) {
		index = pFilePath.find_last_of('/');
	}
	if (index != std::string::npos) {
		pDstString = pFilePath.substr(0, index);

	}
}

FbxAMatrix GetPoseMatrix(FbxPose * pPose, int pNodeIndex)
{
	FbxAMatrix lPoseMatrix;
	FbxMatrix lMatrix;

	lPoseMatrix;
	lMatrix = pPose->GetMatrix(pNodeIndex);
	memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

	return lPoseMatrix;
}

void GetGeometry(FbxAMatrix&pSrcMatrix, FbxNode * pNode)
{
	//const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	//const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	//const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	pSrcMatrix.SetTRS(
		pNode->GetGeometricTranslation(FbxNode::eSourcePivot),
		pNode->GetGeometricRotation(FbxNode::eSourcePivot),
		pNode->GetGeometricScaling(FbxNode::eSourcePivot)

	);

	//	return FbxAMatrix(lT, lR, lS);
}




void GetGlobalPosition(FbxAMatrix&pDstMatrix, FbxNode*pNode, const FbxTime&pTime, FbxPose*pPose = nullptr, FbxAMatrix*pParentGlobalPosition = (fbxsdk::FbxAMatrix*)0)
{
	bool lPositionFound = false;

	if (pPose) {
		int lNodeIndex = pPose->Find(pNode);

		if (lNodeIndex > -1) {
			if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex)) {
				pDstMatrix = GetPoseMatrix(pPose, lNodeIndex);
			}
			else {
				//���[���h���W�n�ɕϊ�����
				FbxAMatrix lParentGlobalPosition;
				if (pParentGlobalPosition) {
					//lParentGlobalPosition = *pParentGlobalPosition;
					memcpy(&lParentGlobalPosition, pParentGlobalPosition, sizeof(FbxAMatrix));
				}
				else {
					if (pNode->GetParent()) {
						GetGlobalPosition(lParentGlobalPosition, pNode->GetParent(), pTime, pPose);
					}
				}

				FbxAMatrix lLocalPosition = GetPoseMatrix(pPose, lNodeIndex);
				pDstMatrix = lParentGlobalPosition*lLocalPosition;

			}
			lPositionFound = true;
		}

	}

	if (!lPositionFound) {
		pDstMatrix = pNode->EvaluateGlobalTransform(pTime);
	}
}



DX11FbxLoader::DX11FbxLoader() :
	mMesh{ std::make_shared<std::vector<std::shared_ptr<FBXMesh>>>() }
{
}

DX11FbxLoader::~DX11FbxLoader()
{
	FbxDestroy();
}

//����������
void DX11FbxLoader::FbxInit(std::string vfileName, bool animationLoad) {
	fileName = vfileName;
	GetFolderPath(fileRelativePath, fileName);
	PoseIndex = -1;

	Cache_Start = FBXSDK_TIME_INFINITE;
	Cache_Stop = FBXSDK_TIME_MINUS_INFINITE;

	//SDK�̏�����
	SdkManager = FbxManager::Create();

	FBXSDK_printf("Autodesk FBX SDK version %s\n", SdkManager->GetVersion());

	//IO�ݒ�
	auto ios = FbxIOSettings::Create(SdkManager, IOSROOT);
	SdkManager->SetIOSettings(ios);

	//�v���O�C���̓ǂݍ���
	FbxString lPath = FbxGetApplicationDirectory();
	SdkManager->LoadPluginsDirectory(lPath.Buffer());
	FBXSDK_printf("Loaded Plugins Directory is \" %s \"\n", lPath.Buffer());

	//�V�[���̍쐬
	Scene = FbxScene::Create(SdkManager, "MyScene");

	if (!SdkManager) {
		FBXSDK_printf("FBX Initialize Error\n");
	}

	//�t�@�C���̃C���|�[�g
	auto FileFormat = -1;
	Importer = FbxImporter::Create(SdkManager, "");
	if (!SdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(fileName.c_str(), FileFormat)) {
		//�o�C�i���t�@�C���������ꍇ�A�o�C�i���f�[�^��ǂݍ��ނ��߂̐ݒ�
		FileFormat = SdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");;
	}


	//�C���|�[�^�̏��������t�@�C���̐ݒ�
	if (Importer->Initialize(fileName.c_str(), FileFormat) == true) {
		WindowMessage = "Importing file ";
		WindowMessage += fileName.c_str();
		WindowMessage += "\nPlease wait!";

	}
	else
	{
		WindowMessage = "Unable to open file ";
		WindowMessage += fileName.c_str();
		WindowMessage += "\nError reported: ";
		WindowMessage += Importer->GetStatus().GetErrorString();
		WindowMessage += "\nEsc to exit";
	}
	FBXSDK_printf("%s\n", WindowMessage.Buffer());

	//�t�@�C���̃C���|�[�g
	FbxLoadFromFile();
	//�A�j���[�V�����̏�����
	SceneAnim = Scene;
	LoadAnimationData();
	EnableAnimation = animationLoad;
}

void DX11FbxLoader::FbxLoadAnimationFromFile(std::string vfileName)
{
	//�V�[���̍쐬(�A�j���[�V�����I�����[)
	SceneAnim = FbxScene::Create(SdkManager, "Animation");

	if (!SdkManager) {
		FBXSDK_printf("FBX Initialize Error\n");
	}

	//�t�@�C���̃C���|�[�g
	auto FileFormat = -1;
	Importer = FbxImporter::Create(SdkManager, "");
	if (!SdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(fileName.c_str(), FileFormat)) {
		//�o�C�i���t�@�C���������ꍇ�A�o�C�i���f�[�^��ǂݍ��ނ��߂̐ݒ�
		FileFormat = SdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");;
	}


	//�C���|�[�^�̏��������t�@�C���̐ݒ�
	if (Importer->Initialize(fileName.c_str(), FileFormat) == true) {
		WindowMessage = "Importing file ";
		WindowMessage += fileName.c_str();
		WindowMessage += "\nPlease wait!";

	}
	else
	{
		WindowMessage = "Unable to open file ";
		WindowMessage += fileName.c_str();
		WindowMessage += "\nError reported: ";
		WindowMessage += Importer->GetStatus().GetErrorString();
		WindowMessage += "\nEsc to exit";
	}
	FBXSDK_printf("%s\n", WindowMessage.Buffer());
	SceneAnim->FillAnimStackNameArray(AnimStackNameArray);

	LoadAnimationData();
}



//�������
void DX11FbxLoader::FbxDestroy() {
	//�W�I���g���̉��
	ReleaseGeometryData();
	//�A�j���[�V�������X�g������̉��
	FbxArrayDelete(AnimStackNameArray);
	//�V�[����ǂݍ���ł����ꍇ�V�[�����J������
	if (Scene) {
		UnLoadCacheRecursive(Scene);
		Scene = nullptr;
	}
	if (SdkManager)SdkManager->Destroy();
	SdkManager = nullptr;
}

void DX11FbxLoader::FbxLoadFromFile()
{

	if (Importer->Import(Scene) == true) {
		//���W�n�̕ϊ�
		FbxAxisSystem SceneAxisSystem = Scene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem OurAxisSystem(
			FbxAxisSystem::EUpVector::eYAxis,
			FbxAxisSystem::EFrontVector::eParityEven,
			FbxAxisSystem::ECoordSystem::eLeftHanded);


		FbxAxisSystem::OpenGL.ConvertScene(Scene);

		//�ǂݍ��񂾃t�@�C���ƍ��W�n���قȂ�ꍇ�̓V�[���̍��W�n��ϊ�
		if (SceneAxisSystem != OurAxisSystem) {
		}

		Scene->FillAnimStackNameArray(AnimStackNameArray);

		//�|���S����S�ĎO�p�`�ɕϊ�
		FbxGeometryConverter geometryConverter(SdkManager);
		//true�ɂ���ƒ��ڏ���������
		geometryConverter.Triangulate(Scene, true);

		//�ŏ��̃t���[���̃V�[�����擾����
		this->LoadCacheRecursive(Scene, CurrentAnimLayer, fileName);

		//�f�t�H�[�}�[����
		this->PreparePointCacheData(Scene, Cache_Start, Cache_Stop);

		//�|�[�Y�̃��X�g���쐬����
		this->FillPoseArray(Scene, PoseArray);

		Scene->GetGlobalSettings().SetTimeMode(FbxTime::EMode::eFrames60);
		FrameTime.SetTime(0, 0, 0, 1, 0, Scene->GetGlobalSettings().GetTimeMode());

		//�C���|�[�^�͂���ȏ�s�v�Ȃ̂ŉ������
		Importer->Destroy();
		Importer = nullptr;
	}
}

//�m�[�h�̑����ƃ}�e���A������ǂݎ��A�V�[���ɕK�v�ȃe�N�X�`���t�@�C�������擾����
void DX11FbxLoader::LoadCacheRecursive(FbxScene * pScene, FbxAnimLayer * pAnimLayer, std::string& pFbxFileName)
{
	//�e�N�X�`�������擾
	const auto lTextureCount = Scene->GetTextureCount();
	FbxString filePath;

	//this->TextureFileName.clear();
	std::vector<FbxTexture::EMappingType> m;

	for (auto lTextureIndex = 0; lTextureIndex < lTextureCount; lTextureIndex++) {
		//�e�N�X�`���擾
		auto lTexture = Scene->GetTexture(lTextureIndex);

		auto lFileTexture = FbxCast<FbxFileTexture>(lTexture);

		m.push_back(lFileTexture->GetMappingType());

		if (lFileTexture && !lFileTexture->GetUserDataPtr()) {
			//FBX�t�@�C���p�X���擾
			const FbxString lFileName = lFileTexture->GetFileName();
			//printf("%s\n", lFileName.Buffer());
			bool lStatus = (bool)PathFileExists(lFileName.Buffer());
			if (lStatus)filePath = lFileName;

			const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(pFbxFileName.c_str());
			const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);

			//�ǂݍ��߂Ȃ������ꍇ
			if (!lStatus) {
				//���΃p�X�œǂݍ���
				const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
				//printf("%s\n", lResolvedFileName.Buffer());
				lStatus = (bool)PathFileExists(lResolvedFileName.Buffer());
				if (lStatus)filePath = lResolvedFileName;
			}


			//�ǂݍ��߂Ȃ������ꍇ
			if (!lStatus) {
				//�J�����g�f�B���N�g���ɂ���Γǂݍ���
				// Load texture from file name only (relative to FBX file)
				const FbxString lTextureFileName = FbxPathUtils::GetFileName(lFileName);
				const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lTextureFileName);
				lStatus = (bool)PathFileExistsA(lResolvedFileName.Buffer());
				//printf("%s\n", lResolvedFileName.Buffer());
				if (lStatus)filePath = lResolvedFileName;

			}



			//�ǂ��̃e�N�X�`���Ȃ̂��L������
			const FbxString uvName = lFileTexture->UVSet.Get();
			//���UV�ɕ����̃e�N�X�`�������݂���\��������
			//this->TextureFileName[uvName.Buffer()].push_back(filePath.Buffer());

			//�ǂݍ��߂���t�@�C���e�N�X�`���̃A�h���X��n��
			if (lStatus) {
				std::string*lTextureName = new std::string(filePath);
				lFileTexture->SetUserDataPtr(lTextureName);
			}

		}
	}

	//�q�m�[�h��T��
	LoadCacheRecursive(Scene->GetRootNode(), pAnimLayer);

}
//�������烂�f���̃��b�V�����Ƃ��B�}�e���A�����擾����
void DX11FbxLoader::LoadCacheRecursive(FbxNode * pNode, FbxAnimLayer * pAnimLayer)
{
	//��̃m�[�h�ɑ��݂���}�e���A���̐����擾
	const auto lMaterialCount = pNode->GetMaterialCount();
	//�}�e���A���������[�v
	for (auto lMaterialIndex = 0; lMaterialIndex < lMaterialCount; lMaterialIndex++) {
		//�}�e���A�����擾
		auto lMaterial = pNode->GetMaterial(lMaterialIndex);

		if (lMaterial && !lMaterial->GetUserDataPtr()) {
			//�}�e���A���L���b�V���쐬
			FbxAutoPtr<FbxMaterialCache> lMaterialCache(new FbxMaterialCache);
			//�}�e���A���L���b�V���̏�����
			if (lMaterialCache->Initialize(lMaterial, fileRelativePath)) {
				//�L���b�V���̃|�C���^���}�e���A���ɓn��
				lMaterial->SetUserDataPtr(lMaterialCache.Release());
			}
		}
	}

	//�m�[�h�̑������擾
	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
	if (lNodeAttribute) {
		//�m�[�h�����b�V����\���Ă��邩�ǂ���
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
			//�m�[�h��o�^
			nodemeshes.push_back(pNode);
			//���b�V�����擾
			FbxMesh*lMesh = pNode->GetMesh();
			if (lMesh && !lMesh->GetUserDataPtr()) {
				FbxAutoPtr<FbxVBO>lMeshCache(new FbxVBO);
				if (lMeshCache->Initialize(lMesh)) {
					//VBO�N���X��o�^
					lMesh->SetUserDataPtr(lMeshCache.Release());
				}
			}
		}
	}


	//�q�m�[�h�̐��𐔂���
	const auto lChildCount = pNode->GetChildCount(); {
		for (auto lChildIndex = 0; lChildIndex < lChildCount; lChildIndex++) {
			LoadCacheRecursive(pNode->GetChild(lChildIndex), pAnimLayer);
		}
	}

}

void DX11FbxLoader::UnLoadCacheRecursive(FbxScene * pScene)
{
	const int lTextureCount = pScene->GetTextureCount();
	for (int lTextureIndex = 0; lTextureIndex < lTextureCount; lTextureIndex++) {
		FbxTexture * lTexture = pScene->GetTexture(lTextureIndex);
		FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>(lTexture);
		if (lFileTexture && lFileTexture->GetUserDataPtr())
		{
			FbxString * lTextureName = static_cast<FbxString *>(lFileTexture->GetUserDataPtr());
			lFileTexture->SetUserDataPtr(NULL);
			//�e�N�X�`���C���[�W�̉��
			//glDeleteTextures(1, lTextureName);
			delete lTextureName;
		}
	}
	UnLoadCacheRecursive(pScene->GetRootNode());
}

/*
�m�[�h�̑S�{�����Ȃ������߂ɁA
eNode���������m�[�h�݂̂̔z����쐬����B
�ȍ~�͂��̃m�[�h�ɂ̂݃A�N�Z�X����
*/


void DX11FbxLoader::UnLoadCacheRecursive(FbxNode * pNode)
{
	//�}�e���A���̉��
	const int lMaterialCount = pNode->GetMaterialCount();
	for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex) {
		FbxSurfaceMaterial*lMaterial = pNode->GetMaterial(lMaterialIndex);
		if (lMaterial&&lMaterial->GetUserDataPtr()) {
			FbxMaterialCache*lMaterialCache = static_cast<FbxMaterialCache*>(lMaterial->GetUserDataPtr());
			lMaterial->SetUserDataPtr(nullptr);
			delete lMaterialCache;
		}
	}
	//���b�V�����������
	FbxNodeAttribute*lNodeAttribute = pNode->GetNodeAttribute();
	if (lNodeAttribute) {
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {

			FbxMesh*lMesh = pNode->GetMesh();
			if (lMesh&&lMesh->GetUserDataPtr()) {
				FbxVBO*lMeshCache = static_cast<FbxVBO*>(lMesh->GetUserDataPtr());
				lMesh->SetUserDataPtr(nullptr);
				delete lMeshCache;
			}

		}
	}

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex) {
		UnLoadCacheRecursive(pNode->GetChild(lChildIndex));
	}

}

void DX11FbxLoader::PreparePointCacheData(FbxScene * pScene, FbxTime & pCache_Start, FbxTime & pCache_Stop)
{
	//�m�[�h�����擾
	const auto lNodeCount = pScene->GetSrcObjectCount<FbxNode>();
	FbxStatus lStatus;
	for (auto lIndex = 0; lIndex<lNodeCount; lIndex++) {
		FbxNode* lNode = pScene->GetSrcObject<FbxNode>(lIndex);
		if (lNode->GetGeometry()) {
			int i;
			int lVertexCacheDeformerCount = lNode->GetGeometry()->GetDeformerCount(FbxDeformer::eVertexCache);
			lVertexCacheDeformerCount = lVertexCacheDeformerCount > 0 ? 1 : 0;
			for (i = 0; i < lVertexCacheDeformerCount; i++) {
				//���_�L���b�V��
				FbxVertexCacheDeformer* lDeformer = static_cast<FbxVertexCacheDeformer*>(lNode->GetGeometry()->GetDeformer(i, FbxDeformer::eVertexCache));
				if (!lDeformer)continue;
				FbxCache*lCache = lDeformer->GetCache();

				if (!lCache)continue;

				if (lDeformer->Active.Get()) {
					if (lCache->GetCacheFileFormat() == FbxCache::eMaxPointCacheV2) {
						//�����Ȃ�
					}
					else if (lCache->GetCacheFileFormat() == FbxCache::eMayaCache) {
						//MC����PC2�t�H�[�}�b�g�ɕϊ�����
						if (!lCache->ConvertFromMCToPC2(FbxTime::GetFrameRate(pScene->GetGlobalSettings().GetTimeMode()), 0, &lStatus)) {
							//���s������
							FbxString lTheErrorIs = lStatus.GetErrorString();
						}
					}


					//�L���b�V����ǂݎ��
					if (!lCache->OpenFileForRead(&lStatus)) {
						FbxString lTheErrorIs = lStatus.GetErrorString();
						lDeformer->Active = false;
					}
					else {
						FbxTime lChannel_Start;
						FbxTime lChannel_Stop;
						int lChannelIndex = lCache->GetChannelIndex(lDeformer->Channel.Get());
						if (lCache->GetAnimationRange(lChannelIndex, lChannel_Start, lChannel_Stop)) {
							// ���C���[�V�����J�n�t���[���ݒ�
							if (lChannel_Start < pCache_Start) pCache_Start = lChannel_Start;

							// �A�j���[�V�����I���t���[���ݒ�
							if (lChannel_Stop > pCache_Stop)  pCache_Stop = lChannel_Stop;


						}
					}

				}

			}
		}
	}
}

void DX11FbxLoader::FillPoseArray(FbxScene * pScene, FbxArray<FbxPose*> pPoseArray)
{
	const auto lPoseCount = pScene->GetPoseCount();
	for (auto i = 0; i < lPoseCount; i++) {
		pPoseArray.Add(pScene->GetPose(i));
	}
}

//�A�j���[�V�����̓ǂݍ���
void DX11FbxLoader::LoadAnimationData()
{
	int lCurrentAnimStackIndex = -1;	//���݂̃A�j���[�V����
	const FbxArray<FbxString*>&lAnimStackNameArray = GetAnimStackNameArray();
	auto lPoseCount = lAnimStackNameArray.GetCount();
	for (int lPoseIndex = 0; lPoseIndex < lPoseCount; lPoseIndex++) {
		AnimData[lAnimStackNameArray[lPoseIndex]->Buffer()] = lPoseIndex;
		//���݂̃A�j���[�V�����X�^�b�N�����ݓK�p����Ă���A�j���[�V�������ǂ���
		if (lAnimStackNameArray[lPoseIndex]->Compare(SceneAnim->ActiveAnimStackName.Get()) == 0) {
			//�K�p����Ă���Ȃ炻�̃A�j���[�V���������݂̃A�j���[�V�����Ƃ���
			lCurrentAnimStackIndex = lPoseIndex;
		}
	}
	//�ݒ肳�ꂽ�A�j���[�V�������Z�b�g����
	//	EnableAnimation = lCurrentAnimStackIndex != -1 ? true : false;
	SetCurrentAnimStack(lCurrentAnimStackIndex);
}

void DX11FbxLoader::ReadVertexCacheData(FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray)
{
	FbxVertexCacheDeformer*lDeformer = static_cast<FbxVertexCacheDeformer*>(pMesh->GetDeformer(0, FbxDeformer::eVertexCache));
	FbxCache* lCache = lDeformer->GetCache();
	int lChannelIndex = lCache->GetChannelIndex(lDeformer->Channel.Get());
	unsigned int lVertexCount = (unsigned int)pMesh->GetControlPointsCount();
	bool lReadSucceed = false;
	float* lReadBuf = nullptr;
	unsigned int BufferSize = 0;

	if (lDeformer->Type.Get() != FbxVertexCacheDeformer::ePositions) {
		return;
	}

	unsigned int Length = 0;
	lCache->Read(NULL, Length, FBXSDK_TIME_ZERO, lChannelIndex);
	if (Length != lVertexCount * 3) {
		return;
	}
	lReadSucceed = lCache->Read(&lReadBuf, BufferSize, pTime, lChannelIndex);

	//�ǂݍ��݂ɐ��������ꍇ
	if (lReadSucceed) {
		unsigned int lReadBufIndex = 0;

		while (lReadBufIndex < 3 * lVertexCount) {
			pVertexArray[lReadBufIndex / 3].mData[0] = lReadBuf[lReadBufIndex];
			++lReadBufIndex;
			pVertexArray[lReadBufIndex / 3].mData[1] = lReadBuf[lReadBufIndex];
			++lReadBufIndex;
			pVertexArray[lReadBufIndex / 3].mData[2] = lReadBuf[lReadBufIndex];
			++lReadBufIndex;
		}
	}


}

void DX11FbxLoader::ComputeShapeDeformation(const int pIndex, FbxMesh * pMesh, FbxTime & pTime, FbxAnimLayer * pAnimLayer, FbxVector4 * pVertexArray)
{
	int lVertexCount = pMesh->GetControlPointsCount();

	FbxVector4*lSrcVertexArray = pVertexArray;

	if (mCache.dstVertexArray[pIndex] == nullptr) {
		mCache.dstVertexArray[pIndex] = new FbxVector4[lVertexCount];
	}

	memcpy(mCache.dstVertexArray[pIndex], pVertexArray, lVertexCount * sizeof(FbxVector4));

	int lBlendShapeDeformerCount = pMesh->GetDeformerCount(FbxDeformer::eBlendShape);
	for (int lBlendShapeIndex = 0; lBlendShapeIndex < lBlendShapeDeformerCount; ++lBlendShapeIndex) {
		FbxBlendShape*lBlendShape = (FbxBlendShape*)pMesh->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);

		int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
		for (int lChannelIndex = 0; lChannelIndex < lBlendShapeChannelCount; ++lChannelIndex) {
			FbxBlendShapeChannel*lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
			if (lChannel) {
				FbxAnimCurve*lFCurve = pMesh->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer);

				if (!lFCurve)continue;
				double lWeight = lFCurve->Evaluate(pTime);

				int lShapeCount = lChannel->GetTargetShapeCount();
				double*lFullWeights = lChannel->GetTargetShapeFullWeights();

				int lStartIndex = -1;
				int lEndIndex = -1;
				for (int lShapeIndex = 0; lShapeIndex < lShapeCount; ++lShapeIndex) {
					if (lWeight > 0 && lWeight <= lFullWeights[0]) {
						lEndIndex = 0;
						break;
					}
					if (lWeight > lFullWeights[lShapeIndex] && lWeight < lFullWeights[lShapeIndex + 1]) {
						lStartIndex = lShapeIndex;
						lEndIndex = lShapeIndex + 1;
						break;
					}
				}

				FbxShape*lStartShape = nullptr;
				FbxShape*lEndShape = nullptr;

				if (lStartIndex > -1) {
					lStartShape = lChannel->GetTargetShape(lStartIndex);
				}
				if (lEndIndex > -1) {
					lEndShape = lChannel->GetTargetShape(lEndIndex);
				}
				if (lStartIndex == -1 && lEndShape) {
					double lEndWeight = lFullWeights[0];
					//�E�F�C�g�̌v�Z
					lWeight = (lWeight / lEndWeight) * 100;
					//�o�͒��_�f�[�^��������
					memcpy(mCache.dstVertexArray[pIndex], lSrcVertexArray, lVertexCount * sizeof(FbxVector4));
					for (int j = 0; j < lVertexCount; ++j) {
						FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lSrcVertexArray[j])*lWeight*0.01;
						mCache.dstVertexArray[pIndex][j] += lInfluence;
					}
				}
				else if (lStartShape&&lEndShape) {
					double lStartWeight = lFullWeights[lStartIndex];
					double lEndWeight = lFullWeights[lEndIndex];
					//�E�F�C�g�̌v�Z
					lWeight = (lWeight / lEndWeight) * 100;
					//�o�͒��_�f�[�^��������
					memcpy(mCache.dstVertexArray[pIndex], lStartShape->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

					for (int j = 0; j < lVertexCount; ++j) {
						FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lStartShape->GetControlPoints()[j])*lWeight*0.01;
						mCache.dstVertexArray[pIndex][j] += lInfluence;
					}
				}
			}
		}
	}
	memcpy(pVertexArray, mCache.dstVertexArray[pIndex], lVertexCount * sizeof(FbxVector4));
}

void DX11FbxLoader::ComputeSkinDeformation(const int pIndex, FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose)
{
	FbxSkin*lSkinDeformer = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);
	FbxSkin::EType lSkinningType = lSkinDeformer->GetSkinningType();

	if (lSkinningType == FbxSkin::eLinear || lSkinningType == FbxSkin::eRigid) {
		ComputeLinearDeformation(pIndex, pGlobalPosition, pMesh, pTime, pVertexArray, pPose);
	}
	else if (lSkinningType == FbxSkin::eDualQuaternion) {
		ComputeDualQuaternionDeformation(pIndex, pGlobalPosition, pMesh, pTime, pVertexArray, pPose);
	}
	else if (lSkinningType == FbxSkin::eBlend) {
		int lVertexCount = pMesh->GetControlPointsCount();


		if (mCache.vertexArrayLinear[pIndex] == nullptr) {
			mCache.vertexArrayLinear[pIndex] = new FbxVector4[lVertexCount];

		}

		if (mCache.vertexArrayDQ[pIndex] == nullptr) {
			mCache.vertexArrayDQ[pIndex] = new FbxVector4[lVertexCount];

		}


		memcpy(mCache.vertexArrayLinear[pIndex], pMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
		memcpy(mCache.vertexArrayDQ[pIndex], pMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

		ComputeLinearDeformation(pIndex, pGlobalPosition, pMesh, pTime, mCache.vertexArrayDQ[pIndex], pPose);
		ComputeDualQuaternionDeformation(pIndex, pGlobalPosition, pMesh, pTime, mCache.vertexArrayDQ[pIndex], pPose);

		//2�̃X�L�j���O�̃E�F�C�g����������
		int lBlendWeightsCount = lSkinDeformer->GetControlPointIndicesCount();
		for (int lBWIndex = 0; lBWIndex < lBlendWeightsCount; ++lBWIndex) {
			double lBlendWeight = lSkinDeformer->GetControlPointBlendWeights()[lBWIndex];
			pVertexArray[lBWIndex] = mCache.vertexArrayDQ[pIndex][lBWIndex] * lBlendWeight + mCache.vertexArrayLinear[pIndex][lBWIndex] * (1 - lBlendWeight);
		}
	}
}
void DX11FbxLoader::ComputeLinearDeformation(const int pIndex, FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose)
{
	FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();
	int lVertexCount = pMesh->GetControlPointsCount();

	if (mCache.clusterDeformation[pIndex] == nullptr) {
		mCache.clusterDeformation[pIndex] = new FbxAMatrix[lVertexCount];
	}

	memset(mCache.clusterDeformation[pIndex], 0, lVertexCount * sizeof(FbxAMatrix));

	if (mCache.clusterLinearWeight[pIndex] == nullptr) {

		mCache.clusterLinearWeight[pIndex] = new double[lVertexCount];
	}

	memset(mCache.clusterLinearWeight[pIndex], 0, lVertexCount * sizeof(double));
	if (lClusterMode == FbxCluster::eAdditive) {
		for (int i = 0; i < lVertexCount; ++i) {
			mCache.clusterDeformation[pIndex][i].SetIdentity();
		}
	}

	int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);
	FbxAMatrix lInfluence;
	FbxAMatrix lVertexTransformMatrix;
	for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex) {

		FbxSkin*lSkinDeformer = (FbxSkin*)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);
		int lClusterCount = lSkinDeformer->GetClusterCount();
		for (int lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex) {


			FbxCluster*lCluster = lSkinDeformer->GetCluster(lClusterIndex);
			if (!lCluster->GetLink()) {
				continue;
			}


			ComputeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime, pPose);


			int lVertexIndexCount = lCluster->GetControlPointIndicesCount();


			for (int k = 0; k < lVertexIndexCount; ++k) {
				int lIndex = lCluster->GetControlPointIndices()[k];

				//�����ȃE�F�C�g�𖳎����Đ��K������
				if (lIndex >= lVertexCount)continue;

				double lWeight = lCluster->GetControlPointWeights()[k];

				if (lWeight == 0.0) {
					continue;
				}

				//				lInfluence = lVertexTransformMatrix;



				memcpy(&lInfluence, &lVertexTransformMatrix, sizeof(FbxAMatrix));

				FbxComputeDeformer::MatrixScale(lInfluence.Buffer()->Buffer(), lWeight);


				if (lClusterMode == FbxCluster::eAdditive) {
					//�Ńt�H�[�}�[���s��ɍ�������
					FbxComputeDeformer::MatrixAddToDiagonal(lInfluence, 1.0 - lWeight);
					mCache.clusterDeformation[pIndex][lIndex] = lInfluence * mCache.clusterDeformation[pIndex][lIndex];
					//�N���X�^�̃E�F�C�g��1.0�ɂ���
					mCache.clusterLinearWeight[pIndex][lIndex] = 1.0;
				}
				else {
					FbxComputeDeformer::MatrixAdd(mCache.clusterDeformation[pIndex][lIndex].Buffer()->Buffer(), lInfluence);
					//���K�������W�I���g�����R���s���[�g�W�I���g���ɃE�F�C�g�����Z����
					mCache.clusterLinearWeight[pIndex][lIndex] += lWeight;
				}
			}

		}

	}

	//���_�R�s�[
	for (int i = 0; i < lVertexCount; ++i)
	{
		//�R�s�[�����_
		FbxVector4 lSrcVertex = pVertexArray[i];
		//�E�F�C�g�̌v�Z���ʂ��܂ޒ��_
		FbxVector4& lDstVertex = pVertexArray[i];
		//�E�F�C�g�̎擾
		double lWeight = mCache.clusterLinearWeight[pIndex][i];

		// Deform the vertex if there was at least a link with an influence on the vertex,
		//�E�F�C�g��0�łȂ��ꍇ�̂݌v�Z
		if (lWeight != 0.0)
		{
			//�x�N�g���ɍs��������������̂��擾����
			lDstVertex = mCache.clusterDeformation[pIndex][i].MultT(lSrcVertex);
			//���K������ꍇ�A�E�F�C�g�Ŋ���
			if (lClusterMode == FbxCluster::eNormalize)
			{
				// In the normalized link mode, a vertex is always totally influenced by the links. 
				lDstVertex /= lWeight;
			}
			//
			else if (lClusterMode == FbxCluster::eTotalOne)
			{
				// In the total 1 link mode, a vertex can be partially influenced by the links. 
				lSrcVertex *= (1.0 - lWeight);
				lDstVertex += lSrcVertex;
			}
		}
	}

}

void DX11FbxLoader::ComputeDualQuaternionDeformation(const int pIndex, FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose)
{
	FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

	int lVertexCount = pMesh->GetControlPointsCount();
	int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);

	if (mCache.dQClusterDeformation[pIndex] == nullptr) {
		mCache.dQClusterDeformation[pIndex] = new FbxDualQuaternion[lVertexCount];
	}
	if (mCache.clusterDualWeight[pIndex] == nullptr) {
		mCache.clusterDualWeight[pIndex] = new double[lVertexCount];
	}

	memset(mCache.dQClusterDeformation[pIndex], 0, lVertexCount * sizeof(FbxDualQuaternion));
	memset(mCache.clusterDualWeight[pIndex], 0, lVertexCount * sizeof(double));

	//�S�ẴN���X�^
	for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
	{
		//�f�t�H�[�}�[���擾
		FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);
		//�N���X�^�����擾
		int lClusterCount = lSkinDeformer->GetClusterCount();
		for (int lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex)
		{
			//�N���X�^���擾
			FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
			//�N���X�^�Ƀ����N���Z�b�g����Ă��Ȃ���Ώ������Ȃ�
			if (!lCluster->GetLink())
				continue;

			//���_�̈ړ��s��
			FbxAMatrix lVertexTransformMatrix;
			ComputeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime, pPose);

			//�N�H�[�^�j�I���擾
			FbxQuaternion lQ = lVertexTransformMatrix.GetQ();
			FbxVector4 lT = lVertexTransformMatrix.GetT();
			FbxDualQuaternion lDualQuaternion(lQ, lT);
			int lVertexIndexCount = lCluster->GetControlPointIndicesCount();
			for (int k = 0; k < lVertexIndexCount; ++k) {
				int lIndex = lCluster->GetControlPointIndices()[k];
				// Sometimes, the mesh can have less points than at the time of the skinning
				// because a smooth operator was active when skinning but has been deactivated during export.
				//���_�������傫�����l���擾�ł��Ă��܂����ꍇ�͖�����
				if (lIndex >= lVertexCount)
					continue;


				//�E�F�C�g�擾
				double lWeight = lCluster->GetControlPointWeights()[k];
				//�E�F�C�g��0�������ꍇ�͏����s�v
				if (lWeight == 0.0)
					continue;

				// Compute the influence of the link on the vertex.
				//�N�H�[�^�j�I���ƃE�F�C�g������
				FbxDualQuaternion lInfluence = lDualQuaternion * lWeight;
				if (lClusterMode == FbxCluster::eAdditive)
				{
					//�N�H�[�^�j�I�����Z�b�g
					mCache.dQClusterDeformation[pIndex][lIndex] = lInfluence;
					//�E�F�C�g��1.0�ɌŒ�
					mCache.clusterDualWeight[pIndex][lIndex] = 1.0;
				}
				else // lLinkMode == FbxCluster::eNormalize || lLinkMode == FbxCluster::eTotalOne
				{
					if (lClusterIndex == 0)
					{
						mCache.dQClusterDeformation[pIndex][lIndex] = lInfluence;
					}
					else
					{
						// Add to the sum of the deformations on the vertex.
						// Make sure the deformation is accumulated in the same rotation direction. 
						// Use dot product to judge the sign.
						//���_��ό`����l�����Z
						double lSign = mCache.dQClusterDeformation[pIndex][lIndex].GetFirstQuaternion().DotProduct(lDualQuaternion.GetFirstQuaternion());
						//�K�����������Z����悤�ɂ���
						if (lSign >= 0.0)
						{
							mCache.dQClusterDeformation[pIndex][lIndex] += lInfluence;
						}
						else
						{
							mCache.dQClusterDeformation[pIndex][lIndex] -= lInfluence;
						}
					}
					//�E�F�C�g�����Z
					mCache.clusterDualWeight[pIndex][lIndex] += lWeight;
				}

			}
		}
	}

	//���_������
	for (int i = 0; i < lVertexCount; ++i)
	{
		FbxVector4 lSrcVertex = pVertexArray[i];
		FbxVector4& lDstVertex = pVertexArray[i];
		double lWeightSum = mCache.clusterDualWeight[pIndex][i];

		// Deform the vertex if there was at least a link with an influence on the vertex,
		//�E�F�C�g��0�Ȃ疢����
		if (lWeightSum != 0.0)
		{
			mCache.dQClusterDeformation[pIndex][i].Normalize();
			lDstVertex = mCache.dQClusterDeformation[pIndex][i].Deform(lDstVertex);

			if (lClusterMode == FbxCluster::eNormalize)
			{
				// In the normalized link mode, a vertex is always totally influenced by the links. 
				//�Q�Ƃ������_�ɃE�F�C�g�Ŋ����Đ��K������
				lDstVertex /= lWeightSum;
			}
			else if (lClusterMode == FbxCluster::eTotalOne)
			{
				// In the total 1 link mode, a vertex can be partially influenced by the links. 
				//�e������Ƃ��낾������
				lSrcVertex *= (1.0 - lWeightSum);
				lDstVertex += lSrcVertex;
			}
		}
	}
}

void DX11FbxLoader::ComputeClusterDeformation(FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxCluster * pCluster, FbxAMatrix & pVertexTransformMatrix, FbxTime pTime, FbxPose * pPose)
{
	FbxCluster::ELinkMode lClusterMode = pCluster->GetLinkMode();
	FbxAMatrix lReferenceGlobalInitPosition;	//�����ʒu
	FbxAMatrix lReferenceGlobalCurrentPosition;	//���݈ʒu
	FbxAMatrix lAssociateGlobalInitPosition;	//�����ʒu
	FbxAMatrix lAssociateGlobalCurrentPosition;	//���݈ʒu
	FbxAMatrix lClusterGlobalInitPosition;		//�����ʒu
	FbxAMatrix lClusterGlobalCurrentPosition;	//���݈ʒu

	FbxAMatrix lReferenceGeometry;
	FbxAMatrix lAssociateGeometry;
	FbxAMatrix lClusterGeometry;

	FbxAMatrix lClusterRelativeInitPosition;
	FbxAMatrix lClusterRelativeCurrentPositionInverse;



	if (lClusterMode == FbxCluster::eAdditive&&pCluster->GetAssociateModel()) {
		pCluster->GetTransformAssociateModelMatrix(lAssociateGlobalInitPosition);

		//���f���̕ϊ�
		{
			GetGeometry(lAssociateGeometry, pCluster->GetAssociateModel());
			lAssociateGlobalInitPosition *= lAssociateGeometry;
			GetGlobalPosition(lAssociateGlobalCurrentPosition, pCluster->GetAssociateModel(), pTime, pPose);
		}

		//�ړ��s��̎擾
		{
			pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
			// Multiply lReferenceGlobalInitPosition by Geometric Transformation
			//�s��̍���
			GetGeometry(lReferenceGeometry, pMesh->GetNode());
			lReferenceGlobalInitPosition *= lReferenceGeometry;
		}
		//���݈ʒu���O���[�o���|�W�V�����Ƃ��Đݒ�
		//lReferenceGlobalCurrentPosition = pGlobalPosition;

		//�ړ��s��̎擾
		{
			pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
			// �s�񍇐�
			GetGeometry(lClusterGeometry, pCluster->GetLink());
			lClusterGlobalInitPosition *= lClusterGeometry;
			GetGlobalPosition(lClusterGlobalCurrentPosition, pCluster->GetLink(), pTime, pPose);
		}

		// Compute the shift of the link relative to the reference.
		//ModelM-1 * AssoM * AssoGX-1 * LinkGX * LinkM-1*ModelM
		//�ŏI�I�ȍs�񍇐�
		{
			pVertexTransformMatrix = lReferenceGlobalInitPosition.Inverse() * lAssociateGlobalInitPosition * lAssociateGlobalCurrentPosition.Inverse() *
				lClusterGlobalCurrentPosition * lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

		}


	}
	else {

		pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
		lReferenceGlobalCurrentPosition = pGlobalPosition;
		// Multiply lReferenceGlobalInitPosition by Geometric Transformation
		GetGeometry(lReferenceGeometry, pMesh->GetNode());
		lReferenceGlobalInitPosition *= lReferenceGeometry;

		// Get the link initial global position and the link current global position.
		pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
		GetGlobalPosition(lClusterGlobalCurrentPosition, pCluster->GetLink(), pTime, pPose);
		// Compute the initial position of the link relative to the reference.
		lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

		// Compute the current position of the link relative to the reference.
		lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition;

		// Compute the shift of the link relative to the reference.
		pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition;
	}


}

void DX11FbxLoader::SetAnimation(std::string pName)
{
	SetCurrentAnimStack(AnimData[pName]);
}

void DX11FbxLoader::SetAnimation(int pIndex)
{
	SetCurrentAnimStack(pIndex);
}


std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>> DX11FbxLoader::GetGeometryData2(FbxTime&aCurrentTime)
{
	//���b�V���f�[�^�̉��
	ReleaseGeometryData();

	FbxPose*lPose{ nullptr };
	if (PoseIndex != -1)
	{
		lPose = Scene->GetPose(PoseIndex);
	}
	FbxAMatrix lDummyGlobalPosition;



	//���b�V���̐������z��쐬
	mMesh = std::make_shared<std::vector<std::shared_ptr<FBXMesh>>>();
	mMesh->reserve(nodemeshes.size());
	std::shared_ptr<FBXMesh>lMeshData;


	mCache.FirstReserve((int)nodemeshes.size());
	for (unsigned int i = 0; i < nodemeshes.size(); ++i) {
		//���b�V���쐬
		lMeshData = std::make_shared<FBXMesh>();

		FbxNode* node = nodemeshes[i];
		FbxAMatrix lGlobalPosition;

		GetGlobalPosition(lGlobalPosition, node, aCurrentTime, lPose, &lDummyGlobalPosition);

		FbxAMatrix lGeometryOffset;
		FbxAMatrix lGlobalOffPosition;


		//�W�I���g���̃I�t�Z�b�g���擾
		GetGeometry(lGeometryOffset, node);
		lGlobalOffPosition = lGlobalPosition*lGeometryOffset;

		//���f���f�[�^�Ɋi�[����s����쐬
		lMeshData->mWorld = std::make_unique<D3DXMATRIX>();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				lMeshData->mWorld->m[i][j] = (float)lGlobalOffPosition.Buffer()->Buffer()[i * 4 + j];
			}
		}


		//�m�[�h�̃��b�V�����擾
		FbxMesh* lMesh = node->GetMesh();

		//���_�����擾
		const int lVertexCount = lMesh->GetControlPointsCount();

		//���_������Ȃ��ꍇ�͏������Ȃ�
		if (lVertexCount == 0) {
			continue;
		}


		//VBO�擾
		FbxVBO* lMeshCache = static_cast<FbxVBO*>(lMesh->GetUserDataPtr());
		//�{�[��������Ȃ�{�[�����擾
		const bool lHasVertexCache = lMesh->GetDeformerCount(FbxDeformer::eVertexCache) && (static_cast<FbxVertexCacheDeformer*>(lMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->Active.Get();
		const bool lHasShape = lMesh->GetShapeCount() > 0;
		const bool lHasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
		const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;


		//���_�o�b�t�@
		//FbxVector4* lVertexArray = nullptr;
		////�@���o�b�t�@
		//FbxVector4* lNormalArray = nullptr;
		////VBO���쐬 or �{�[�����Ȃ��ꍇ�A���̂܂܃R�s�[
		if (!lMeshCache || lHasDeformation) {

			if (mCache.vertexArray[i] == nullptr) {
				mCache.vertexArray[i] = new FbxVector4[lVertexCount];

			}
			//���_�o�b�t�@�̃R�s�[(�擾)
			memmove(mCache.vertexArray[i], lMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
		}

		//�A�j���[�V�����֌W�̏���
		//�f�t�H�[�}�[���������ꍇ�A�A�j���[�V��������

		if (lHasDeformation) {

			if (lHasVertexCache) {
				ReadVertexCacheData(lMesh, aCurrentTime, mCache.vertexArray[i]);
			}
			else {
				if (lHasShape)
				{
					// Deform the vertex array with the shapes.
					ComputeShapeDeformation(i, lMesh, aCurrentTime, CurrentAnimLayer, mCache.vertexArray[i]);
				}
				//we need to get the number of clusters
				const int lSkinCount = lMesh->GetDeformerCount(FbxDeformer::eSkin);
				int lClusterCount = 0;
				for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
				{
					lClusterCount += ((FbxSkin *)(lMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin)))->GetClusterCount();
				}
				if (lClusterCount)
				{
					/*
						�����o�O
					*/
					// Deform the vertex array with the skin deformer.
					ComputeSkinDeformation(i, lGlobalOffPosition, lMesh, aCurrentTime, mCache.vertexArray[i], lPose);

				}

			}



			if (lMeshCache)
				//�X�V�������_�̔��f
				lMeshCache->UpdateVertexPosition(lMesh, mCache.vertexArray[i]);

		}



		std::vector<FBXModelData*>mdv;
		//�A�j���[�V�����Ȃǂ��ׂĂ̏������I�������߂�l�p�̃W�I���g�����쐬����
		if (lMeshCache) {
			const int lSubMeshCount = lMeshCache->GetSubMeshCount();
			//�T�u���b�V���̍쐬
			lMeshData->subMesh.reserve(lSubMeshCount);
			//�T�u���b�V���̌����s��
			std::shared_ptr<FBXModelData>md;
			for (int lIndex = 0; lIndex < lSubMeshCount; ++lIndex) {

				int lOffset = lMeshCache->mSubMeshes[lIndex]->IndexOffset;

				//���f���C���X�^���X�쐬
				md = std::make_shared<FBXModelData>();
				//�}�e���A�����̎擾
				const FbxSurfaceMaterial*lMaterial = node->GetMaterial(lIndex);

				if (lMaterial) {
					FbxMaterialCache * lMaterialCache = static_cast<FbxMaterialCache *>(lMaterial->GetUserDataPtr());
					if (lMaterialCache)
					{
						lMaterialCache->SetCurrentMaterial(md);
					}
				}
				else {
					FbxMaterialCache::SetDefaultMaterial(md);
				}


				//�W�I���g���o�b�t�@�쐬
				auto loopCount = lMeshCache->mSubMeshes[lIndex]->TriangleCount * 3;
				md->IndexLength = loopCount;

				auto vertexCount = lMeshCache->lVertices.Size();
				md->PosLength = vertexCount / 4;
				md->Data.resize(md->PosLength);

				//�C���f�b�N�X���W���擾
				md->Index = lMeshCache->lIndices.GetArray() + lOffset;
				//				md->Index = lMeshCache->lIndices.GetArray() + lOffset;
				auto count = lMeshCache->lIndices.GetCount();

				md->PolygonCount = lMesh->GetPolygonCount();

				for (int i = 0; i < vertexCount / 4; ++i) {

					//���_�̃R�s�[
					memcpy(&md->Data[i].Pos, &lMeshCache->lVertices.GetArray()[i * 4], sizeof(md->Data[i].Pos));


					if (!lHasDeformation) {
						if (lMeshCache->lNormals.Size() > 0) {
							memcpy(&md->Data[i].Normal, &lMeshCache->lNormals.GetArray()[i * 3], sizeof(D3DXVECTOR3));
						}
					}




					if (lMeshCache->lUVs.Size()>0) {
						memcpy(&md->Data[i].UV, &lMeshCache->lUVs.GetArray()[i * 2], sizeof(D3DXVECTOR2));
					}
				}

				for (int i = 0; i < count; ++i) {
					int idx = lMeshCache->lIndices.GetArray()[i];
					int a = idx;
				}

				//���_���W�ƃC���f�b�N�X���W�����Ƃɖ@���̌v�Z
				if (lHasDeformation) {
					for (int i = 0; i < lMeshCache->lIndices.Size() / 3 - 2; ++i) {
						DXVector3 v0;
						DXVector3 v1;
						DXVector3 v2;
						DXVector3 vv, vv1, vv2;
						v0 = (DXVector3)md->Data[md->Index[i * 3 + 0]].Pos;
						v1 = (DXVector3)md->Data[md->Index[i * 3 + 1]].Pos;
						v2 = (DXVector3)md->Data[md->Index[i * 3 + 2]].Pos;
						vv1 = v1 - v0;
						vv2 = v2 - v0;

						D3DXVec3Cross(&vv, &vv1, &vv2);
						D3DXVec3Normalize(&vv, &vv);
						memcpy(&md->Data[md->Index[i * 3 + 0]].Normal, &vv, sizeof(DXVector3));
						memcpy(&md->Data[md->Index[i * 3 + 1]].Normal, &vv, sizeof(DXVector3));
						memcpy(&md->Data[md->Index[i * 3 + 2]].Normal, &vv, sizeof(DXVector3));
						

					}
				}




				//md.Index = idx;
				//md.Pos = vv;
				//�T�u���b�V���̓o�^
				lMeshData->subMesh.push_back(md);

				//				mdv.push_back(md);
			}
		}
		else {

			//const int lPolygonCount = lMesh->GetPolygonCount();
			//for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; lPolygonIndex++) {
			//	const int VerticeCount = lMesh->GetPolygonSize(lPolygonIndex);

			//	SimpleVertex v;
			//	std::vector<SimpleVertex>vv;
			//	ModelData* md=new ModelData();

			//	for (int lVerticeIndex = 0; lVerticeIndex < lVertexCount; lVerticeIndex++) {
			//		auto x = lVertexArray[lMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex)];
			//		md->Pos = x[0];
			//		md->Pos = x[1];
			//		md->Pos = x[];
			//		v.Pos.x = x[0];
			//		v.Pos.y = x[1];
			//		v.Pos.z = x[2];
			//		
			//		vv.push_back(v);
			//	}
			//	md.Pos = vv;
			//	mdv.push_back(md);

			//}
		}
		//�T�u���b�V���̓o�^
		mMesh->push_back(lMeshData);

		//Geometry.push_back(mdv);


	}




	return mMesh;
}

void DX11FbxLoader::GetMeshCount(std::vector<int>& pCountVector)
{
	//���b�V���A�T�u���b�V���̌���\���z��̍쐬
	pCountVector.resize(nodemeshes.size());
	for (unsigned int i = 0; i < pCountVector.size(); ++i) {
		auto lMesh = nodemeshes[i]->GetMesh();

		FbxVBO* lMeshCache = static_cast<FbxVBO*>(lMesh->GetUserDataPtr());
		pCountVector[i] = lMeshCache->GetSubMeshCount();
	}
}

void DX11FbxLoader::GetMeshVertexCount(std::vector<std::vector<int>>& pCountVector)
{
	pCountVector.resize(nodemeshes.size());
	for (unsigned int i = 0; i < pCountVector.size(); ++i) {
		auto lMesh = nodemeshes[i]->GetMesh();

		FbxVBO* lMeshCache = static_cast<FbxVBO*>(lMesh->GetUserDataPtr());
		pCountVector[i].resize(lMeshCache->GetSubMeshCount());
		for (unsigned int j = 0; j < pCountVector[i].size(); ++j) {
			//���_��
			//Xx,y,z,w��1�Ƃ��邽��4�Ŋ���
			pCountVector[i][j] = lMeshCache->lVertices.Size() / 4;
		}

	}

}

void DX11FbxLoader::GetMeshIndexCount(std::vector<std::vector<int>>& pCountVector)
{
	pCountVector.resize(nodemeshes.size());
	for (unsigned int i = 0; i < pCountVector.size(); ++i) {
		auto lMesh = nodemeshes[i]->GetMesh();

		FbxVBO* lMeshCache = static_cast<FbxVBO*>(lMesh->GetUserDataPtr());
		pCountVector[i].resize(lMeshCache->GetSubMeshCount());
		for (unsigned int j = 0; j < pCountVector[i].size(); ++j) {
			//�C���f�b�N�X���擾
			pCountVector[i][j] = lMeshCache->mSubMeshes[j]->TriangleCount * 3;
		}

	}
}



//�ǂ̃A�j���[�V�������g�����ݒ肷��
bool DX11FbxLoader::SetCurrentAnimStack(int pIndex)
{
	const int lAnimStackCount = AnimStackNameArray.GetCount();

	//�A�j���[�V�������Ȃ� or ���݂��Ȃ��A�j���[�V�����̎w��������ꍇ�̓G���[
	if (lAnimStackCount == 0 || pIndex >= lAnimStackCount || pIndex<0) {
		return false;
	}

	FbxAnimStack*lCurrentAnimationStack = SceneAnim->FindMember<FbxAnimStack>(AnimStackNameArray[pIndex]->Buffer());
	if (lCurrentAnimationStack == nullptr) {
		//�V�[���ɃA�j���[�V�����X�^�b�N���Ȃ��ꍇ�͐ݒ�ł��Ȃ�
		return false;
	}

	//�A�j���[�V�����̃��C���[���擾
	CurrentAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();
	//�V�[���ɃA�j���[�V�������C���[��ݒ肷��
	SceneAnim->SetCurrentAnimationStack(lCurrentAnimationStack);

	FbxTakeInfo*lCurrentTakeInfo = SceneAnim->GetTakeInfo(*(AnimStackNameArray[pIndex]));
	//�A�j���[�V�����J�n���ԂƏI�����Ԃ��擾����
	if (lCurrentTakeInfo) {
		Start = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
		Stop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
	}
	else {
		FbxTimeSpan lTimeLineTimeSpan;
		SceneAnim->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);
		Start = lTimeLineTimeSpan.GetStart();
		Stop = lTimeLineTimeSpan.GetStop();
	}

	//�擾�������Ԃ��L���Ȏ��Ԃ����ׂ�
	if (Cache_Start < Start)
		Start = Cache_Start;

	if (Cache_Stop  > Stop)
		Stop = Cache_Stop;

	return true;
}

bool DX11FbxLoader::SetCurrentPoseIndex(int pPoseIndex)
{
	SetCurrentAnimStack(pPoseIndex);
	return false;
}



void DX11FbxLoader::ReleaseGeometryData()
{

	mMesh->clear();



}

