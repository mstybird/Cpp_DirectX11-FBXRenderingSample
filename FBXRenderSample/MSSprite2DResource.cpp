#include "MSSprite2DResource.h"
#include"DX11Resrouce.h"

#include"MSBaseSpriteShader.h"
#include"DX11Texture.h"
#include<cassert>
ID3D11Device* MSSprite2DResource::sDevice{ nullptr };
MSSprite2DResource::MSSprite2DResource():
	mVertexBuffer{ nullptr },
	mPosition{std::make_shared<DXVector2>()},
	mPivot{ std::make_shared<DXVector2>() },
	mSize{ std::make_shared<DXVector2>() },
	mScale{ std::make_shared<DXVector2>() },
	mMatrix{ std::make_shared<DXMatrix>() }
{
}
void MSSprite2DResource::sInitialize(ID3D11Device * pDevice)
{
	sDevice = pDevice;
}
void MSSprite2DResource::SetTexture(const std::weak_ptr<DX11TextureManager>& pTextureManager, const int pID)
{
	pTextureManager.lock()->Load(mTexture, pID);
}

std::weak_ptr<DXTexture> MSSprite2DResource::GetTexture()
{
	return mTexture;
}

void MSSprite2DResource::SetPosition(const DXVector2 & pPosition)
{
	*mPosition = pPosition;
}

void MSSprite2DResource::SetPivot(const DXVector2 & pPivot)
{
	*mPivot = pPivot;
}

void MSSprite2DResource::SetSize(const DXVector2 & pSize)
{
	*mSize = pSize;
	SAFE_RELEASE(mVertexBuffer);

	MSBase2DSpriteShader::SpriteVertex vertices[] = {
		{
			{ 0,0,0 },{ 0,0 }
		},
		{
			{ mSize->x,0,0 },{ 1,0 } 
		},
		{
			{ 0,mSize->y,0 },{ 0,1 }
		},
		{
			{ mSize->x,mSize->y,0 },{ 1,1 }
		}
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MSBase2DSpriteShader::SpriteVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;

	if (FAILED(sDevice->CreateBuffer(&bd, &InitData, &mVertexBuffer))) {
		assert(0);
	};
	
}

void MSSprite2DResource::SetScale(const DXVector2 & pScale)
{
	*mScale = pScale;
}

const std::weak_ptr<DXVector2>  MSSprite2DResource::GetPosition() const
{
	return mPosition;
}

const std::weak_ptr<DXVector2>  MSSprite2DResource::GetPivot() const
{
	return mPivot;
}

const std::weak_ptr<DXVector2>  MSSprite2DResource::GetSize() const
{
	return mSize;
}

const std::weak_ptr<DXVector2>  MSSprite2DResource::GetScale() const
{
	return mScale;
}

const std::weak_ptr<DXMatrix> MSSprite2DResource::GetMatrix()
{
	DXMatrix lTrans, lScale;
	
	lTrans.Translation(*mPosition);
	lScale.Scaling({ 1,1 });
	
	*mMatrix = /*lScale **/ lTrans;
	return mMatrix;
}

