#pragma once
#include"DX11FbxResource.h"
#include"DX11FbxLoader.h"
#include<D3DX11.h>
#include<string>
#include<vector>
/*
	モデルデータとしての機能のみ
	描画に必要な座標などの行列は一切残さない
*/
class DX11FbxManager {
public:
	DX11FbxManager();
	~DX11FbxManager();

	//デバイスは登録するものが常に同じなのですべてのクラスで共通に使えるようにしておく
	static void InitDevice(
		ID3D11Device*pDevice,
		ID3D11DeviceContext*pDeviceContext
	);

	void Initialize();
	//ファイル読み込み
	void LoadFile(std::string pFileName, bool animationLoad = true);
	void LoadAnimationFromFile(std::string vfileName);

	//メッシュの更新(毎フレーム必要)
	void Update();

	//解放処理
	void Release();
	std::vector<FBXMesh*>* GetMeshData();
	ID3D11Buffer*GetVertexBuffer(int i, int j);
	ID3D11Buffer*GetIndexBuffer(int i, int j);
	unsigned int*GetIndexBufferCount(int i,int j);


private:
	//モデルごとに記憶
	DX11FbxLoader*mLoader;				//FBX読み込み用
	ID3D11Buffer***mVertexBuffer;		//頂点バッファ
	ID3D11Buffer***mIndexBuffer;		//インデックスバッファ
	std::vector<FBXMesh*>*mMeshData;//メッシュデータ
private:
	//static変数
	//アプリに一つ
	static ID3D11Device*sDevice;				//DirectX11デバイス
	static ID3D11DeviceContext*sDeviceContext;	//DirectX11デバイスコンテキスト
};

