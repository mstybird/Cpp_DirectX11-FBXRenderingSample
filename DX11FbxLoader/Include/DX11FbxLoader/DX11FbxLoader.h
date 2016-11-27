#pragma once
//警告非表示

#include<fbxsdk.h>

#include<string>
#include<vector>


#include<unordered_map>
#include<memory>
typedef std::tr1::unordered_map<std::string, std::vector<std::string>> TextureName_ut;
#include"DX11FbxResource.h"


#include"FbxVBO.h"
#include"FbxMaterialCache.h"
#include"FbxComputeDeformer.h"

struct FbxDataCache {

	FbxDataCache()
	{
		
	};

	~FbxDataCache() {


		auto DeleteVector = [](auto&aVector) {
			for (auto&v : aVector) {
				FBX_SAFE_DELETE_ARRAY(v);
			}
		};

		DeleteVector(dstVertexArray);
		DeleteVector(vertexArrayLinear);
		DeleteVector(vertexArrayDQ);
		DeleteVector(clusterDeformation);
		DeleteVector(clusterLinearWeight);
		DeleteVector(dQClusterDeformation);
		DeleteVector(clusterDualWeight);
		DeleteVector(vertexArray);


	}

	void FirstReserve(const int aLength) {
		if (dstVertexArray.size() == 0) {
			dstVertexArray.resize(aLength);
		}
		if (vertexArrayLinear.size() == 0) {
			vertexArrayLinear.resize(aLength);
		}
		if (vertexArrayDQ.size() == 0) {
			vertexArrayDQ.resize(aLength);
		}
		if (clusterDeformation.size() == 0) {
			clusterDeformation.resize(aLength);
		}
		if (clusterLinearWeight.size() == 0) {
			clusterLinearWeight.resize(aLength);
		}
		if (dQClusterDeformation.size() == 0) {
			dQClusterDeformation.resize(aLength);
		}
		if (clusterDualWeight.size() == 0) {
			clusterDualWeight.resize(aLength);
		}
		if (vertexArray.size() == 0) {
			vertexArray.resize(aLength);
		}
	}

	std::vector<FbxVector4*>dstVertexArray;
	std::vector<FbxVector4*>vertexArrayLinear;
	std::vector<FbxVector4*>vertexArrayDQ;
	std::vector<FbxAMatrix*>clusterDeformation;

	std::vector<FbxDualQuaternion*>dQClusterDeformation;
	std::vector<double*>clusterLinearWeight;
	std::vector<double*>clusterDualWeight;

	std::vector<FbxVector4*> vertexArray;
};


class MSFbxObject;
class DX11FbxLoader {
	friend class MSFbxObject;
public:
	//解放処理用
	DX11FbxLoader();
	~DX11FbxLoader();
	void FbxInit(std::string vfileName, bool animationLoad = true);
	//アニメーションデータのみ別で読み込む
	//作成途中
	void FbxLoadAnimationFromFile(std::string vfileName);
	void LoadAnimationRecursive(FbxScene* pScene, FbxAnimLayer* pAnimLayer);
	void LoadAnimationRecursive(FbxNode*pNode, FbxAnimLayer*pAnimLayer);
	//FBXクラス解放
	void FbxDestroy();
	//メッシュデータを取得する
	std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>> GetGeometryData2(FbxTime&aCurrentTime);
	void GetMeshCount(std::vector<int>&pCountVector);
	void GetMeshVertexCount(std::vector<std::vector<int>>&pCountVector);
	void GetMeshIndexCount(std::vector<std::vector<int>>&pCountVector);
	//アニメーションの切り替え
	void SetAnimation(std::string pName);
	void SetAnimation(int pIndex);
	int GetDefaultAnimation() {
		return mDefaultAnimation;
	}

	FbxTime GetStartFrame() {
		return Start;
	}
private:
	//FBXファイル読み込み
	void FbxLoadFromFile();	
	//シーンの読み込み(テクスチャ)
	void LoadCacheRecursive(FbxScene* pScene, FbxAnimLayer* pAnimLayer, std::string& pFbxFileName);
	//ノードの読み込み
	void LoadCacheRecursive(FbxNode*pNode, FbxAnimLayer*pAnimLayer);
	//シーンの解放
	void UnLoadCacheRecursive(FbxScene*pScene);
	//ノードの解放
	void UnLoadCacheRecursive(FbxNode*pNode);
	//デフォーマーの初期化
	void PreparePointCacheData(FbxScene*pScene, FbxTime&pCache_Start, FbxTime&pCache_Stop);
	//ポーズの種類を取得
	void FillPoseArray(FbxScene*pScene, FbxArray<FbxPose*>pPoseArray);


	//アニメーションの初期化
	void LoadAnimationData();

	/*********アニメーション計算関係*********/

	void ReadVertexCacheData(FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray);
	void ComputeShapeDeformation(const int pIndex, FbxMesh * pMesh, FbxTime & pTime, FbxAnimLayer * pAnimLayer, FbxVector4 * pVertexArray);
	//スキンデフォーマの計算
	void ComputeSkinDeformation(const int pIndex, FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose);
	void ComputeLinearDeformation(const int pIndex, FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose);
	void ComputeDualQuaternionDeformation(const int pIndex, FbxAMatrix & pGlobalPosition, FbxMesh * pMesh, FbxTime & pTime, FbxVector4 * pVertexArray, FbxPose * pPose);
	void ComputeClusterDeformation(FbxAMatrix& pGlobalPosition, FbxMesh* pMesh, FbxCluster* pCluster, FbxAMatrix& pVertexTransformMatrix, FbxTime pTime, FbxPose* pPose);
	//アニメーション関係
	//アニメーションのセット
	bool SetCurrentAnimStack(int pIndex);
	bool SetCurrentPoseIndex(int pPoseIndex);
	const FbxArray<FbxString*>&GetAnimStackNameArray()const { return AnimStackNameArray; }
	//	void SetSelectNode(FbxNode*pSelectedNode);

	void ReleaseGeometryData();



private:

	//メンバ宣言部分
	std::string fileName;				//fbxファイル名
	std::string fileRelativePath;		//FBXファイルのあるフォルダへの相対パス

	FbxString WindowMessage;	//ウィンドウメッセージ用
	FbxManager * SdkManager;			//FBXマネージャ
	FbxScene * Scene;					//FBXシーン
	FbxScene * SceneAnim;				//FBXシーン(アニメーション用)
	FbxImporter * Importer;			//FBXインポータ
	FbxNode * SelectedNode;			//FBXノード

	//TextureName_ut TextureFileName;

	int PoseIndex;								//ポーズのインデクス
	FbxArray<FbxString*> AnimStackNameArray;	//アニメーション名リスト
	FbxArray<FbxPose*> PoseArray;				//ポーズのリスト
	FbxTime Cache_Start, Cache_Stop;

	//アニメーション情報
	bool EnableAnimation;	//アニメーションが有効かどうか
	FbxAnimLayer * CurrentAnimLayer;	//FBXアニメーション
	FbxTime FrameTime, Start, Stop;
	int mDefaultAnimation;	//デフォルトアニメーション

	std::unordered_map<std::string, int>AnimData;		//アニメーションスタック
	//std::vector<std::vector<FBXModelData*>> Geometry;	//メッシュデータ
	std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>>mMesh;	//サブメッシュを含むメッシュデータ
	std::vector<FbxNode*>nodemeshes;	//メッシュ情報を持つノードリスト
	std::vector<FbxNode*>nodeAnimeMeshes;	//メッシュ情報を持つノードリスト(アニメーション)


	//アニメーション用リソースのキャッシュ
	FbxDataCache mCache;
};