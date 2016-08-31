#pragma once
//警告非表示
#pragma warning(disable : 4005)
#include<fbxsdk.h>
#include<string>
#include<vector>
#include<D3DX10.h>
#ifdef _DEBUG
#pragma comment(lib,"libfbxsdk-md.lib")
#else
#pragma comment(lib,"libfbxsdk-md.lib")
#endif
#include<unordered_map>
typedef std::tr1::unordered_map<std::string, std::vector<std::string>> TextureName_ut;



struct ColorChannel {
	ColorChannel() {
		Color[0] = 0.0f;
		Color[1] = 0.0f;
		Color[2] = 0.0f;
		Color[3] = 1.0f;
	}
	FbxString TextureName;
	FbxFloat Color[4];
};

struct SimpleVertex
{
	D3DXVECTOR4 Pos;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 UV;
};


struct FBXModelData {

	std::vector<SimpleVertex> Data;	//ジオメトリデータ

	unsigned int PosLength;
	unsigned int *Index;		//インデックスバッファ生成用
	unsigned int IndexLength;	//インデックス数

	ColorChannel *Emissive;		//エミッシブ
	ColorChannel *Ambient;		//アンビエント
	ColorChannel *Diffuse;		//ディフューズ
	ColorChannel *Specular;		//スペキュラ



};


#include"FbxVBO.h"
#include"FbxMaterialCache.h"
#include"FbxComputeDeformer.h"

class DX11FbxLoader {

public:
	//解放処理用
	~DX11FbxLoader();
	void FbxInit(std::string vfileName);
	//FBXクラス解放
	void FbxDestroy();
	//モデルデータを取得する
	std::vector<std::vector<FBXModelData*>>* GetGeometryData2(D3DXVECTOR3 *transPos);
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
	FbxImporter * Importer;			//FBXインポータ
	FbxAnimLayer * CurrentAnimLayer;	//FBXアニメーション
	FbxNode * SelectedNode;			//FBXノード

	TextureName_ut TextureFileName;

	int PoseIndex;								//ポーズのインデクス
	FbxArray<FbxString*> AnimStackNameArray;	//アニメーション名リスト
	FbxArray<FbxPose*> PoseArray;				//ポーズのリスト

	FbxTime FrameTime, Start, Stop, CurrentTime;
	FbxTime Cache_Start, Cache_Stop;

	FbxUInt indexCount;

	std::vector<std::vector<FBXModelData*>> Geometry;
	std::vector<FbxNode*>nodemeshes;
};