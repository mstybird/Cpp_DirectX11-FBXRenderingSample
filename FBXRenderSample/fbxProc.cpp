#include"MAIN.h"
#include<Shlwapi.h>

//初期化処理
void MYFBX::FbxInit(std::string vfileName) {
	fileName = vfileName;
	PoseIndex = -1;
	
	Cache_Start = FBXSDK_TIME_INFINITE;
	Cache_Stop = FBXSDK_TIME_MINUS_INFINITE;

	//SDKの初期化
	SdkManager = FbxManager::Create();

	FBXSDK_printf("Autodesk FBX SDK version %s\n", SdkManager->GetVersion());

	//IO設定
	auto ios = FbxIOSettings::Create(SdkManager, IOSROOT);
	SdkManager->SetIOSettings(ios);

	//プラグインの読み込み
	FbxString lPath = FbxGetApplicationDirectory();
	SdkManager->LoadPluginsDirectory(lPath.Buffer());
	FBXSDK_printf("Loaded Plugins Directory is \" %s \"\n", lPath.Buffer());

	//シーンの作成
	Scene = FbxScene::Create(SdkManager, "MyScene");

	if (!SdkManager) {
		FBXSDK_printf("FBX Initialize Error\n");
	}

	//ファイルのインポート
	auto FileFormat = -1;
	Importer = FbxImporter::Create(SdkManager, "");
	if (!SdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(fileName.c_str(), FileFormat)) {
		//バイナリファイルだった場合、バイナリデータを読み込むための設定
		FileFormat = SdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");;
	}

	

	//インポータの初期化＆ファイルの設定
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

	//ファイルのインポート
	FbxLoadFromFile();
	//アニメーションの初期化
	LoadAnimationData();
}

//ループ処理
void MYFBX::FbxProc() {

}

//解放処理
void MYFBX::FbxDestroy() {

}

void MYFBX::FbxLoadFromFile()
{
	if (Importer->Import(Scene) == true) {
		//座標系の変換
		FbxAxisSystem SceneAxisSystem = Scene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem OurAxisSystem(
			FbxAxisSystem::eYAxis,
			FbxAxisSystem::eParityOdd,
			FbxAxisSystem::eRightHanded
		);
		//読み込んだファイルと座標系が異なる場合はシーンの座標系を変換
		if (SceneAxisSystem != OurAxisSystem) {
			OurAxisSystem.ConvertScene(Scene);
		}

		Scene->FillAnimStackNameArray(AnimStackNameArray);

		//ポリゴンを全て三角形に変換
		FbxGeometryConverter geometryConverter(SdkManager);
		//trueにすると直接書き換える
		geometryConverter.Triangulate(Scene, true);

		//最初のフレームのシーンを取得する
		this->LoadCacheRecursive(Scene, CurrentAnimLayer, fileName);

		//デフォーマーから
		this->PreparePointCacheData(Scene, Cache_Start, Cache_Stop);
		
		//ポーズのリストを作成する
		this->FillPoseArray(Scene, PoseArray);


		FrameTime.SetTime(0, 0, 0, 1, 0, Scene->GetGlobalSettings().GetTimeMode());

		//インポータはこれ以上不要なので解放する
		Importer->Destroy();
		Importer = nullptr;
	}
}

