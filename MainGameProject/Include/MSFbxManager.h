#pragma once

#include<fbxsdk.h>
#include<D3DX11.h>
#include<string>
#include<vector>
#include<memory>
#include<unordered_map>
/*
	モデルデータとしての機能のみ
	描画に必要な座標などの行列は一切残さない
*/
struct FBXMesh;
class DX11FbxLoader;
class DXVector3;
class MSCollisionSphere;
class DX11RenderResource;
class MSFbxObject;

struct MeshBuffer {
	std::vector<std::vector<ID3D11Buffer*>>mVertexBuffer;
	std::vector<std::vector<ID3D11Buffer*>>mIndexBuffer;
	std::vector<std::vector<uint32_t>>mIndexLength;
	//コリジョンデータ
	std::vector<std::vector<MSCollisionSphere>>mCollisions;
	std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>>mMeshData;//メッシュデータ
	~MeshBuffer();
};

class MSFbxManager {
public:
	friend class MSFbxObject;
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
	//メッシュ情報をキャッシュする
	void RegisterMesh(FbxTime& mCurrentFrame, const int pAnimationIndex);

	void LoadAnimationFromFile(std::string vfileName);
	std::string GetFileName();
	//メッシュの更新(毎フレーム必要)
	bool Update(FbxTime& mCurrentFrame, const int pAnimationIndex);

	//解放処理
	void Release();

	//現在のメッシュを使ってコリジョン作成
	void CreateCollisionSphere(FbxTime& mCurrentFrame, const int pAnimationIndex);
	//作成済みのコリジョンをリソースに登録する
	//void RegisterCollision(const std::shared_ptr<DX11RenderResource>&pResource);

	std::vector<std::shared_ptr<FBXMesh>>*GetMeshData(int aAnimation, FbxTime aTime);
	std::vector<std::vector<ID3D11Buffer*>>*GetVertexBuffer(int aAnimation,FbxTime aTime);
	std::vector<std::vector<ID3D11Buffer*>>*GetIndexBuffer(int aAnimation, FbxTime aTime);
	std::vector<std::vector<uint32_t>>*GetIndexBufferCount(int aAnimation, FbxTime aTime);
	std::vector<std::vector<MSCollisionSphere>>*GetCollisionSphere(int aAnimation, FbxTime aTime);

	//これらのメソッドはムーブするので使用後、このクラスの各インスタンスは空になる
	//キャッシュするため不要となる
	//std::vector<std::shared_ptr<FBXMesh>> MoveMeshData();
	//std::vector<std::vector<ID3D11Buffer*>>MoveVertexBuffer();
	//std::vector<std::vector<ID3D11Buffer*>>MoveIndexBuffer();

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

	//アニメーションとアニメーションタイムごとのバッファ
	std::unordered_map<int,std::unordered_map<LONG64, MeshBuffer>> mMeshBuffer;
	std::vector<std::vector<int>>mMeshVertexCount;
	std::vector<std::vector<int>>mMeshIndexCount;
	//std::vector<std::vector<D3D11_BUFFER_DESC>>mMeshVBDesc;
	//std::vector<std::vector<D3D11_BUFFER_DESC>>mMeshIBDesc;
	//D3D11_BUFFER_DESC mVBDesc;
	//D3D11_BUFFER_DESC mIBDesc;

	




	bool mAnimationFlag;	//アニメーションする場合はtrue

private:
	//static変数
	//アプリに一つ
	static ID3D11Device*sDevice;				//DirectX11デバイス
	static ID3D11DeviceContext*sDeviceContext;	//DirectX11デバイスコンテキスト
};

