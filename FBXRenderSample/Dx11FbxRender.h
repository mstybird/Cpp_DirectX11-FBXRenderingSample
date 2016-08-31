#pragma once

#include"DX11FbxLoader.h"
#include<D3D11.h>
class DX11FbxManager {
public:
	void Initialize(
		ID3D11Device*pDevice, 
		ID3D11DeviceContext*pDeviceContext,
		ID3D11VertexShader*pVertexShader,
		ID3D11PixelShader*pPixelShader,
		ID3D11InputLayout*pVertexLayout
		);
	//ファイル読み込み
	void LoadFile(std::string pFileName);
	//メッシュの更新(毎フレーム必要)
	void Update();
	//全てのメッシュに適用するデータを登録
	void SetMatrix(
		D3DXMATRIX matrixW,
		D3DXMATRIX matrixV, 
		D3DXMATRIX matrixP);
	//レンダリング
	void Render();
	//解放処理
	void Release();
private:
	struct FBXCONSTANTBUFFER1
	{
		D3DXMATRIX mW;//ワールド、ビュー、射影の合成変換行列
		D3DXMATRIX mWVP;//ワールド、ビュー、射影の合成変換行列
		D3DXVECTOR4 LightDir{ 1,0,-1,0 };
	};

	struct FBXCONSTANTBUFFER2
	{
		D3DXVECTOR4 Diffuse;
	};

	//アプリに一つ
	ID3D11Device*mDevice;				//DirectX11デバイス
	ID3D11DeviceContext*mDeviceContext;	//DirectX11デバイスコンテキスト
	//シェーダごとに記憶
	ID3D11VertexShader*mVertexShader;	//頂点シェーダー
	ID3D11PixelShader*mPixelShader;		//ピクセルシェーダー
	ID3D11InputLayout*mVertexLayout;	//頂点インプットレイアウト
	ID3D11Buffer*mConstantBuffer1;		//一度のみ適用するバッファ
	ID3D11Buffer*mConstantBuffer2;		//メッシュ毎に適用するバッファ
	//モデルごとに記憶
	DX11FbxLoader*mLoader;				//FBX読み込み用
	ID3D11Buffer***mVertexBuffer;		//頂点バッファ
	ID3D11Buffer***mIndexBuffer;		//インデックスバッファ
	std::vector<std::vector<FBXModelData*>>*mMeshData;//メッシュデータ

};

class DX11FbxRender {

};