//ノードの属性とマテリアル情報を読み取り、シーンに必要なテクスチャファイル名を取得する
void MYFBX::LoadCacheRecursive(FbxScene * pScene, FbxAnimLayer * pAnimLayer, std::string& pFbxFileName)
{
	//テクスチャ数を取得
	const auto lTextureCount = Scene->GetTextureCount();
	FbxString filePath;
	
	this->TextureFileName.clear();


	for (auto lTextureIndex = 0; lTextureIndex < lTextureCount; lTextureIndex++) {
		//テクスチャ取得
		auto lTexture = Scene->GetTexture(lTextureIndex);
		auto lFileTexture = FbxCast<FbxFileTexture>(lTexture);
		if (lFileTexture && !lFileTexture->GetUserDataPtr()) {
			//FBXファイルパスを取得
			const FbxString lFileName = lFileTexture->GetFileName();
			printf("%s\n", lFileName.Buffer());
			bool lStatus = PathFileExists(lFileName.Buffer());
			if (lStatus)filePath = lFileName;

			const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(pFbxFileName.c_str());
			const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);

			//読み込めなかった場合
			if (!lStatus) {
				//相対パスで読み込み
				const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
				printf("%s\n", lResolvedFileName.Buffer());
				lStatus = PathFileExists(lResolvedFileName.Buffer());
				if (lStatus)filePath = lResolvedFileName;
			}


			//読み込めなかった場合
			if (!lStatus) {
				//カレントディレクトリにあれば読み込む
				// Load texture from file name only (relative to FBX file)
				const FbxString lTextureFileName = FbxPathUtils::GetFileName(lFileName);
				const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lTextureFileName);
				lStatus = PathFileExists(lResolvedFileName.Buffer());
				printf("%s\n", lResolvedFileName.Buffer());
				if (lStatus)filePath = lResolvedFileName;

			}


			//どこのテクスチャなのか記憶する
			const FbxString uvName = lFileTexture->UVSet.Get();
			//一つのUVに複数のテクスチャが存在する可能性がある
			this->TextureFileName[uvName.Buffer()].push_back(filePath.Buffer());

			//読み込めたらファイルテクスチャのアドレスを渡す
			if (lStatus) {
				FbxString*lTextureName = new FbxString(filePath);
				lFileTexture->SetUserDataPtr(lTextureName);
			}
			
		}
	}

	//子ノードを探索
	LoadCacheRecursive(Scene->GetRootNode(), pAnimLayer);

}
//ここからモデルのメッシュ情報とか。マテリアルを取得する
void MYFBX::LoadCacheRecursive(FbxNode * pNode, FbxAnimLayer * pAnimLayer)
{
	//一つのノードに存在するマテリアルの数を取得
	const auto lMaterialCount = pNode->GetMaterialCount();
	//マテリアル数分ループ
	for (auto lMaterialIndex = 0; lMaterialIndex < lMaterialCount; lMaterialIndex++) {
		//マテリアルを取得
		auto lMaterial = pNode->GetMaterial(lMaterialIndex);
		
		if (lMaterial && !lMaterial->GetUserDataPtr()) {
			//マテリアルキャッシュ作成
			FbxAutoPtr<MaterialCache> lMaterialCache(new MaterialCache);
			//マテリアルキャッシュの初期化
			if (lMaterialCache->Initialize(lMaterial)) {
				//キャッシュのポインタをマテリアルに渡す
				lMaterial->SetUserDataPtr(lMaterialCache.Release());
			}
		}
	}

	//ノードの属性を取得
	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
	if (lNodeAttribute) {
		//ノードがメッシュを表しているかどうか
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
			//メッシュを取得
			FbxMesh*lMesh = pNode->GetMesh();
			if (lMesh && !lMesh->GetUserDataPtr()) {
				FbxAutoPtr<MYVBO>lMeshCache(new MYVBO);
				if (lMeshCache->Initialize(lMesh)) {
					//VBOクラスを登録
					lMesh->SetUserDataPtr(lMeshCache.Release());
				}
			}
		}
	}


	//子ノードの数を数える
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
	//ノード数を取得
	const auto lNodeCount = pScene->GetSrcObjectCount<FbxNode>();
	FbxStatus lStatus;
	for(auto lIndex=0;lIndex<lNodeCount; lIndex++){
		FbxNode* lNode = pScene->GetSrcObject<FbxNode>(lIndex);
		if (lNode->GetGeometry()) {
			int i;
			int lVertexCacheDeformerCount = lNode->GetGeometry()->GetDeformerCount(FbxDeformer::eVertexCache);
			lVertexCacheDeformerCount = lVertexCacheDeformerCount > 0 ? 1 : 0;
			for (i = 0; i < lVertexCacheDeformerCount; i++) {
				//頂点キャッシュ
				FbxVertexCacheDeformer* lDeformer = static_cast<FbxVertexCacheDeformer*>(lNode->GetGeometry()->GetDeformer(i, FbxDeformer::eVertexCache));
				if (!lDeformer)continue;
				FbxCache*lCache = lDeformer->GetCache();

				if (!lCache)continue;

				if (lDeformer->Active.Get()) {
					if (lCache->GetCacheFileFormat() == FbxCache::eMaxPointCacheV2) {
						//処理なし
					}
					else if (lCache->GetCacheFileFormat() == FbxCache::eMayaCache) {
						//MCからPC2フォーマットに変換する
						if (!lCache->ConvertFromMCToPC2(FbxTime::GetFrameRate(pScene->GetGlobalSettings().GetTimeMode()), 0, &lStatus)) {
							//失敗した時
							FbxString lTheErrorIs = lStatus.GetErrorString();
						}
					}


					//キャッシュを読み取る
					if (!lCache->OpenFileForRead(&lStatus)) {
						FbxString lTheErrorIs = lStatus.GetErrorString();
						lDeformer->Active = false;
					}
					else {
						FbxTime lChannel_Start;
						FbxTime lChannel_Stop;
						int lChannelIndex = lCache->GetChannelIndex(lDeformer->Channel.Get());
						if (lCache->GetAnimationRange(lChannelIndex, lChannel_Start, lChannel_Stop)) {
							// あイメーション開始フレーム設定
							if (lChannel_Start < pCache_Start) pCache_Start = lChannel_Start;

							// アニメーション終了フレーム設定
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

//アニメーションの読み込み
void MYFBX::LoadAnimationData()
{
	int lCurrentAnimStackIndex = 0;	//現在のアニメーション
	const FbxArray<FbxString*>&lAnimStackNameArray = GetAnimStackNameArray();
	auto lPoseCount = lAnimStackNameArray.GetCount();
	for (int lPoseIndex = 0; lPoseIndex < lPoseCount; lPoseIndex++) {
		//現在のアニメーションスタックが現在適用されているアニメーションかどうか
		if (lAnimStackNameArray[lPoseIndex]->Compare(Scene->ActiveAnimStackName.Get()) == 0) {
			//適用されているならそのアニメーションを現在のアニメーションとする
			lCurrentAnimStackIndex = lPoseIndex;
		}
	}
	//設定されたアニメーションをセットする
	SetCurrentAnimStack(lCurrentAnimStackIndex);
}

std::vector<std::vector<ModelData>> MYFBX::GetGeometryData()
{
	int PoseIndex = 0;
	FbxPose*lPose = nullptr;
	//ポーズを取得
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
		//ジオメトリのオフセットを取得
		FbxAMatrix lGeometryOffset = GetGeometry(pNode);
		FbxAMatrix lGlobalOffPosition = lGlobalPosition*lGeometryOffset;
		GetNode(pNode, pTime, pAnimLayer, pParentGlobalPosition, lGlobalOffPosition, pPose);
	}

	//子ノードの探索
	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; lChildIndex++) {
		GetNodeRecursive(pNode->GetChild(lChildIndex), pTime, pAnimLayer, lGlobalPosition, pPose);
	}

}

void MYFBX::GetNode(FbxNode * pNode, FbxTime & pTime, FbxAnimLayer * pAnimLayer, FbxAMatrix & pParentGlobalPosition, FbxAMatrix & pGlobalPosition, FbxPose * pPose)
{
	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
	if (lNodeAttribute && lNodeAttribute->GetAttributeType()==FbxNodeAttribute::eMesh) {
		//ノードのメッシュを取得
		FbxMesh* lMesh = pNode->GetMesh();
		//頂点数を取得
		const int lVertexCount = lMesh->GetControlPointsCount();

		//頂点が一つもない場合は処理しない
		if (lVertexCount == 0) {
			return;
		}

		//VBO取得
		const MYVBO* lMeshCache = static_cast<const MYVBO*>(lMesh->GetUserDataPtr());
		//ボーンがあるならボーンを取得
		const bool lHasVertexCache = lMesh->GetDeformerCount(FbxDeformer::eVertexCache) && (static_cast<FbxVertexCacheDeformer*>(lMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->Active.Get();
		const bool lHasShape = lMesh->GetShapeCount() > 0;
		const bool lHasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
		const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;

		//頂点バッファ
		FbxVector4* lVertexArray = nullptr;
		//VBO未作成 or ボーンがない場合、そのままコピー
		if (!lMeshCache || lHasDeformation) {

			lVertexArray = new FbxVector4[lVertexCount];

			//頂点バッファのコピー(取得)
			memcpy(lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
		}

		//アニメーション関係の処理
		//デフォーマーがあった場合、アニメーション処理
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
				//更新した頂点の反映
				lMeshCache->UpdateVertexPosition(lMesh, lVertexArray);

		}



		std::vector<ModelData>mdv;
		//アニメーションなどすべての処理が終わったら戻り値用のジオメトリを作成する
		if (lMeshCache) {
			const int lSubMeshCount = lMeshCache->GetSubMeshCount();
			for (int lIndex = 0; lIndex < lSubMeshCount; lIndex++) {

				int lOffset = lMeshCache->mSubMeshes[lIndex]->IndexOffset;

				ModelData md;

				std::vector<SimpleVertex>vv;
				SimpleVertex v;
				std::vector<unsigned int> idx;
				//ジオメトリバッファ作成
				auto loopCount = lMeshCache->mSubMeshes[lIndex]->TriangleCount * 3;
				idx.resize(loopCount);
				auto count = lMeshCache->lVertices.Size();
				indexCount = 0;

				auto vertexCount = lMeshCache->lVertices.Size();
				for (int i = 0; i < vertexCount/4; i++) {
					v.Pos.x = lMeshCache->lVertices[i * 4 + 0];
					v.Pos.y = lMeshCache->lVertices[i * 4 + 1];
					v.Pos.z = lMeshCache->lVertices[i * 4 + 2];

					//頂点の追加
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
				//ワールド座標系に変換する
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

//どのアニメーションを使うか設定する
bool MYFBX::SetCurrentAnimStack(int pIndex)
{
	const int lAnimStackCount = AnimStackNameArray.GetCount();

	//アニメーションがない or 存在しないアニメーションの指定をした場合はエラー
	if (lAnimStackCount == 0 || pIndex >= lAnimStackCount) {
		return false;
	}

	FbxAnimStack*lCurrentAnimationStack = Scene->FindMember<FbxAnimStack>(AnimStackNameArray[pIndex]->Buffer());
	if (lCurrentAnimationStack == nullptr) {
		//シーンにアニメーションスタックがない場合は設定できない
		return false;
	}

	//アニメーションのレイヤーを取得
	CurrentAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();
	//シーンにアニメーションレイヤーを設定する
	Scene->SetCurrentAnimationStack(lCurrentAnimationStack);

	FbxTakeInfo*lCurrentTakeInfo = Scene->GetTakeInfo(*(AnimStackNameArray[pIndex]));
	//アニメーション開始時間と終了時間を取得する
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

	//取得した時間が有効な時間か調べる
	if (Cache_Start < Start)
		Start = Cache_Start;

	if (Cache_Stop  > Stop)
		Stop = Cache_Stop;

	//スタート時間を設定しておく
	CurrentTime = Start;
	return true;
}

