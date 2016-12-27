#include "MSLayout.h"
#include<DX11Resrouce.h>

ID3D11Device* MSView::sDevice{};
ID3D11DeviceContext* MSView::sDeviceContext{};
IDXGISwapChain* MSView::sSwapChain{};

MSLayout::MSLayout()
{
}

MSView::MSView():
	mRTV{nullptr},
	mDSV{nullptr},
	mDSTexture{nullptr}
{
	mDepthDesc.Width = 640U;
	mDepthDesc.Height = 480U;
	mDepthDesc.MipLevels = 1;
	mDepthDesc.ArraySize = 1;
	mDepthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	mDepthDesc.SampleDesc.Count = 4U;
	mDepthDesc.SampleDesc.Quality = 0U;
	mDepthDesc.Usage = D3D11_USAGE_DEFAULT;
	mDepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	mDepthDesc.CPUAccessFlags = 0;
	mDepthDesc.MiscFlags = 0;
}

MSView::~MSView()
{
	SAFE_RELEASE(mRTV);
	SAFE_RELEASE(mDSV);
	SAFE_RELEASE(mDSTexture);
}

void MSView::Initialize(ID3D11Device * aDevice, ID3D11DeviceContext * aDeviceContext, IDXGISwapChain* aSwapChain)
{
	sDevice = aDevice;
	sDeviceContext = aDeviceContext;
	sSwapChain = aSwapChain;
}

void MSView::CreateRTV()
{
	SAFE_RELEASE(mRTV);
	ID3D11Texture2D *lBackBuffer_Tex;
	sSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&lBackBuffer_Tex);
	sDevice->CreateRenderTargetView(lBackBuffer_Tex, nullptr, &mRTV);
	SAFE_RELEASE(lBackBuffer_Tex);
}

void MSView::CreateDSV()
{
	sDevice->CreateTexture2D(&mDepthDesc, nullptr, &mDSTexture);
	sDevice->CreateDepthStencilView(mDSTexture, nullptr, &mDSV);
}

ID3D11RenderTargetView ** MSView::GetRTV()
{
	return &mRTV;
}

ID3D11DepthStencilView ** MSView::GetDSV()
{
	return &mDSV;
}

void MSView::SetSize(uint32_t aWidth, uint32_t aHeight)
{
	mDepthDesc.Width = aWidth;
	mDepthDesc.Height = aHeight;
}

void MSView::SetSampleRate(uint32_t aCount, uint32_t aQuality)
{
	mDepthDesc.SampleDesc.Count = aCount;
	mDepthDesc.SampleDesc.Quality = aQuality;
}

