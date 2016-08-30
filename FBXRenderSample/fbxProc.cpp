#include"MAIN.h"
#include<Shlwapi.h>

MYFBX::~MYFBX()
{
	//ジオメトリの解放
	ReleaseGeometryData();
	//アニメーションリスト文字列の解放
	FbxArrayDelete(AnimStackNameArray);
	//シーンを読み込んでいた場合シーンを開放する
	if (Scene) {
		UnLoadCacheRecursive(Scene);
	}
	if (SdkManager)SdkManager->Destroy();
}

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
			//printf("%s\n", lFileName.Buffer());
			bool lStatus = PathFileExists(lFileName.Buffer());
			if (lStatus)filePath = lFileName;

			const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(pFbxFileName.c_str());
			const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);

			//読み込めなかった場合
			if (!lStatus) {
				//相対パスで読み込み
				const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
				//printf("%s\n", lResolvedFileName.Buffer());
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
			//ノードを登録
			nodemeshes.push_back(pNode);
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
	const int lTextureCount = pScene->GetTextureCount();
	for (int lTextureIndex = 0; lTextureIndex < lTextureCount; lTextureIndex++) {
		FbxTexture * lTexture = pScene->GetTexture(lTextureIndex);
		FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>(lTexture);
		if (lFileTexture && lFileTexture->GetUserDataPtr())
		{
			FbxString * lTextureName = static_cast<FbxString *>(lFileTexture->GetUserDataPtr());
			lFileTexture->SetUserDataPtr(NULL);
			//テクスチャイメージの解放
			//glDeleteTextures(1, lTextureName);
			delete lTextureName;
		}
	}
	UnLoadCacheRecursive(pScene->GetRootNode());
}

/*
	ノードの全捜査をなくすために、
	eNode属性を持つノードのみの配列を作成する。
	以降はそのノードにのみアクセスする
*/


void MYFBX::UnLoadCacheRecursive(FbxNode * pNode)
{
	//マテリアルの解放
	const int lMaterialCount = pNode->GetMaterialCount();
	for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex) {
		FbxSurfaceMaterial*lMaterial = pNode->GetMaterial(lMaterialIndex);
		if (lMaterial&&lMaterial->GetUserDataPtr()) {
			MaterialCache*lMaterialCache = static_cast<MaterialCache*>(lMaterial->GetUserDataPtr());
			lMaterial->SetUserDataPtr(nullptr);
			delete lMaterialCache;
		}
	}
	//メッシュを解放する
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

