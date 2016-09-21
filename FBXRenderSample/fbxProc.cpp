#include"DX11FbxLoader.h"
#include"Helper.h"
#include<Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#pragma warning(disable : 4800)

DX11FbxLoader::DX11FbxLoader():
	mMesh{std::make_shared<std::vector<std::shared_ptr<FBXMesh>>>()}
{
}

DX11FbxLoader::~DX11FbxLoader()
{
	FbxDestroy();
}

//����������
void DX11FbxLoader::FbxInit(std::string vfileName, bool animationLoad) {
	fileName = vfileName;
	MSString::GetFolderPath(fileRelativePath, fileName);
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
	FBXSDK_printf("%s\n",WindowMessage.Buffer());

	//�t�@�C���̃C���|�[�g
	FbxLoadFromFile();
	//�A�j���[�V�����̏�����
	SceneAnim = Scene;
	LoadAnimationData();
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
			FbxAxisSystem::eYAxis,
			FbxAxisSystem::eParityOdd,
			FbxAxisSystem::eRightHanded
		);


		//�ǂݍ��񂾃t�@�C���ƍ��W�n���قȂ�ꍇ�̓V�[���̍��W�n��ϊ�
		if (SceneAxisSystem != OurAxisSystem) {
			OurAxisSystem.ConvertScene(Scene);
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
				//std::string*lTextureName = new std::string(filePath);
				//lFileTexture->SetUserDataPtr(lTextureName);
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
			if (lMaterialCache->Initialize(lMaterial,fileRelativePath)) {
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
	for(auto lIndex=0;lIndex<lNodeCount; lIndex++){
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
	EnableAnimation = lCurrentAnimStackIndex != -1 ? true : false;
	SetCurrentAnimStack(lCurrentAnimStackIndex);
}

void DX11FbxLoader::SetAnimation(std::string pName)
{
	SetCurrentAnimStack(AnimData[pName]);
}

void DX11FbxLoader::SetAnimation(int pIndex)
{
	SetCurrentAnimStack(pIndex);
}


std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>> DX11FbxLoader::GetGeometryData2()
{
	//���b�V���f�[�^�̉��
	ReleaseGeometryData();

	FbxPose*lPose{ nullptr };
	if (PoseIndex != -1)
	{
		lPose = Scene->GetPose(PoseIndex);
	}
	FbxAMatrix lDummyGlobalPosition;


	if (Stop > Start)
	{
		CurrentTime += FrameTime;

		if (CurrentTime > Stop)
		{
			CurrentTime = Start;
		}
	}

	//���b�V���̐������z��쐬
	mMesh = std::make_shared<std::vector<std::shared_ptr<FBXMesh>>>();
	mMesh->reserve(nodemeshes.size());
	std::shared_ptr<FBXMesh>lMeshData;

	
	

	for (unsigned int i = 0; i < nodemeshes.size();i++) {
		//���b�V���쐬
		lMeshData = std::make_shared<FBXMesh>();
//		FBXMesh*lMeshData = new FBXMesh;

		FbxNode* node = nodemeshes[i];
		FbxAMatrix lGlobalPosition;

		FbxComputeDeformer::GetGlobalPosition(lGlobalPosition, node, CurrentTime, lPose, &lDummyGlobalPosition);
		
		FbxAMatrix lGeometryOffset;
		FbxAMatrix lGlobalOffPosition;


		//�W�I���g���̃I�t�Z�b�g���擾
		FbxComputeDeformer::GetGeometry(lGeometryOffset, node);
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
		FbxVector4* lVertexArray = nullptr;
		//�@���o�b�t�@
		FbxVector4* lNormalArray = nullptr;
		//VBO���쐬 or �{�[�����Ȃ��ꍇ�A���̂܂܃R�s�[
		if (!lMeshCache || lHasDeformation) {

			lVertexArray = new FbxVector4[lVertexCount];
			//���_�o�b�t�@�̃R�s�[(�擾)
			memmove(lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
		}

		//�A�j���[�V�����֌W�̏���
		//�f�t�H�[�}�[���������ꍇ�A�A�j���[�V��������

		if (lHasDeformation) {

			if (lHasVertexCache) {
				FbxComputeDeformer::ReadVertexCacheData(lMesh, CurrentTime, lVertexArray);
			}
			else {
				if (lHasShape)
				{
					// Deform the vertex array with the shapes.
					FbxComputeDeformer::ComputeShapeDeformation(lMesh, CurrentTime, CurrentAnimLayer, lVertexArray);
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
					// Deform the vertex array with the skin deformer.
					FbxComputeDeformer::ComputeSkinDeformation(lGlobalOffPosition, lMesh, CurrentTime, lVertexArray, lPose);

				}

			}



			if (lMeshCache)
				//�X�V�������_�̔��f
				lMeshCache->UpdateVertexPosition(lMesh, lVertexArray);

		}



		std::vector<FBXModelData*>mdv;
		//�A�j���[�V�����Ȃǂ��ׂĂ̏������I�������߂�l�p�̃W�I���g�����쐬����
		if (lMeshCache) {
			const int lSubMeshCount = lMeshCache->GetSubMeshCount();
			//�T�u���b�V���̍쐬
			lMeshData->subMesh.reserve(lSubMeshCount);
			//�T�u���b�V���̌����s��
			std::shared_ptr<FBXModelData>md;
			for (int lIndex = 0; lIndex < lSubMeshCount; lIndex++) {

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
				md->Index= lMeshCache->lIndices.GetArray() + lOffset;
//				md->Index = lMeshCache->lIndices.GetArray() + lOffset;
				auto count = lMeshCache->lIndices.GetCount();

				md->PolygonCount= lMesh->GetPolygonCount();

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

				for (int i = 0; i < count; i++) {
					int idx = lMeshCache->lIndices.GetArray()[i];
					int a = idx;
				}

				//���_���W�ƃC���f�b�N�X���W�����Ƃɖ@���̌v�Z
				if (lHasDeformation) {
					for (int i = 0; i < lMeshCache->lIndices.Size() / 3 - 2; i++) {
						D3DXVECTOR3 v0;
						D3DXVECTOR3 v1;
						D3DXVECTOR3 v2;
						D3DXVECTOR3 vv, vv1, vv2;
						v0 = (D3DXVECTOR3)md->Data[md->Index[i * 3 + 0]].Pos;
						v1 = (D3DXVECTOR3)md->Data[md->Index[i * 3 + 1]].Pos;
						v2 = (D3DXVECTOR3)md->Data[md->Index[i * 3 + 2]].Pos;
						vv1 = v1 - v0;
						vv2 = v2 - v0;

						D3DXVec3Cross(&vv, &vv1, &vv2);
						D3DXVec3Normalize(&vv, &vv);
						memcpy(&md->Data[md->Index[i * 3 + 0]].Normal, &vv, sizeof(D3DXVECTOR3));
						memcpy(&md->Data[md->Index[i * 3 + 1]].Normal, &vv, sizeof(D3DXVECTOR3));
						memcpy(&md->Data[md->Index[i * 3 + 2]].Normal, &vv, sizeof(D3DXVECTOR3));


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

		delete[]lVertexArray;


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
	if (lAnimStackCount == 0 || pIndex >= lAnimStackCount||pIndex<0) {
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

	//�X�^�[�g���Ԃ�ݒ肵�Ă���
	CurrentTime = Start;
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

