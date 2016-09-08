#pragma once
#include<memory>
#include"DX11TextureManager.h"
#include"DXVector2.h"
#include"DX11BaseShader.h"
//2D描画用
using std::weak_ptr;
using std::unique_ptr;
class DX11BaseSprite;
class DX11SpriteResource {
public:
	DX11SpriteResource() {};
	//描画するテクスチャの設定
	void SetTexture(const DX11TextureManager&pTextureManager, const int pID);
	void SetPosition(const DXVector2&pPosition);
	void SetPivot(const DXVector2&pPivot);
	void SetSize(const DXVector2&pSize);
	void SetScale(const DXVector2&pScale);
	const DXVector2& GetPosition()const;
	const DXVector2& GetPivot()const;
	const DXVector2& GetSize()const;
	const DXVector2& GetScale()const;
private:
	//描画するテクスチャ
	weak_ptr<DXTexture> mTexture;

	DXVector2 mPosition;	//描画位置
	DXVector2 mPivot;		//中心となる位置
	DXVector2 mSize;		//描画サイズ
	DXVector2 mScale;		//拡大率
};

//2Dスプライト
class DX11Sprite2D {
public:
	//DirectX11デバイスの登録
	static void Initialize(
		ID3D11Device*pDevice,
		ID3D11DeviceContext*pDeviceContext,
		ID3D11RenderTargetView*pRenderTargetView,
		ID3D11DepthStencilView*pDepthStencilView
		);

	void Render(DX11SpriteResource&pSprite);
	//シェーダーの登録
	void SetShader(DX11BaseSprite*pShader);

private:
	DX11BaseSprite*shader;
	static ID3D11Device*sDevice;
	static ID3D11DeviceContext*sDeviceContext;
	static ID3D11RenderTargetView*sRenderTargetView;
	static ID3D11DepthStencilView*sDepthStencilView;
};