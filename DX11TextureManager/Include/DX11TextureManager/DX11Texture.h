#pragma once
#include<string>
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;
struct ID3D11Device;
struct ID3D11Texture2D;
struct ID3D11BlendState;
class DXTexture {

	void CreateSampler();
	void CreateBlendState(const bool pIsAlpha);
public:
	DXTexture();
	~DXTexture();
	static void Initialize(ID3D11Device*pDevice);
	//テクスチャ作成(デフォルトでは透過させる(α値が有効)
	bool Create(const std::string &pFileName, const bool pIsAlpha = true);
	bool Create(ID3D11Texture2D*&pTexture, const bool pIsAlpha = true);
	//テクスチャの解放
	void Release();

	ID3D11SamplerState* GetSampler();
	ID3D11ShaderResourceView* GetTexture();
	bool GetIsTexture();
	ID3D11SamplerState*mSampleLinear;
	ID3D11ShaderResourceView*mTexture;
	ID3D11BlendState*mBlendState;
	bool mIsTexture;
	static ID3D11Device * mDevice;
};
