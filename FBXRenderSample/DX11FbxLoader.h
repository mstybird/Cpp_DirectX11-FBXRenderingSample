#pragma once
//警告非表示

#include<fbxsdk.h>
#include<string>
#include<vector>
#include<D3DX9.h>
#ifdef _DEBUG
#pragma comment(lib,"libfbxsdk-md.lib")
#else
#pragma comment(lib,"libfbxsdk-md.lib")
#endif
#include<unordered_map>
typedef std::tr1::unordered_map<std::string, std::vector<std::string>> TextureName_ut;

#include"DX11FbxResource.h"


#include"FbxVBO.h"
#include"FbxMaterialCache.h"
#include"FbxComputeDeformer.h"

class DX11FbxLoader {

public:
	//解放処理用
	~DX11FbxLoader();
	void FbxInit(std::string vfileName, bool animationLoad = true);
	//アニメーションデータのみ別で読み込む
	//作成途中
	void FbxLoadAnimationFromFile(std::string vfileName);
	void LoadAnimationRecursive(FbxScene* pScene, FbxAnimLayer* pAnimLayer);
	void LoadAnimationRecursive(FbxNode*pNode, FbxAnimLayer*pAnimLayer);
	//FBXクラス解放
	void FbxDestroy();
	//モデルデータを取得する
	std::vector<std::vector<FBXModelData*>>* GetGeometryData2(D3DXVECTOR3 *transPos);
	//アニメーションの切り替え
	void SetAnimation(std::string pName);
	void SetAnimation(int pIndex);

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
	FbxAnimLayer * CurrentAnimLayer;	//FBXアニメーション
	FbxTime FrameTime, Start, Stop, CurrentTime;


	std::unordered_map<std::string, int>AnimData;		//アニメーションスタック
	std::vector<std::vector<FBXModelData*>> Geometry;	//メッシュデータ
	std::vector<FbxNode*>nodemeshes;	//メッシュ情報を持つノードリスト
	std::vector<FbxNode*>nodeAnimeMeshes;	//メッシュ情報を持つノードリスト(アニメーション)
};