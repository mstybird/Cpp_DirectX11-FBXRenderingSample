#include "MSSprite2DResource.h"
#include"DX11Resrouce.h"
#include"DXVector2.h"
#include"DXVector3.h"
#include"DXMatrix.h"
#include"DXCamera.h"
#include"DXProjection.h"
#include"DX11TextureManager.h"
#include"DXDisplay.h"
#include"MSBase2DSpriteShader.h"
#include"DX11Texture.h"
#include<cassert>
ID3D11Device* MSSpriteBaseResource::sDevice{ nullptr };
MSSpriteBaseResource::MSSpriteBaseResource():
	mVertexBuffer{ nullptr },
	mPosition{std::make_shared<DXVector3>(0.0f,0.0f,0.0f)},
	mPivot{ std::make_shared<DXVector2>(0.0f,0.0f) },
	mSize{ std::make_shared<DXVector2>(100.0f,100.0f) },
	mScale{ std::make_shared<DXVector2>(1.0f,1.0f) },
	mMatrix{ std::make_shared<DXMatrix>() }
{
}
void MSSpriteBaseResource::sInitialize(ID3D11Device * pDevice)
{
	sDevice = pDevice;
}
void MSSpriteBaseResource::SetTexture(const std::weak_ptr<DX11TextureManager>& pTextureManager, const int pID)
{
	pTextureManager.lock()->Load(mTexture, pID);
}

std::weak_ptr<DXTexture> MSSpriteBaseResource::GetTexture()
{
	return mTexture;
}

void MSSpriteBaseResource::SetPosition(const DXVector2 & pPosition)
{
	*mPosition = pPosition;
}

void MSSpriteBaseResource::SetPosition(const DXVector3 & pPosition)
{
	*mPosition = pPosition;
}

void MSSpriteBaseResource::SetPivot(const DXVector2 & pPivot)
{
	*mPivot = pPivot;
	CreateBuffer();
}

void MSSpriteBaseResource::SetSize(const DXVector2 & pSize)
{
	*mSize = pSize;
	CreateBuffer();
}

void MSSpriteBaseResource::SetScale(const DXVector2 & pScale)
{
	*mScale = pScale;
}

const std::weak_ptr<DXVector3>  MSSpriteBaseResource::GetPosition() const
{
	return mPosition;
}

const std::weak_ptr<DXVector2>  MSSpriteBaseResource::GetPivot() const
{
	return mPivot;
}

const std::weak_ptr<DXVector2>  MSSpriteBaseResource::GetSize() const
{
	return mSize;
}

const std::weak_ptr<DXVector2>  MSSpriteBaseResource::GetScale() const
{
	return mScale;
}

const std::weak_ptr<DXMatrix> MSSpriteBaseResource::GetMatrix()
{
	DXMatrix lTrans, lScale;
	
	lTrans.Translation(*mPosition);
	lScale.Scaling({ mScale->x,mScale->y });
	
	*mMatrix = lScale * lTrans;
	return mMatrix;
}

std::unique_ptr<DXMatrix> MSSpriteBaseResource::GetMatrixWVP(const std::weak_ptr<DXDisplay> pDisplay)
{
	std::unique_ptr<DXMatrix> lResult = std::make_unique<DXMatrix>();
	*lResult = *GetMatrix().lock() *
		*pDisplay.lock()->GetCamera().lock()->GetMatrix().lock() *
		*pDisplay.lock()->GetProjection().lock()->GetMatrix().lock();
	return std::move(lResult);
}

void MSSpriteBaseResource::CreateBuffer()
{
	SAFE_RELEASE(mVertexBuffer);

	//Pivotの値を使用して中心位置を調整する
	DXVector2 lPivotPos{ mSize->x*mPivot->x,mSize->y*mPivot->y };

	SpriteVertex vertices[4];
	CreatePolygon(vertices);

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SpriteVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;

	if (FAILED(sDevice->CreateBuffer(&bd, &InitData, &mVertexBuffer))) {
		assert(0);
	};

}

void MSSprite2DResource::SetPosition(const DXVector2 & pPosition)
{
	MSSpriteBaseResource::SetPosition(pPosition);
}

void MSSprite2DResource::SetPosition(const DXVector3 & pPosition)
{
	MSSpriteBaseResource::SetPosition({ pPosition.x,pPosition.y });
}

void MSSprite2DResource::CreatePolygon(SpriteVertex pPolygon[4])
{
	//Pivotの値を使用して中心位置を調整する
	DXVector2 lPivotPos{ mSize->x*mPivot->x,mSize->y*mPivot->y };
	pPolygon[0] = {
		{ 0 - lPivotPos.x, 0 - lPivotPos.y }, { 0,0 }
	};
	pPolygon[1] = {
	{ mSize->x - lPivotPos.x,0 - lPivotPos.y },{ 1,0 }
	};
	pPolygon[2] = {
	{ 0 - lPivotPos.x,mSize->y - lPivotPos.y },{ 0,1 }
	};
	pPolygon[3] = {
	{ mSize->x - lPivotPos.x,mSize->y - lPivotPos.y },{ 1,1 }
	};
}

void MSSprite3DResource::CreatePolygon(SpriteVertex pPolygon[4])
{
	//Pivotの値を使用して中心位置を調整する
	DXVector2 lPivotPos{ mSize->x*mPivot->x,mSize->y*mPivot->y };

	//Direct3D空間上ではY軸は上
	pPolygon[0] = {
		{ 0 - lPivotPos.x, mSize->y - lPivotPos.y },{ 0,0 }
	};
	pPolygon[1] = {
		{ mSize->x - lPivotPos.x,mSize->y - lPivotPos.y },{ 1,0 }
	};
	pPolygon[2] = {
		{ 0 - lPivotPos.x,0 - lPivotPos.y },{ 0,1 }
	};
	pPolygon[3] = {
		{ mSize->x - lPivotPos.x,0 - lPivotPos.y },{ 1,1 }
	};
}
