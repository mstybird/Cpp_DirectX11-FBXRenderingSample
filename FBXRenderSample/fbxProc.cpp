#include"MAIN.h"
#include<Shlwapi.h>

MYFBX::~MYFBX()
{
	//�W�I���g���̉��
	ReleaseGeometryData();
	//�A�j���[�V�������X�g������̉��
	FbxArrayDelete(AnimStackNameArray);
	//�V�[����ǂݍ���ł����ꍇ�V�[�����J������
	if (Scene) {
		UnLoadCacheRecursive(Scene);
	}
	if (SdkManager)SdkManager->Destroy();
}

//����������
void MYFBX::FbxInit(std::string vfileName) {
	fileName = vfileName;
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
	LoadAnimationData();
}

//���[�v����
void MYFBX::FbxProc() {

}

//�������
void MYFBX::FbxDestroy() {

}

void MYFBX::FbxLoadFromFile()
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


		FrameTime.SetTime(0, 0, 0, 1, 0, Scene->GetGlobalSettings().GetTimeMode());

		//�C���|�[�^�͂���ȏ�s�v�Ȃ̂ŉ������
		Importer->Destroy();
		Importer = nullptr;
	}
}

//�m�[�h�̑����ƃ}�e���A������ǂݎ��A�V�[���ɕK�v�ȃe�N�X�`���t�@�C�������擾����
void MYFBX::LoadCacheRecursive(FbxScene * pScene, FbxAnimLayer * pAnimLayer, std::string& pFbxFileName)
{
	//�e�N�X�`�������擾
	const auto lTextureCount = Scene->GetTextureCount();
	FbxString filePath;
	
	this->TextureFileName.clear();


	for (auto lTextureIndex = 0; lTextureIndex < lTextureCount; lTextureIndex++) {
		//�e�N�X�`���擾
		auto lTexture = Scene->GetTexture(lTextureIndex);
		auto lFileTexture = FbxCast<FbxFileTexture>(lTexture);
		if (lFileTexture && !lFileTexture->GetUserDataPtr()) {
			//FBX�t�@�C���p�X���擾
			const FbxString lFileName = lFileTexture->GetFileName();
			//printf("%s\n", lFileName.Buffer());
			bool lStatus = PathFileExists(lFileName.Buffer());
			if (lStatus)filePath = lFileName;

			const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(pFbxFileName.c_str());
			const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);

			//�ǂݍ��߂Ȃ������ꍇ
			if (!lStatus) {
				//���΃p�X�œǂݍ���
				const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
				//printf("%s\n", lResolvedFileName.Buffer());
				lStatus = PathFileExists(lResolvedFileName.Buffer());
				if (lStatus)filePath = lResolvedFileName;
			}


			//�ǂݍ��߂Ȃ������ꍇ
			if (!lStatus) {
				//�J�����g�f�B���N�g���ɂ���Γǂݍ���
				// Load texture from file name only (relative to FBX file)
				const FbxString lTextureFileName = FbxPathUtils::GetFileName(lFileName);
				const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lTextureFileName);
				lStatus = PathFileExists(lResolvedFileName.Buffer());
				printf("%s\n", lResolvedFileName.Buffer());
				if (lStatus)filePath = lResolvedFileName;

			}


			//�ǂ��̃e�N�X�`���Ȃ̂��L������
			const FbxString uvName = lFileTexture->UVSet.Get();
			//���UV�ɕ����̃e�N�X�`�������݂���\��������
			this->TextureFileName[uvName.Buffer()].push_back(filePath.Buffer());

			//�ǂݍ��߂���t�@�C���e�N�X�`���̃A�h���X��n��
			if (lStatus) {
				FbxString*lTextureName = new FbxString(filePath);
				lFileTexture->SetUserDataPtr(lTextureName);
			}
			
		}
	}

	//�q�m�[�h��T��
	LoadCacheRecursive(Scene->GetRootNode(), pAnimLayer);

}
//�������烂�f���̃��b�V�����Ƃ��B�}�e���A�����擾����
void MYFBX::LoadCacheRecursive(FbxNode * pNode, FbxAnimLayer * pAnimLayer)
{
	//��̃m�[�h�ɑ��݂���}�e���A���̐����擾
	const auto lMaterialCount = pNode->GetMaterialCount();
	//�}�e���A���������[�v
	for (auto lMaterialIndex = 0; lMaterialIndex < lMaterialCount; lMaterialIndex++) {
		//�}�e���A�����擾
		auto lMaterial = pNode->GetMaterial(lMaterialIndex);
		
		if (lMaterial && !lMaterial->GetUserDataPtr()) {
			//�}�e���A���L���b�V���쐬
			FbxAutoPtr<MaterialCache> lMaterialCache(new MaterialCache);
			//�}�e���A���L���b�V���̏�����
			if (lMaterialCache->Initialize(lMaterial)) {
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
				FbxAutoPtr<MYVBO>lMeshCache(new MYVBO);
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

void MYFBX::UnLoadCacheRecursive(FbxScene * pScene)
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


void MYFBX::UnLoadCacheRecursive(FbxNode * pNode)
{
	//�}�e���A���̉��
	const int lMaterialCount = pNode->GetMaterialCount();
	for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex) {
		FbxSurfaceMaterial*lMaterial = pNode->GetMaterial(lMaterialIndex);
		if (lMaterial&&lMaterial->GetUserDataPtr()) {
			MaterialCache*lMaterialCache = static_cast<MaterialCache*>(lMaterial->GetUserDataPtr());
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
				MYVBO*lMeshCache = static_cast<MYVBO*>(lMesh->GetUserDataPtr());
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

void MYFBX::PreparePointCacheData(FbxScene * pScene, FbxTime & pCache_Start, FbxTime & pCache_Stop)
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

void MYFBX::FillPoseArray(FbxScene * pScene, FbxArray<FbxPose*> pPoseArray)
{
	const auto lPoseCount = pScene->GetPoseCount();
	for (auto i = 0; i < lPoseCount; i++) {
		pPoseArray.Add(pScene->GetPose(i));
	}
}

//�A�j���[�V�����̓ǂݍ���
void MYFBX::LoadAnimationData()
{
	int lCurrentAnimStackIndex = 0;	//���݂̃A�j���[�V����
	const FbxArray<FbxString*>&lAnimStackNameArray = GetAnimStackNameArray();
	auto lPoseCount = lAnimStackNameArray.GetCount();
	for (int lPoseIndex = 0; lPoseIndex < lPoseCount; lPoseIndex++) {
		//���݂̃A�j���[�V�����X�^�b�N�����ݓK�p����Ă���A�j���[�V�������ǂ���
		if (lAnimStackNameArray[lPoseIndex]->Compare(Scene->ActiveAnimStackName.Get()) == 0) {
			//�K�p����Ă���Ȃ炻�̃A�j���[�V���������݂̃A�j���[�V�����Ƃ���
			lCurrentAnimStackIndex = lPoseIndex;
		}
	}
	//�ݒ肳�ꂽ�A�j���[�V�������Z�b�g����
	SetCurrentAnimStack(lCurrentAnimStackIndex);
}

std::vector<std::vector<FBXModelData*>>* MYFBX::GetGeometryData(D3DXVECTOR3 *transPos)
{
	//������
	ReleaseGeometryData();


	FbxPose*lPose = nullptr;
	//�|�[�Y���擾
	if (PoseIndex != -1)
	{
		lPose = Scene->GetPose(PoseIndex);
	}
	FbxAMatrix lDummyGlobalPosition;
	

	if (Stop > Start)
	{
		// Set the scene status flag to refresh 
		// the scene in the next timer callback.
		CurrentTime += FrameTime;

		if (CurrentTime > Stop)
		{
			CurrentTime = Start;
		}
	}
	indexCount = 0;
	
	GetNodeRecursive(Scene->GetRootNode(), CurrentTime, CurrentAnimLayer, lDummyGlobalPosition, lPose);

	*transPos = D3DXVECTOR3(lDummyGlobalPosition.mData[3][1], lDummyGlobalPosition.mData[3][2], lDummyGlobalPosition.mData[3][3]);

	return &Geometry;
}

std::vector<std::vector<FBXModelData*>>* MYFBX::GetGeometryData2(D3DXVECTOR3 * transPos)
{
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

	for (int i = 0; i < nodemeshes.size();i++) {

		FbxNode* node = nodemeshes[i];
		FbxAMatrix lGlobalPosition;

		GetGlobalPosition(lGlobalPosition, node, CurrentTime, lPose, &lDummyGlobalPosition);

		FbxAMatrix lGeometryOffset;
		FbxAMatrix lGlobalOffPosition;


		//�W�I���g���̃I�t�Z�b�g���擾
		GetGeometry(lGeometryOffset, node);
		lGlobalOffPosition = lGlobalPosition*lGeometryOffset;

		//�m�[�h�̃��b�V�����擾
		FbxMesh* lMesh = node->GetMesh();

		//���_�����擾
		const int lVertexCount = lMesh->GetControlPointsCount();

		//���_������Ȃ��ꍇ�͏������Ȃ�
		if (lVertexCount == 0) {
			continue;
		}


		//VBO�擾
		MYVBO* lMeshCache = static_cast<MYVBO*>(lMesh->GetUserDataPtr());
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
				ReadVertexCacheData(lMesh, CurrentTime, lVertexArray);
			}
			else {
				if (lHasShape)
				{
					// Deform the vertex array with the shapes.
					ComputeShapeDeformation(lMesh, CurrentTime, CurrentAnimLayer, lVertexArray);
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
					ComputeSkinDeformation(lGlobalOffPosition, lMesh, CurrentTime, lVertexArray, lPose);

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
			//�T�u���b�V���̌����s��
			for (int lIndex = 0; lIndex < lSubMeshCount; lIndex++) {

				int lOffset = lMeshCache->mSubMeshes[lIndex]->IndexOffset;

				//���f���C���X�^���X�쐬
				FBXModelData* md = new FBXModelData();

				//�}�e���A�����̎擾
				const FbxSurfaceMaterial*lMaterial = node->GetMaterial(lIndex);

				if (lMaterial) {
					MaterialCache * lMaterialCache = static_cast<MaterialCache *>(lMaterial->GetUserDataPtr());
					if (lMaterialCache)
					{
						lMaterialCache->SetCurrentMaterial(md);
					}
				}
				else {
					MaterialCache::SetDefaultMaterial(md);
				}


				//�W�I���g���o�b�t�@�쐬
				auto loopCount = lMeshCache->mSubMeshes[lIndex]->TriangleCount * 3;
				md->IndexLength = loopCount;
				auto count = lMeshCache->lVertices.Size();
				indexCount = 0;

				auto vertexCount = lMeshCache->lVertices.Size();
				md->PosLength = vertexCount / 4;
				md->Data.resize(md->PosLength);
				for (int i = 0; i < vertexCount / 4; ++i) {

					//���_�̃R�s�[
					memcpy(&md->Data[i].Pos, &lMeshCache->lVertices.GetArray()[i * 4], sizeof(SimpleVector4));


					if (!lHasDeformation)
						memcpy(&md->Data[i].Normal, &lMeshCache->lNormals.GetArray()[i * 3], sizeof(SimpleVector3));




					if (lMeshCache->lUVs.Size()>0) {
						memcpy(&md->Data[i].UV, &lMeshCache->lUVs.GetArray()[i * 2], sizeof(SimpleVector2));

					}
				}

				//�C���f�b�N�X���W���擾
				md->Index = lMeshCache->lIndices.GetArray() + lOffset;

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
				mdv.push_back(md);
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
		Geometry.push_back(mdv);
		end = timeGetTime();

		delete[]lVertexArray;


	}
	*transPos = D3DXVECTOR3(lDummyGlobalPosition.mData[3][1], lDummyGlobalPosition.mData[3][2], lDummyGlobalPosition.mData[3][3]);



	return &Geometry;
}

void MYFBX::GetNodeRecursive(FbxNode * pNode, FbxTime & pTime, FbxAnimLayer * pAnimLayer, FbxAMatrix & pParentGlobalPosition, FbxPose * pPose)
{

		FbxAMatrix lGlobalPosition;

		GetGlobalPosition(lGlobalPosition,pNode, pTime, pPose, &pParentGlobalPosition);


	if (pNode->GetNodeAttribute()) {

		FbxAMatrix lGeometryOffset;
		FbxAMatrix lGlobalOffPosition;


		//�W�I���g���̃I�t�Z�b�g���擾
		GetGeometry(lGeometryOffset,pNode);
		lGlobalOffPosition = lGlobalPosition*lGeometryOffset;
		GetNode(pNode, pTime, pAnimLayer, pParentGlobalPosition, lGlobalOffPosition, pPose);

	}

	//�q�m�[�h�̒T��
	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; lChildIndex++) {
		
		GetNodeRecursive(pNode->GetChild(lChildIndex), pTime, pAnimLayer, lGlobalPosition, pPose);
	}

}

void MYFBX::GetNode(FbxNode * pNode, FbxTime & pTime, FbxAnimLayer * pAnimLayer, FbxAMatrix & pParentGlobalPosition, FbxAMatrix & pGlobalPosition, FbxPose * pPose)
{

	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	if (lNodeAttribute && lNodeAttribute->GetAttributeType()==FbxNodeAttribute::eMesh) {



	//	pNode->LclRotation.Set(FbxDouble3(0, 0, 45));

		start = timeGetTime();
		//�m�[�h�̃��b�V�����擾
		FbxMesh* lMesh = pNode->GetMesh();
		
		//���_�����擾
		const int lVertexCount = lMesh->GetControlPointsCount();

		//���_������Ȃ��ꍇ�͏������Ȃ�
		if (lVertexCount == 0) {
			return;
		}

		//VBO�擾
		MYVBO* lMeshCache = static_cast<MYVBO*>(lMesh->GetUserDataPtr());
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
			lNormalArray = new FbxVector4[lVertexCount];
			//���_�o�b�t�@�̃R�s�[(�擾)
			memcpy(lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
			memcpy(lNormalArray, lMesh->GetElementNormal(0), lVertexCount * sizeof(FbxVector4));
			

		}

		//�A�j���[�V�����֌W�̏���
		//�f�t�H�[�}�[���������ꍇ�A�A�j���[�V��������
		if (lHasDeformation) {

			if (lHasVertexCache) {
				ReadVertexCacheData(lMesh, pTime, lVertexArray);
			}else {
				if (lHasShape)
				{
					// Deform the vertex array with the shapes.
					ComputeShapeDeformation(lMesh, pTime, pAnimLayer, lVertexArray);
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
					ComputeSkinDeformation(pGlobalPosition, lMesh, pTime, lVertexArray, pPose);

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
			//�T�u���b�V���̌����s��
			for (int lIndex = 0; lIndex < lSubMeshCount; lIndex++) {

				int lOffset = lMeshCache->mSubMeshes[lIndex]->IndexOffset;

				//���f���C���X�^���X�쐬
				FBXModelData* md = new FBXModelData();
				
				//�}�e���A�����̎擾
				const FbxSurfaceMaterial*lMaterial = pNode->GetMaterial(lIndex);
				if (lMaterial) {

				}


				//�W�I���g���o�b�t�@�쐬
				auto loopCount = lMeshCache->mSubMeshes[lIndex]->TriangleCount * 3;
				md->IndexLength = loopCount;
				auto count = lMeshCache->lVertices.Size();
				indexCount = 0;

				auto vertexCount = lMeshCache->lVertices.Size();
				md->PosLength = vertexCount / 4;
				md->Data.resize(md->PosLength);
				for (int i = 0; i < vertexCount/4; ++i) {
					
					//���_�̃R�s�[
					memcpy(&md->Data[i].Pos, &lMeshCache->lVertices.GetArray()[i*4],sizeof(SimpleVector4));

						
					if(!lHasDeformation)
					memcpy(&md->Data[i].Normal, &lMeshCache->lNormals.GetArray()[i * 3], sizeof(SimpleVector3));




					if (lMeshCache->lUVs.Size()>0) {
						memcpy(&md->Data[i].UV, &lMeshCache->lUVs.GetArray()[i * 2], sizeof(SimpleVector2));

					}
				}

				//�C���f�b�N�X���W���擾
				md->Index = lMeshCache->lIndices.GetArray() + lOffset;

				//���_���W�ƃC���f�b�N�X���W�����Ƃɖ@���̌v�Z
				if (lHasDeformation) {
					for (int i = 0; i < lMeshCache->lIndices.Size()/3 - 2; i++) {
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
				mdv.push_back(md);
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



		Geometry.push_back(mdv);
		end = timeGetTime();

		delete[]lVertexArray;

	}

	++indexCount;

}

void MYFBX::GetGlobalPosition(FbxAMatrix&pDstMatrix,FbxNode * pNode, const FbxTime & pTime, FbxPose * pPose, FbxAMatrix * pParentGlobalPosition)
{
	//FbxAMatrix lGlobalPosition;
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
						 GetGlobalPosition(lParentGlobalPosition,pNode->GetParent(), pTime, pPose);
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

	//return lGlobalPosition;
}

FbxAMatrix MYFBX::GetPoseMatrix(FbxPose * pPose, int pNodeIndex)
{
	FbxAMatrix lPoseMatrix;
	FbxMatrix lMatrix;
	
	lPoseMatrix;
	lMatrix = pPose->GetMatrix(pNodeIndex);
	memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

	return lPoseMatrix;
}

void MYFBX::GetGeometry(FbxAMatrix&pSrcMatrix,FbxNode * pNode)
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

//�ǂ̃A�j���[�V�������g�����ݒ肷��
bool MYFBX::SetCurrentAnimStack(int pIndex)
{
	const int lAnimStackCount = AnimStackNameArray.GetCount();

	//�A�j���[�V�������Ȃ� or ���݂��Ȃ��A�j���[�V�����̎w��������ꍇ�̓G���[
	if (lAnimStackCount == 0 || pIndex >= lAnimStackCount) {
		return false;
	}

	FbxAnimStack*lCurrentAnimationStack = Scene->FindMember<FbxAnimStack>(AnimStackNameArray[pIndex]->Buffer());
	if (lCurrentAnimationStack == nullptr) {
		//�V�[���ɃA�j���[�V�����X�^�b�N���Ȃ��ꍇ�͐ݒ�ł��Ȃ�
		return false;
	}

	//�A�j���[�V�����̃��C���[���擾
	CurrentAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();
	//�V�[���ɃA�j���[�V�������C���[��ݒ肷��
	Scene->SetCurrentAnimationStack(lCurrentAnimationStack);

	FbxTakeInfo*lCurrentTakeInfo = Scene->GetTakeInfo(*(AnimStackNameArray[pIndex]));
	//�A�j���[�V�����J�n���ԂƏI�����Ԃ��擾����
	if (lCurrentTakeInfo) {
		Start = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
		Stop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
	}
	else {
		FbxTimeSpan lTimeLineTimeSpan;
		Scene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);
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

void MYFBX::ReadVertexCacheData(FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray)
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

void MYFBX::ComputeShapeDeformation(FbxMesh * pMesh, FbxTime & pTime, FbxAnimLayer * pAnimLayer, FbxVector4 * pVertexArray)
{
	int lVertexCount = pMesh->GetControlPointsCount();

	FbxVector4*lSrcVertexArray = pVertexArray;
	FbxVector4*lDstVertexArray=new FbxVector4[lVertexCount];
	memcpy(lDstVertexArray, pVertexArray, lVertexCount * sizeof(FbxVector4));

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
					memcpy(lDstVertexArray, lSrcVertexArray, lVertexCount * sizeof(FbxVector4));
					for (int j = 0; j < lVertexCount; ++j) {
						FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lSrcVertexArray[j])*lWeight*0.01;
						lDstVertexArray[j] += lInfluence;
					}
				}
				else if (lStartShape&&lEndShape) {
					double lStartWeight = lFullWeights[lStartIndex];
					double lEndWeight = lFullWeights[lEndIndex];
					//�E�F�C�g�̌v�Z
					lWeight = (lWeight / lEndWeight) * 100;
					//�o�͒��_�f�[�^��������
					memcpy(lDstVertexArray, lStartShape->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

					for (int j = 0; j < lVertexCount; ++j) {
						FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lStartShape->GetControlPoints()[j])*lWeight*0.01;
						lDstVertexArray[j] += lInfluence;
					}
				}
			}
		}
	}
	memcpy(pVertexArray, lDstVertexArray, lVertexCount * sizeof(FbxVector4));
	delete[] lDstVertexArray;
}

void MYFBX::ComputeSkinDeformation(FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose)
{
	FbxSkin*lSkinDeformer = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);
	FbxSkin::EType lSkinningType = lSkinDeformer->GetSkinningType();

	if (lSkinningType == FbxSkin::eLinear || lSkinningType == FbxSkin::eRigid) {
		ComputeLinearDeformation(pGlobalPosition, pMesh, pTime, pVertexArray, pPose);
	}
	else if (lSkinningType == FbxSkin::eDualQuaternion) {
		ComputeDualQuaternionDeformation(pGlobalPosition, pMesh, pTime, pVertexArray, pPose);
	}
	else if (lSkinningType == FbxSkin::eBlend) {
		int lVertexCount = pMesh->GetControlPointsCount();
		FbxVector4*lVertexArrayLinear = new FbxVector4[lVertexCount];
		memcpy(lVertexArrayLinear, pMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
		FbxVector4*lVertexArrayDQ = new FbxVector4[lVertexCount];
		memcpy(lVertexArrayDQ, pMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));

		ComputeLinearDeformation(pGlobalPosition, pMesh, pTime, lVertexArrayDQ, pPose);
		ComputeDualQuaternionDeformation(pGlobalPosition, pMesh, pTime, lVertexArrayDQ, pPose);

		//2�̃X�L�j���O�̃E�F�C�g����������
		int lBlendWeightsCount = lSkinDeformer->GetControlPointIndicesCount();
		for (int lBWIndex = 0; lBWIndex < lBlendWeightsCount; ++lBWIndex) {
			double lBlendWeight = lSkinDeformer->GetControlPointBlendWeights()[lBWIndex];
			pVertexArray[lBWIndex] = lVertexArrayDQ[lBWIndex] * lBlendWeight + lVertexArrayLinear[lBWIndex] * (1 - lBlendWeight);
		}

	}
}

void MYFBX::ComputeLinearDeformation(FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose)
{
	FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();
	int lVertexCount = pMesh->GetControlPointsCount();

	

	FbxAMatrix*lClusterDeformation = new FbxAMatrix[lVertexCount];

	memset(lClusterDeformation, 0, lVertexCount * sizeof(FbxAMatrix));
	double*lClusterWeight = new double[lVertexCount];
	memset(lClusterWeight, 0, lVertexCount * sizeof(double));
	if (lClusterMode == FbxCluster::eAdditive) {
		for (int i = 0; i < lVertexCount; ++i) {
			lClusterDeformation[i].SetIdentity();
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

			//��邱�ƃ��X�g/////////////////////////////////////////
			//���[�J���ϐ��������o�ϐ�������
			//FBXAMATRIX�̌v�Z���œK������
			//�����������������


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

				MatrixScale(lInfluence.Buffer()->Buffer(), lWeight);


				if (lClusterMode == FbxCluster::eAdditive) {
					//�Ńt�H�[�}�[���s��ɍ�������
					MatrixAddToDiagonal(lInfluence, 1.0 - lWeight);
					lClusterDeformation[lIndex] = lInfluence * lClusterDeformation[lIndex];
					//�N���X�^�̃E�F�C�g��1.0�ɂ���
					lClusterWeight[lIndex] = 1.0;
				}
				else {
					MatrixAdd(lClusterDeformation[lIndex].Buffer()->Buffer(), lInfluence);
					//���K�������W�I���g�����R���s���[�g�W�I���g���ɃE�F�C�g�����Z����
					lClusterWeight[lIndex] += lWeight;
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
		double lWeight = lClusterWeight[i];

		// Deform the vertex if there was at least a link with an influence on the vertex,
		//�E�F�C�g��0�łȂ��ꍇ�̂݌v�Z
		if (lWeight != 0.0)
		{
			//�x�N�g���ɍs��������������̂��擾����
			lDstVertex = lClusterDeformation[i].MultT(lSrcVertex);
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

	//�@���v�Z
	

	delete[] lClusterDeformation;
	delete[] lClusterWeight;

}

void MYFBX::ComputeDualQuaternionDeformation(FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose)
{
	FbxCluster::ELinkMode lClusterMode = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(0)->GetLinkMode();

	int lVertexCount = pMesh->GetControlPointsCount();
	int lSkinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin);

	FbxDualQuaternion*lDQClusterDeformation = new FbxDualQuaternion[lVertexCount];
	memset(lDQClusterDeformation, 0, lVertexCount * sizeof(FbxDualQuaternion));
	double*lClusterWeight = new double[lVertexCount];
	memset(lClusterWeight, 0, lVertexCount * sizeof(double));

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
					lDQClusterDeformation[lIndex] = lInfluence;
					//�E�F�C�g��1.0�ɌŒ�
					lClusterWeight[lIndex] = 1.0;
				}
				else // lLinkMode == FbxCluster::eNormalize || lLinkMode == FbxCluster::eTotalOne
				{
					if (lClusterIndex == 0)
					{
						lDQClusterDeformation[lIndex] = lInfluence;
					}
					else
					{
						// Add to the sum of the deformations on the vertex.
						// Make sure the deformation is accumulated in the same rotation direction. 
						// Use dot product to judge the sign.
						//���_��ό`����l�����Z
						double lSign = lDQClusterDeformation[lIndex].GetFirstQuaternion().DotProduct(lDualQuaternion.GetFirstQuaternion());
						//�K�����������Z����悤�ɂ���
						if (lSign >= 0.0)
						{
							lDQClusterDeformation[lIndex] += lInfluence;
						}
						else
						{
							lDQClusterDeformation[lIndex] -= lInfluence;
						}
					}
					//�E�F�C�g�����Z
					lClusterWeight[lIndex] += lWeight;
				}

			}
		}
	}

	//���_������
	for (int i = 0; i < lVertexCount; ++i)
	{
		FbxVector4 lSrcVertex = pVertexArray[i];
		FbxVector4& lDstVertex = pVertexArray[i];
		double lWeightSum = lClusterWeight[i];

		// Deform the vertex if there was at least a link with an influence on the vertex,
		//�E�F�C�g��0�Ȃ疢����
		if (lWeightSum != 0.0)
		{
			lDQClusterDeformation[i].Normalize();
			lDstVertex = lDQClusterDeformation[i].Deform(lDstVertex);

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
	delete[] lDQClusterDeformation;
	delete[] lClusterWeight;

}

//���_��ϊ�����s����v�Z����
void MYFBX::ComputeClusterDeformation(FbxAMatrix& pGlobalPosition,FbxMesh* pMesh,FbxCluster* pCluster,FbxAMatrix& pVertexTransformMatrix,FbxTime pTime,FbxPose* pPose)
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
		
		D3DXMATRIX m;
		D3DXMATRIX m2;
		{
		 GetGeometry(lReferenceGeometry,pMesh->GetNode());
		pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
		lReferenceGlobalInitPosition *= lReferenceGeometry;
		GetGlobalPosition(lClusterGlobalCurrentPosition,pCluster->GetLink(), pTime, pPose);
		}

		{
		memcpy(&lReferenceGlobalCurrentPosition, &pGlobalPosition, sizeof(FbxAMatrix));
		MatrixInverse(lReferenceGlobalCurrentPosition);
		pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
		lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse();
		}

		

		{
		MatrixFbxToD3DX(&m, lReferenceGlobalCurrentPosition);
		MatrixFbxToD3DX(&m2, lClusterGlobalCurrentPosition);
		m = m2;
		MatrixD3DXToFbx(lClusterRelativeCurrentPositionInverse, &m);
		// Compute the shift of the link relative to the reference.
		pVertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition * lReferenceGlobalInitPosition;
		}

	}


}

void MYFBX::MatrixScale(FbxDouble* pMatrix, double pValue)
{
	//FbxDouble* v = pMatrix.Buffer()->Buffer();
	
	for (int i = 0; i < 16; ++i) {
		pMatrix[i] *= pValue;
	}

}

void MYFBX::MatrixAddToDiagonal(FbxAMatrix & pMatrix, double pValue)
{



	pMatrix[0][0] += pValue;
	pMatrix[1][1] += pValue;
	pMatrix[2][2] += pValue;
	pMatrix[3][3] += pValue;
}

void MYFBX::MatrixAdd(FbxDouble* pDstMatrix, FbxDouble*pSrcMatrix)
{
	int i, j;

	//FbxDouble* lDstMatrix = pDstMatrix.Buffer()->Buffer();
	//FbxDouble* lSrcMatrix = pSrcMatrix.Buffer()->Buffer();

	for (int i = 0; i < 16; ++i) {
		pDstMatrix[i] += pSrcMatrix[i];
	}

	//for (i = 0; i < 4; i++)
	//{
	//	for (j = 0; j < 4; j++)
	//	{
	//		pDstMatrix[i][j] += pSrcMatrix[i][j];
	//	}
	//}
}

void MYFBX::MatrixInverse(FbxDouble * pDstMatrix)
{
	D3DXMATRIX m;
	MatrixFbxToD3DX(&m, pDstMatrix);
	D3DXMatrixInverse(&m, nullptr, &m);
	MatrixD3DXToFbx(pDstMatrix, &m);
}

void MYFBX::MatrixFbxToD3DX(D3DXMATRIX * pDstMatrix, FbxDouble * pSrcMatrix)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			pDstMatrix->m[i][j] = pSrcMatrix[i * 4 + j];
		}
	}

}

void MYFBX::MatrixD3DXToFbx(FbxDouble * pDstMatrix, D3DXMATRIX * pSrcMatrix)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			pDstMatrix[i * 4 + j] = pSrcMatrix->m[i][j];
		}
	}
}

void MYFBX::ReleaseGeometryData()
{
	for (int i = 0; i < Geometry.size(); i++) {
		for (int j = 0; j < Geometry[i].size(); j++) {
			if (Geometry[i][j] != nullptr) {
				delete Geometry[i][j];
				Geometry[i][j] = nullptr;
			}
		}
		Geometry[i].clear();
	}
	Geometry.clear();
}

