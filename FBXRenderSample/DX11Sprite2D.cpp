#include "DX11Sprite2D.h"



void DX11SpriteResource::SetTexture(const DX11TextureManager & pTextureManager, const int pID)
{
	pTextureManager.Load(mTexture, pID);
}

void DX11SpriteResource::SetPosition(const DXVector2 & pPosition)
{
	mPosition = pPosition;
}

void DX11SpriteResource::SetPivot(const DXVector2 & pPivot)
{
	mPivot = pPivot;
}

void DX11SpriteResource::SetSize(const DXVector2 & pSize)
{
	mSize = pSize;
}

void DX11SpriteResource::SetScale(const DXVector2 & pScale)
{
	mScale = pScale;
}

const DXVector2 & DX11SpriteResource::GetPosition() const
{
	return mPosition;
}

const DXVector2 & DX11SpriteResource::GetPivot() const
{
	return mPivot;
}

const DXVector2 & DX11SpriteResource::GetSize() const
{
	return mSize;
}

const DXVector2 & DX11SpriteResource::GetScale() const
{
	return mScale;
}

ID3D11Device*DX11Sprite2D::sDevice;
ID3D11DeviceContext*DX11Sprite2D::sDeviceContext;
ID3D11RenderTargetView*DX11Sprite2D::sRenderTargetView;
ID3D11DepthStencilView*DX11Sprite2D::sDepthStencilView;

void DX11Sprite2D::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, ID3D11RenderTargetView * pRenderTargetView, ID3D11DepthStencilView * pDepthStencilView)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
	sRenderTargetView = pRenderTargetView;
	sDepthStencilView = pDepthStencilView;
}

void DX11Sprite2D::Render(DX11SpriteResource&pSprite)
{

}

void DX11Sprite2D::SetShader(DX11BaseSprite * pShader)
{
	shader = pShader;
}
