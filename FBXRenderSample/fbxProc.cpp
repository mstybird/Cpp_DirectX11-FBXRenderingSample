#include"MAIN.h"
#include<Shlwapi.h>

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
			printf("%s\n", lFileName.Buffer());
			bool lStatus = PathFileExists(lFileName.Buffer());
			if (lStatus)filePath = lFileName;

			const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(pFbxFileName.c_str());
			const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);

			//�ǂݍ��߂Ȃ������ꍇ
			if (!lStatus) {
				//���΃p�X�œǂݍ���
				const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
				printf("%s\n", lResolvedFileName.Buffer());
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
}

void MYFBX::UnLoadCacheRecursive(FbxNode * pNode)
{
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

std::vector<std::vector<ModelData>> MYFBX::GetGeometryData()
{
	int PoseIndex = 0;
	FbxPose*lPose = nullptr;
	//�|�[�Y���擾
	lPose = Scene->GetPose(PoseIndex);

	FbxAMatrix lDummyGlobalPosition;

	indexCount = 0;
	GetNodeRecursive(Scene->GetRootNode(), CurrentTime, CurrentAnimLayer, lDummyGlobalPosition, lPose);
	
	return Geometry;
}

void MYFBX::GetNodeRecursive(FbxNode * pNode, FbxTime & pTime, FbxAnimLayer * pAnimLayer, FbxAMatrix & pParentGlobalPosition, FbxPose * pPose)
{
	FbxAMatrix lGlobalPosition = GetGlobalPosition(pNode, pTime, pPose, &pParentGlobalPosition);

	if (pNode->GetNodeAttribute()) {
		//�W�I���g���̃I�t�Z�b�g���擾
		FbxAMatrix lGeometryOffset = GetGeometry(pNode);
		FbxAMatrix lGlobalOffPosition = lGlobalPosition*lGeometryOffset;
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
		//�m�[�h�̃��b�V�����擾
		FbxMesh* lMesh = pNode->GetMesh();
		//���_�����擾
		const int lVertexCount = lMesh->GetControlPointsCount();

		//���_������Ȃ��ꍇ�͏������Ȃ�
		if (lVertexCount == 0) {
			return;
		}

		//VBO�擾
		const MYVBO* lMeshCache = static_cast<const MYVBO*>(lMesh->GetUserDataPtr());
		//�{�[��������Ȃ�{�[�����擾
		const bool lHasVertexCache = lMesh->GetDeformerCount(FbxDeformer::eVertexCache) && (static_cast<FbxVertexCacheDeformer*>(lMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->Active.Get();
		const bool lHasShape = lMesh->GetShapeCount() > 0;
		const bool lHasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
		const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;

		//���_�o�b�t�@
		FbxVector4* lVertexArray = nullptr;
		//VBO���쐬 or �{�[�����Ȃ��ꍇ�A���̂܂܃R�s�[
		if (!lMeshCache || lHasDeformation) {

			lVertexArray = new FbxVector4[lVertexCount];

			//���_�o�b�t�@�̃R�s�[(�擾)
			memcpy(lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
		}

		//�A�j���[�V�����֌W�̏���
		//�f�t�H�[�}�[���������ꍇ�A�A�j���[�V��������
		if (lHasDeformation) {
			if (lHasVertexCache) {
				ReadVertexCacheData(lMesh, pTime, lVertexArray);
			}
			else {
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



		std::vector<ModelData>mdv;
		//�A�j���[�V�����Ȃǂ��ׂĂ̏������I�������߂�l�p�̃W�I���g�����쐬����
		if (lMeshCache) {
			const int lSubMeshCount = lMeshCache->GetSubMeshCount();
			for (int lIndex = 0; lIndex < lSubMeshCount; lIndex++) {

				int lOffset = lMeshCache->mSubMeshes[lIndex]->IndexOffset;

				ModelData md;

				std::vector<SimpleVertex>vv;
				SimpleVertex v;
				std::vector<unsigned int> idx;
				//�W�I���g���o�b�t�@�쐬
				auto loopCount = lMeshCache->mSubMeshes[lIndex]->TriangleCount * 3;
				idx.resize(loopCount);
				auto count = lMeshCache->lVertices.Size();
				indexCount = 0;

				auto vertexCount = lMeshCache->lVertices.Size();
				for (int i = 0; i < vertexCount/4; i++) {
					v.Pos.x = lMeshCache->lVertices[i * 4 + 0];
					v.Pos.y = lMeshCache->lVertices[i * 4 + 1];
					v.Pos.z = lMeshCache->lVertices[i * 4 + 2];

					//���_�̒ǉ�
					vv.push_back(v);
				}


				for (int i = 0; i < loopCount; i++) {
					lMeshCache->lIndices.GetAt(i + lOffset);
					idx[i] = lMeshCache->lIndices[i+lOffset];


				}


				md.Index = idx;
				md.Pos = vv;
				mdv.push_back(md);
			}
		}
		else {
			const int lPolygonCount = lMesh->GetPolygonCount();
			for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; lPolygonIndex++) {
				const int VerticeCount = lMesh->GetPolygonSize(lPolygonIndex);

				SimpleVertex v;
				std::vector<SimpleVertex>vv;
				ModelData md;

				for (int lVerticeIndex = 0; lVerticeIndex < lVertexCount; lVerticeIndex++) {
					auto x=lVertexArray[lMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex)];
					v.Pos.x = x[0];
					v.Pos.y = x[1];
					v.Pos.z = x[2];
					
					vv.push_back(v);
				}
				md.Pos = vv;
				mdv.push_back(md);

			}
		}
		Geometry.push_back(mdv);
	}

}

FbxAMatrix MYFBX::GetGlobalPosition(FbxNode * pNode, const FbxTime & pTime, FbxPose * pPose, FbxAMatrix * pParentGlobalPosition)
{
	FbxAMatrix lGlobalPosition;
	bool lPositionFound = false;

	if (pPose) {
		int lNodeIndex = pPose->Find(pNode);

		if (lNodeIndex > -1) {
			if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex)) {
				lGlobalPosition = GetPoseMatrix(pPose, lNodeIndex);
			}
			else {
				//���[���h���W�n�ɕϊ�����
				FbxAMatrix lParentGlobalPosition;
				if (pParentGlobalPosition) {
					lParentGlobalPosition = *pParentGlobalPosition;
				}
				else {
					if (pNode->GetParent()) {
						lParentGlobalPosition = GetGlobalPosition(pNode->GetParent(), pTime, pPose);
					}
				}

				FbxAMatrix lLocalPosition = GetPoseMatrix(pPose, lNodeIndex);
				lGlobalPosition = lParentGlobalPosition*lLocalPosition;

			}
			lPositionFound = true;
		}

	}

	if (!lPositionFound) {
		lGlobalPosition = pNode->EvaluateGlobalTransform(pTime);
	}

	return lGlobalPosition;
}

FbxAMatrix MYFBX::GetPoseMatrix(FbxPose * pPose, int pNodeIndex)
{
	FbxAMatrix lPoseMatrix;
	FbxMatrix lMatrix = pPose->GetMatrix(pNodeIndex);
	memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

	return lPoseMatrix;
}

FbxAMatrix MYFBX::GetGeometry(FbxNode * pNode)
{
	const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
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

