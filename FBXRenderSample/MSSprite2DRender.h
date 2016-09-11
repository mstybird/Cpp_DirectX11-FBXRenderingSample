#pragma once
//2Dスプライト
#include<d3dx11.h>
#include<memory>
class MSBaseSpriteShader;
class MSSpriteBaseResource;
class DX11RenderResource;
class DXDisplay;


class MSSpriteBaseRender abstract{
public:
	MSSpriteBaseRender() :mPrimitiveTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP } {

	}
	//DirectX11デバイスの登録
	static void Initialize(
		ID3D11Device*pDevice,
		ID3D11DeviceContext*pDeviceContext,
		ID3D11RenderTargetView*pRenderTargetView,
		ID3D11DepthStencilView*pDepthStencilView
	);

	virtual void Render(const std::weak_ptr<MSSpriteBaseResource>&pSprite) = 0;
	//シェーダーの登録
	void SetShader(const std::shared_ptr<MSBaseSpriteShader>&pShader);
protected:

	std::weak_ptr<MSBaseSpriteShader>shader;
	//	DX11BaseSprite*shader;
	const D3D_PRIMITIVE_TOPOLOGY mPrimitiveTopology;
	static ID3D11Device*sDevice;
	static ID3D11DeviceContext*sDeviceContext;
	static ID3D11RenderTargetView*sRenderTargetView;
	static ID3D11DepthStencilView*sDepthStencilView;
};

//2Dスプライトレンダラー
class MSSprite2DRender:public MSSpriteBaseRender {
public:
	void Render(const std::weak_ptr<MSSpriteBaseResource>&pSprite);
	//ビューポートの設定
	void SetViewPort(D3D11_VIEWPORT*pViewPort);
	//描画先サイズ
	D3D11_VIEWPORT*mViewPort;

};

//3Dスプライトレンダラー
class MSSprite3DRender:public MSSpriteBaseRender {
public:
	MSSprite3DRender();
	void SetRenderTarget(const std::weak_ptr<DX11RenderResource>resource);
	void Render(const std::weak_ptr<MSSpriteBaseResource>&pSprite);
private:
	bool mBillBoardFlag;
	std::shared_ptr<DXDisplay>display;

};