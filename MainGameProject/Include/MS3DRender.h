#pragma once
#include<d3dx9.h>
#include<D3DX11.h>
class MSFbxManager;
class DX11RenderResource;
class MSBase3DShader;
class DXDisplay;
#include<memory>
//#include"DXDisplay.h"
//DirectX11で描画するクラス
class MS3DRender {
public:
	//トポロジーの初期化
	MS3DRender();
	~MS3DRender();
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
	void Render(DX11RenderResource&resource);

	//描画画面(ビュー行列と射影行列)を設定
	void SetRenderTarget(const std::weak_ptr<DX11RenderResource>resource);
	void SetRenderTarget(DX11RenderResource& resource);
	void SetRenderTarget(const DXDisplay& aDisplay);

	void GetDisplay(DXDisplay&aOutDisplay);

	//シェーダーの登録
	void SetShader(MSBase3DShader*pShader);
protected:

	//レンダリングに使用するシェーダー

	MSBase3DShader* shader;
	std::shared_ptr<DXDisplay>display;
	const D3D_PRIMITIVE_TOPOLOGY mPrimitiveTopology;

	 static ID3D11Device*sDevice;				//DirectX11デバイス
	 static ID3D11DeviceContext*sDeviceContext;	//DirectX11デバイスコンテキスト
	 static ID3D11RenderTargetView*sRenderTargetView;
	 static ID3D11DepthStencilView*sDepthStencilView;

};