std::vector<std::vector<FBXModelData*>>* MYFBX::GetGeometryData(D3DXVECTOR3 *transPos)
{
	//初期化
	ReleaseGeometryData();


	FbxPose*lPose = nullptr;
	//ポーズを取得
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


		//ジオメトリのオフセットを取得
		GetGeometry(lGeometryOffset, node);
		lGlobalOffPosition = lGlobalPosition*lGeometryOffset;

		//ノードのメッシュを取得
		FbxMesh* lMesh = node->GetMesh();

		//頂点数を取得
		const int lVertexCount = lMesh->GetControlPointsCount();

		//頂点が一つもない場合は処理しない
		if (lVertexCount == 0) {
			continue;
		}


		//VBO取得
		MYVBO* lMeshCache = static_cast<MYVBO*>(lMesh->GetUserDataPtr());
		//ボーンがあるならボーンを取得
		const bool lHasVertexCache = lMesh->GetDeformerCount(FbxDeformer::eVertexCache) && (static_cast<FbxVertexCacheDeformer*>(lMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->Active.Get();
		const bool lHasShape = lMesh->GetShapeCount() > 0;
		const bool lHasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
		const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;


		//頂点バッファ
		FbxVector4* lVertexArray = nullptr;
		//法線バッファ
		FbxVector4* lNormalArray = nullptr;
		//VBO未作成 or ボーンがない場合、そのままコピー
		if (!lMeshCache || lHasDeformation) {

			lVertexArray = new FbxVector4[lVertexCount];
			//頂点バッファのコピー(取得)
			memmove(lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
		}

		//アニメーション関係の処理
		//デフォーマーがあった場合、アニメーション処理
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
				//更新した頂点の反映
				lMeshCache->UpdateVertexPosition(lMesh, lVertexArray);

		}



		std::vector<FBXModelData*>mdv;
		//アニメーションなどすべての処理が終わったら戻り値用のジオメトリを作成する
		if (lMeshCache) {
			const int lSubMeshCount = lMeshCache->GetSubMeshCount();
			//サブメッシュの個数分行う
			for (int lIndex = 0; lIndex < lSubMeshCount; lIndex++) {

				int lOffset = lMeshCache->mSubMeshes[lIndex]->IndexOffset;

				//モデルインスタンス作成
				FBXModelData* md = new FBXModelData();

				//マテリアル情報の取得
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


				//ジオメトリバッファ作成
				auto loopCount = lMeshCache->mSubMeshes[lIndex]->TriangleCount * 3;
				md->IndexLength = loopCount;
				auto count = lMeshCache->lVertices.Size();
				indexCount = 0;

				auto vertexCount = lMeshCache->lVertices.Size();
				md->PosLength = vertexCount / 4;
				md->Data.resize(md->PosLength);
				for (int i = 0; i < vertexCount / 4; ++i) {

					//頂点のコピー
					memcpy(&md->Data[i].Pos, &lMeshCache->lVertices.GetArray()[i * 4], sizeof(SimpleVector4));


					if (!lHasDeformation)
						memcpy(&md->Data[i].Normal, &lMeshCache->lNormals.GetArray()[i * 3], sizeof(SimpleVector3));




					if (lMeshCache->lUVs.Size()>0) {
						memcpy(&md->Data[i].UV, &lMeshCache->lUVs.GetArray()[i * 2], sizeof(SimpleVector2));

					}
				}

				//インデックス座標を取得
				md->Index = lMeshCache->lIndices.GetArray() + lOffset;

				//頂点座標とインデックス座標をもとに法線の計算
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


		//ジオメトリのオフセットを取得
		GetGeometry(lGeometryOffset,pNode);
		lGlobalOffPosition = lGlobalPosition*lGeometryOffset;
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



	//	pNode->LclRotation.Set(FbxDouble3(0, 0, 45));

		start = timeGetTime();
		//ノードのメッシュを取得
		FbxMesh* lMesh = pNode->GetMesh();
		
		//頂点数を取得
		const int lVertexCount = lMesh->GetControlPointsCount();

		//頂点が一つもない場合は処理しない
		if (lVertexCount == 0) {
			return;
		}

		//VBO取得
		MYVBO* lMeshCache = static_cast<MYVBO*>(lMesh->GetUserDataPtr());
		//ボーンがあるならボーンを取得
		const bool lHasVertexCache = lMesh->GetDeformerCount(FbxDeformer::eVertexCache) && (static_cast<FbxVertexCacheDeformer*>(lMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->Active.Get();
		const bool lHasShape = lMesh->GetShapeCount() > 0;
		const bool lHasSkin = lMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
		const bool lHasDeformation = lHasVertexCache || lHasShape || lHasSkin;

		//頂点バッファ
		FbxVector4* lVertexArray = nullptr;
		//法線バッファ
		FbxVector4* lNormalArray = nullptr;
		//VBO未作成 or ボーンがない場合、そのままコピー
		if (!lMeshCache || lHasDeformation) {

			lVertexArray = new FbxVector4[lVertexCount];
			lNormalArray = new FbxVector4[lVertexCount];
			//頂点バッファのコピー(取得)
			memcpy(lVertexArray, lMesh->GetControlPoints(), lVertexCount * sizeof(FbxVector4));
			memcpy(lNormalArray, lMesh->GetElementNormal(0), lVertexCount * sizeof(FbxVector4));
			

		}

		//アニメーション関係の処理
		//デフォーマーがあった場合、アニメーション処理
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
				//更新した頂点の反映
				lMeshCache->UpdateVertexPosition(lMesh, lVertexArray);

		}

		

		std::vector<FBXModelData*>mdv;
		//アニメーションなどすべての処理が終わったら戻り値用のジオメトリを作成する
		if (lMeshCache) {
			const int lSubMeshCount = lMeshCache->GetSubMeshCount();
			//サブメッシュの個数分行う
			for (int lIndex = 0; lIndex < lSubMeshCount; lIndex++) {

				int lOffset = lMeshCache->mSubMeshes[lIndex]->IndexOffset;

				//モデルインスタンス作成
				FBXModelData* md = new FBXModelData();
				
				//マテリアル情報の取得
				const FbxSurfaceMaterial*lMaterial = pNode->GetMaterial(lIndex);
				if (lMaterial) {

				}


				//ジオメトリバッファ作成
				auto loopCount = lMeshCache->mSubMeshes[lIndex]->TriangleCount * 3;
				md->IndexLength = loopCount;
				auto count = lMeshCache->lVertices.Size();
				indexCount = 0;

				auto vertexCount = lMeshCache->lVertices.Size();
				md->PosLength = vertexCount / 4;
				md->Data.resize(md->PosLength);
				for (int i = 0; i < vertexCount/4; ++i) {
					
					//頂点のコピー
					memcpy(&md->Data[i].Pos, &lMeshCache->lVertices.GetArray()[i*4],sizeof(SimpleVector4));

						
					if(!lHasDeformation)
					memcpy(&md->Data[i].Normal, &lMeshCache->lNormals.GetArray()[i * 3], sizeof(SimpleVector3));




					if (lMeshCache->lUVs.Size()>0) {
						memcpy(&md->Data[i].UV, &lMeshCache->lUVs.GetArray()[i * 2], sizeof(SimpleVector2));

					}
				}

				//インデックス座標を取得
				md->Index = lMeshCache->lIndices.GetArray() + lOffset;

				//頂点座標とインデックス座標をもとに法線の計算
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
				//ワールド座標系に変換する
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

	//読み込みに成功した場合
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
					//ウェイトの計算
					lWeight = (lWeight / lEndWeight) * 100;
					//出力頂点データを初期化
					memcpy(lDstVertexArray, lSrcVertexArray, lVertexCount * sizeof(FbxVector4));
					for (int j = 0; j < lVertexCount; ++j) {
						FbxVector4 lInfluence = (lEndShape->GetControlPoints()[j] - lSrcVertexArray[j])*lWeight*0.01;
						lDstVertexArray[j] += lInfluence;
					}
				}
				else if (lStartShape&&lEndShape) {
					double lStartWeight = lFullWeights[lStartIndex];
					double lEndWeight = lFullWeights[lEndIndex];
					//ウェイトの計算
					lWeight = (lWeight / lEndWeight) * 100;
					//出力頂点データを初期化
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

		//2つのスキニングのウェイトを合成する
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

			//やることリスト/////////////////////////////////////////
			//ローカル変数をメンバ変数化する
			//FBXAMATRIXの計算を最適化する
			//解放処理を実装する


			for (int k = 0; k < lVertexIndexCount; ++k) {
				int lIndex = lCluster->GetControlPointIndices()[k];

				//小さなウェイトを無視して正規化する
				if (lIndex >= lVertexCount)continue;

				double lWeight = lCluster->GetControlPointWeights()[k];

				if (lWeight == 0.0) {
					continue;
				}
				
//				lInfluence = lVertexTransformMatrix;

				

				memcpy(&lInfluence, &lVertexTransformMatrix, sizeof(FbxAMatrix));

				MatrixScale(lInfluence.Buffer()->Buffer(), lWeight);


				if (lClusterMode == FbxCluster::eAdditive) {
					//でフォーマーを行列に合成する
					MatrixAddToDiagonal(lInfluence, 1.0 - lWeight);
					lClusterDeformation[lIndex] = lInfluence * lClusterDeformation[lIndex];
					//クラスタのウェイトを1.0にする
					lClusterWeight[lIndex] = 1.0;
				}
				else {
					MatrixAdd(lClusterDeformation[lIndex].Buffer()->Buffer(), lInfluence);
					//正規化したジオメトリかコンピュートジオメトリにウェイトを加算する
					lClusterWeight[lIndex] += lWeight;
				}
			}

		}

	}

	//頂点コピー
	for (int i = 0; i < lVertexCount; ++i)
	{
		//コピー元頂点
		FbxVector4 lSrcVertex = pVertexArray[i];
		//ウェイトの計算結果を含む頂点
		FbxVector4& lDstVertex = pVertexArray[i];
		//ウェイトの取得
		double lWeight = lClusterWeight[i];

		// Deform the vertex if there was at least a link with an influence on the vertex,
		//ウェイトが0でない場合のみ計算
		if (lWeight != 0.0)
		{
			//ベクトルに行列を合成したものを取得する
			lDstVertex = lClusterDeformation[i].MultT(lSrcVertex);
			//正規化する場合、ウェイトで割る
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

	//法線計算
	

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

	//全てのクラスタ
	for (int lSkinIndex = 0; lSkinIndex < lSkinCount; ++lSkinIndex)
	{
		//デフォーマーを取得
		FbxSkin * lSkinDeformer = (FbxSkin *)pMesh->GetDeformer(lSkinIndex, FbxDeformer::eSkin);
		//クラスタ数を取得
		int lClusterCount = lSkinDeformer->GetClusterCount();
		for (int lClusterIndex = 0; lClusterIndex < lClusterCount; ++lClusterIndex)
		{
			//クラスタを取得
			FbxCluster* lCluster = lSkinDeformer->GetCluster(lClusterIndex);
			//クラスタにリンクがセットされていなければ処理しない
			if (!lCluster->GetLink())
				continue;

			//頂点の移動行列
			FbxAMatrix lVertexTransformMatrix;
			ComputeClusterDeformation(pGlobalPosition, pMesh, lCluster, lVertexTransformMatrix, pTime, pPose);

			//クォータニオン取得
			FbxQuaternion lQ = lVertexTransformMatrix.GetQ();
			FbxVector4 lT = lVertexTransformMatrix.GetT();
			FbxDualQuaternion lDualQuaternion(lQ, lT);
			int lVertexIndexCount = lCluster->GetControlPointIndicesCount();
			for (int k = 0; k < lVertexIndexCount; ++k) {
				int lIndex = lCluster->GetControlPointIndices()[k];
				// Sometimes, the mesh can have less points than at the time of the skinning
				// because a smooth operator was active when skinning but has been deactivated during export.
				//頂点数よりも大きい数値が取得できてしまった場合は未処理
				if (lIndex >= lVertexCount)
					continue;


				//ウェイト取得
				double lWeight = lCluster->GetControlPointWeights()[k];
				//ウェイトが0だった場合は処理不要
				if (lWeight == 0.0)
					continue;

				// Compute the influence of the link on the vertex.
				//クォータニオンとウェイトを合成
				FbxDualQuaternion lInfluence = lDualQuaternion * lWeight;
				if (lClusterMode == FbxCluster::eAdditive)
				{
					//クォータニオンをセット
					lDQClusterDeformation[lIndex] = lInfluence;
					//ウェイトを1.0に固定
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
						//頂点を変形する値を加算
						double lSign = lDQClusterDeformation[lIndex].GetFirstQuaternion().DotProduct(lDualQuaternion.GetFirstQuaternion());
						//必ず正数を加算するようにする
						if (lSign >= 0.0)
						{
							lDQClusterDeformation[lIndex] += lInfluence;
						}
						else
						{
							lDQClusterDeformation[lIndex] -= lInfluence;
						}
					}
					//ウェイトを加算
					lClusterWeight[lIndex] += lWeight;
				}

			}
		}
	}

	//頂点数分回す
	for (int i = 0; i < lVertexCount; ++i)
	{
		FbxVector4 lSrcVertex = pVertexArray[i];
		FbxVector4& lDstVertex = pVertexArray[i];
		double lWeightSum = lClusterWeight[i];

		// Deform the vertex if there was at least a link with an influence on the vertex,
		//ウェイトが0なら未処理
		if (lWeightSum != 0.0)
		{
			lDQClusterDeformation[i].Normalize();
			lDstVertex = lDQClusterDeformation[i].Deform(lDstVertex);

			if (lClusterMode == FbxCluster::eNormalize)
			{
				// In the normalized link mode, a vertex is always totally influenced by the links. 
				//参照した頂点にウェイトで割って正規化する
				lDstVertex /= lWeightSum;
			}
			else if (lClusterMode == FbxCluster::eTotalOne)
			{
				// In the total 1 link mode, a vertex can be partially influenced by the links. 
				//影響するところだけ合成
				lSrcVertex *= (1.0 - lWeightSum);
				lDstVertex += lSrcVertex;
			}
		}
	}
	delete[] lDQClusterDeformation;
	delete[] lClusterWeight;

}

//頂点を変換する行列を計算する
void MYFBX::ComputeClusterDeformation(FbxAMatrix& pGlobalPosition,FbxMesh* pMesh,FbxCluster* pCluster,FbxAMatrix& pVertexTransformMatrix,FbxTime pTime,FbxPose* pPose)
{
	FbxCluster::ELinkMode lClusterMode = pCluster->GetLinkMode();
	FbxAMatrix lReferenceGlobalInitPosition;	//初期位置
	FbxAMatrix lReferenceGlobalCurrentPosition;	//現在位置
	FbxAMatrix lAssociateGlobalInitPosition;	//初期位置
	FbxAMatrix lAssociateGlobalCurrentPosition;	//現在位置
	FbxAMatrix lClusterGlobalInitPosition;		//初期位置
	FbxAMatrix lClusterGlobalCurrentPosition;	//現在位置

	FbxAMatrix lReferenceGeometry;
	FbxAMatrix lAssociateGeometry;
	FbxAMatrix lClusterGeometry;

	FbxAMatrix lClusterRelativeInitPosition;
	FbxAMatrix lClusterRelativeCurrentPositionInverse;



	if (lClusterMode == FbxCluster::eAdditive&&pCluster->GetAssociateModel()) {
		pCluster->GetTransformAssociateModelMatrix(lAssociateGlobalInitPosition);

		//モデルの変換
		{
			GetGeometry(lAssociateGeometry, pCluster->GetAssociateModel());
			lAssociateGlobalInitPosition *= lAssociateGeometry;
			GetGlobalPosition(lAssociateGlobalCurrentPosition, pCluster->GetAssociateModel(), pTime, pPose);
		}

		//移動行列の取得
		{
			pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);
			// Multiply lReferenceGlobalInitPosition by Geometric Transformation
			//行列の合成
			GetGeometry(lReferenceGeometry, pMesh->GetNode());
			lReferenceGlobalInitPosition *= lReferenceGeometry;
		}
		//現在位置をグローバルポジションとして設定
		//lReferenceGlobalCurrentPosition = pGlobalPosition;

		//移動行列の取得
		{
			pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
			// 行列合成
			GetGeometry(lClusterGeometry, pCluster->GetLink());
			lClusterGlobalInitPosition *= lClusterGeometry;
			GetGlobalPosition(lClusterGlobalCurrentPosition, pCluster->GetLink(), pTime, pPose);
		}

		// Compute the shift of the link relative to the reference.
		//ModelM-1 * AssoM * AssoGX-1 * LinkGX * LinkM-1*ModelM
		//最終的な行列合成
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

