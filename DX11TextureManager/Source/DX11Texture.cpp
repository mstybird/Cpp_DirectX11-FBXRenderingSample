#include"DX11Texture.h"
#include"TGALoader.hh"
#pragma warning(disable:4005)
#include<D3DX11.h>

#define SAFE_RELEASE(x) if(x){(x)->Release(); (x)=NULL;}


ID3D11Device * DXTexture::mDevice{ nullptr };

void DXTexture::CreateSampler()
{
	//サンプラの作成
	D3D11_SAMPLER_DESC lSamDesc;
	ZeroMemory(&lSamDesc, sizeof(lSamDesc));
	lSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	lSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	lSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	lSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	mDevice->CreateSamplerState(&lSamDesc, &mSampleLinear);
}

//現時点ではアルファ値強制敵に有効中
void DXTexture::CreateBlendState(const bool pIsAlpha)
{
	D3D11_BLEND_DESC lDesc{};
	lDesc.AlphaToCoverageEnable = false;
	lDesc.IndependentBlendEnable = false;
	lDesc.RenderTarget[0].BlendEnable = true;
	lDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	lDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	lDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	lDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	lDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	lDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	lDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	mDevice->CreateBlendState(&lDesc, &mBlendState);

}

DXTexture::DXTexture() :
	mSampleLinear{ nullptr },
	mTexture{ nullptr },
	mIsTexture{ nullptr }
{
}
DXTexture::~DXTexture()
{
	Release();
}
void DXTexture::Initialize(ID3D11Device * pDevice)
{
	mDevice = pDevice;
}

bool DXTexture::Create(const std::string & pFileName, const bool pIsAlpha)
{
	////サンプラの作成
	//D3D11_SAMPLER_DESC lSamDesc;
	//ZeroMemory(&lSamDesc, sizeof(lSamDesc));
	//lSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//lSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//lSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//lSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//mDevice->CreateSamplerState(&lSamDesc, &mSampleLinear);

	//テクスチャの作成
	if (SUCCEEDED(D3DX11CreateShaderResourceViewFromFile(mDevice, pFileName.c_str(), nullptr, nullptr, &mTexture, nullptr))) {
		mIsTexture = true;
		CreateSampler();
		CreateBlendState(pIsAlpha);
	}
	else {
		Comfort::TGAImage tga;
		if (tga.ReadTGA(pFileName.c_str())==true) {
			if (tga.CreateTextureResource2D(mDevice, mTexture)) {
				mIsTexture = true;
				CreateSampler();
				CreateBlendState(pIsAlpha);
			}
		}
		else {
			mIsTexture = false;
		}
	}
	return mIsTexture;
}

bool DXTexture::Create(ID3D11Texture2D*& pTexture, const bool pIsAlpha)
{
	////サンプラの作成
	//D3D11_SAMPLER_DESC lSamDesc;
	//ZeroMemory(&lSamDesc, sizeof(lSamDesc));
	//lSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//lSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//lSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//lSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//mDevice->CreateSamplerState(&lSamDesc, &mSampleLinear);

	//テクスチャの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC lSRVDesc{};
	lSRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	lSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	lSRVDesc.Texture2D.MipLevels = 1;
	if (SUCCEEDED(mDevice->CreateShaderResourceView(pTexture, &lSRVDesc, &mTexture))) {
		mIsTexture = true;
		CreateSampler();
		CreateBlendState(pIsAlpha);
	}
	else {
		mIsTexture = true;
	}
	return mIsTexture;

}

void DXTexture::Release()
{
	if (mSampleLinear != nullptr) {
		mSampleLinear->Release();
		mSampleLinear = nullptr;
	}
	if (mTexture != nullptr) {
		mTexture->Release();
		mTexture = nullptr;
		mIsTexture = false;
	}
	SAFE_RELEASE(mSampleLinear);
	SAFE_RELEASE(mTexture);
}

ID3D11SamplerState * DXTexture::GetSampler()
{
	return mSampleLinear;
}

ID3D11ShaderResourceView * DXTexture::GetTexture()
{
	return mTexture;
}

bool DXTexture::GetIsTexture()
{
	return mIsTexture;
}


