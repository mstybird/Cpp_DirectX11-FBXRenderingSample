#pragma once

#include<D3DX11.h>
#include<string>
#include<vector>
#include<memory>
/*
	モデルデータとしての機能のみ
	描画に必要な座標などの行列は一切残さない
*/
struct FBXMesh;
class DX11FbxLoader;
class DXVector3;
class MSCollisionSphere;
class MSFbxManager {
public:
	MSFbxManager();
	~MSFbxManager();

	//デバイスは登録するものが常に同じなのですべてのクラスで共通に使えるようにしておく
	static void InitDevice(
		ID3D11Device*pDevice,
		ID3D11DeviceContext*pDeviceContext
	);

	void Initialize();
	//ファイル読み込み
	void LoadFile(std::string pFileName, bool animationLoad = false);
	void LoadAnimationFromFile(std::string vfileName);

	//メッシュの更新(毎フレーム必要)
	void Update();

	//解放処理
	void Release();
	std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>> &GetMeshData();

	//既にあるメッシュから頂点データとインデックスデータのみ取得する
	void GetGeometryOnly(
		std::vector<std::vector<DXVector3>>*pDstVertexData,
		std::vector<std::vector<DXVector3>>*pDstIndexData
		);
	//現在のメッシュを使ってコリジョン作成
	void CreateCollisionSphere();

	ID3D11Buffer*GetVertexBuffer(int i, int j);
	ID3D11Buffer*GetIndexBuffer(int i, int j);
	unsigned int*GetIndexBufferCount(int i,int j);

private:
	//バッファリソースを確保する
	void CreateBuffer();

	std::string mFileName;
	//モデルごとに記憶
	//FBX読み込み用
	DX11FbxLoader*mLoader;				
	
	//(サブ含む)メッシュの個数
	//ベクタ要素数	=	メッシュ数
	//要素の値		=	サブメッシュ数)
	std::vector<int>mMeshCount;
	std::vector<std::vector<int>>mMeshVertexCount;
	std::vector<std::vector<int>>mMeshIndexCount;
	std::vector<std::vector<D3D11_BUFFER_DESC>>mMeshVBDesc;
	std::vector<std::vector<D3D11_BUFFER_DESC>>mMeshIBDesc;
	std::vector<std::vector<ID3D11Buffer*>>mVertexBuffer;
	std::vector<std::vector<ID3D11Buffer*>>mIndexBuffer;
	D3D11_BUFFER_DESC mVBDesc;
	D3D11_BUFFER_DESC mIBDesc;

	//コリジョンデータ
	std::shared_ptr<std::vector<std::vector<MSCollisionSphere>>>mCollisions;
	

	std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>>mMeshData;//メッシュデータ
	bool mAnimationFlag;	//アニメーションする場合はtrue
private:
	//static変数
	//アプリに一つ
	static ID3D11Device*sDevice;				//DirectX11デバイス
	static ID3D11DeviceContext*sDeviceContext;	//DirectX11デバイスコンテキスト
};

