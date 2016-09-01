#pragma once
//DirectX11で描画するクラス
#include<D3DX11.h>
#include"DX11RenderResource.h"
#include"DX11FbxManager.h"
#include"DXDisplay.h"
class DX11BaseShader;
class DX11Render {

public:
	//DirectX11デバイスの登録
	static void Initialize(
		ID3D11Device*pDevice,
		ID3D11DeviceContext*pDeviceContext,
		ID3D11RenderTargetView*sRenderTargetView,
		ID3D11DepthStencilView*sDepthStencilView
		);
	//画面クリア
	static void Clear(D3DXVECTOR4 pColor);
	//FBXモデルのレンダリング
	void Render(DX11FbxManager*fbxManager,DX11RenderResource*resource);
	//ビュー射影行列の登録
	void SetRenderTarget(DX11RenderResource*resource);
	//シェーダーの登録
	void SetShader(DX11BaseShader*pShader);
	//レンダリングに使用するシェーダー
	DX11BaseShader*shader;
	DXDisplay display;
private:
	//どのシェーダを使うか
private:
	 static ID3D11Device*sDevice;				//DirectX11デバイス
	 static ID3D11DeviceContext*sDeviceContext;	//DirectX11デバイスコンテキスト
	 static ID3D11RenderTargetView*sRenderTargetView;
	 static ID3D11DepthStencilView*sDepthStencilView;

};

#include"DX11BaseShader.h"