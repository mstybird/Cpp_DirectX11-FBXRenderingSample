#pragma once
#include<d3dx9.h>
#include<D3DX11.h>
class DX11FbxManager;
class DX11RenderResource;
class DX11BaseShader;
class DXDisplay;
//#include"DXDisplay.h"
//DirectX11で描画するクラス
class DX11Render {
public:
	//トポロジーの初期化
	DX11Render();
	~DX11Render();
	//DirectX11デバイスの登録
	static void Initialize(
		ID3D11Device*pDevice,
		ID3D11DeviceContext*pDeviceContext,
		ID3D11RenderTargetView*sRenderTargetView,
		ID3D11DepthStencilView*sDepthStencilView
		);
	//画面クリア
	static void Clear(D3DXVECTOR4 pColor);

	//レンダリング
	void Render(DX11FbxManager*fbxManager,DX11RenderResource*resource);

	//描画画面(ビュー行列と射影行列)を設定
	void SetRenderTarget(DX11RenderResource*resource);
	//シェーダーの登録
	void SetShader(DX11BaseShader*pShader);
protected:

	//レンダリングに使用するシェーダー
	DX11BaseShader*shader;
	DXDisplay*display;
	const D3D_PRIMITIVE_TOPOLOGY mPrimitiveTopology;

	 static ID3D11Device*sDevice;				//DirectX11デバイス
	 static ID3D11DeviceContext*sDeviceContext;	//DirectX11デバイスコンテキスト
	 static ID3D11RenderTargetView*sRenderTargetView;
	 static ID3D11DepthStencilView*sDepthStencilView;

};


