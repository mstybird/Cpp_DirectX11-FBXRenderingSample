#pragma once
#include<D3DX11.h>
#include<string>
class DXTexture {
public:
	DXTexture();
	~DXTexture();
	static void Initialize(ID3D11Device*pDevice);
	void Create(const std::string &pFileName);
	void Release();
	ID3D11SamplerState* GetSampler();
	ID3D11ShaderResourceView* GetTexture();
	bool GetIsTexture();
	ID3D11SamplerState*mSampleLinear;
	ID3D11ShaderResourceView*mTexture;
	bool mIsTexture;
	static ID3D11Device * mDevice;
};
