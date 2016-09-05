#include"DX11Texture.h"
ID3D11Device * DXTexture::mDevice{ nullptr };
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

void DXTexture::Create(const std::string & pFileName)
{
	//サンプラの作成
	D3D11_SAMPLER_DESC lSamDesc;
	ZeroMemory(&lSamDesc, sizeof(lSamDesc));
	lSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	lSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	lSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	lSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	mDevice->CreateSamplerState(&lSamDesc, &mSampleLinear);

	//テクスチャの作成
	if (SUCCEEDED(D3DX11CreateShaderResourceViewFromFile(mDevice, pFileName.c_str(), nullptr, nullptr, &mTexture, nullptr))) {
		mIsTexture = true;
	}
	else {
		mIsTexture = false;
	}

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


