#pragma once
#include<D3DX11.h>
#include<cstdint>
//ビュー情報
/*
	RTV,DSV関係のみ
*/
class MSView {
public:
	MSView();
	~MSView();

	static void Initialize(ID3D11Device*aDevice, ID3D11DeviceContext*aDeviceContext, IDXGISwapChain* aSwapChain);
	void CreateRTV();
	void CreateDSV();

	ID3D11RenderTargetView** GetRTV();
	ID3D11DepthStencilView** GetDSV();
	void SetSize(uint32_t aWidth, uint32_t aHeight);
	void SetSampleRate(uint32_t aCount, uint32_t Quality);

private:
	ID3D11RenderTargetView* mRTV;
	ID3D11DepthStencilView* mDSV;
	ID3D11Texture2D* mDSTexture;

	static ID3D11Device* sDevice;
	static ID3D11DeviceContext* sDeviceContext;
	static IDXGISwapChain* sSwapChain;

	D3D11_TEXTURE2D_DESC mDepthDesc;

};

//レイアウト情報

class MSLayout {
public:

	MSLayout();
	~MSLayout();

	static void Initialize();

	ID3D11RasterizerState* m_pRasterizerState;
	D3D11_VIEWPORT mViewPort;
};