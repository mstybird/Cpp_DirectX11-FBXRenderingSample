#include "MSSprite2DResource.h"
#include"DX11Resrouce.h"
#include"DXMath.hpp"
#include"DX11TextureManager.hpp"
#include"MSBase2DSpriteShader.h"
#include<cassert>
ID3D11Device* MSSpriteBaseResource::sDevice{ nullptr };
MSSpriteBaseResource::MSSpriteBaseResource():
	mVertexBuffer{ nullptr },
	mPosition{std::make_shared<DXVector3>(0.0f,0.0f,0.0f)},
	mPivot{ std::make_shared<DXVector2>(0.0f,0.0f) },
	mSize{ std::make_shared<DXVector2>(100.0f,100.0f) },
	mScale{ std::make_shared<DXVector2>(1.0f,1.0f) },
	mSplitPolygonX{ 0.0f,1.0f },
	mSplitPolygonY{ 0.0f,1.0f },
	mSplitImageX{ 0.0f,1.0f },
	mSplitImageY{ 0.0f,1.0f },
	mMatrix{ std::make_shared<DXMatrix>() },
	mUpdateFlag{false}
{
}
void MSSpriteBaseResource::sInitialize(ID3D11Device * pDevice)
{
	sDevice = pDevice;
}
void MSSpriteBaseResource::SetTexture(DX11TextureManager& pTextureManager, const int pID)
{
	auto lBeforeTexture = mTexture;
	pTextureManager.Load(mTexture, pID);
	if (lBeforeTexture!=mTexture) {
		mUpdateFlag = true;
	}
}

void MSSpriteBaseResource::SetTexture(DXTexture*& pTexture)
{
	auto lBeforeTexture = mTexture;
	mTexture = pTexture;
	if (lBeforeTexture != mTexture) {
		mUpdateFlag = true;
	}
}

DXTexture* MSSpriteBaseResource::GetTexture()
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
	if (*mPivot == pPivot)return;
	*mPivot = pPivot;
	mUpdateFlag = true;
}

void MSSpriteBaseResource::SetSize(const DXVector2 & pSize)
{
	if (*mSize == pSize)return;
	*mSize = pSize;
	mUpdateFlag = true;
}

void MSSpriteBaseResource::SetScale(const DXVector2 & pScale)
{
	*mScale = pScale;
}

void MSSpriteBaseResource::SetSplitSize(const float pLeft, const float pRight, const float pTop, const float pBottom)
{

	if (mSplitPolygonX.x == pLeft&&
		mSplitPolygonX.y == pRight&&
		mSplitPolygonY.x == pTop&&
		mSplitPolygonY.y == pBottom&&
		mSplitImageX.x == pLeft&&
		mSplitImageX.y == pRight&&
		mSplitImageY.x == pTop&&
		mSplitImageY.y == pBottom
		)return;

	mSplitPolygonX.Set(pLeft, pRight);
	mSplitPolygonY.Set(pTop, pBottom);
	mSplitImageX.Set(pLeft, pRight);
	mSplitImageY.Set(pTop, pBottom);
	mUpdateFlag = true;
}

void MSSpriteBaseResource::SetSplitSizeX(const DXVector2 & pSizeX)
{
	if(	mSplitPolygonX == pSizeX &&
		mSplitImageX == pSizeX
	)return;

	mSplitPolygonX = pSizeX;
	mSplitImageX = pSizeX;
	mUpdateFlag = true;
}

void MSSpriteBaseResource::SetSplitSizeY(const DXVector2 & pSizeY)
{

	if (mSplitPolygonY == pSizeY &&
		mSplitImageY == pSizeY
		)return;

	mSplitPolygonY = pSizeY;
	mSplitImageY = pSizeY;

	mUpdateFlag = true;
}

void MSSpriteBaseResource::SetSplitImage(const float pLeft, const float pRight, const float pTop, const float pBottom)
{
	if (
		mSplitImageX.x == pLeft&&
		mSplitImageX.y == pRight&&
		mSplitImageY.x == pTop&&
		mSplitImageY.y == pBottom
		)return;

	mSplitImageX.Set(pLeft, pRight);
	mSplitImageY.Set(pTop, pBottom);
	mUpdateFlag = true;
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
	
	*mMatrix = lScale*lTrans;
	return mMatrix;
}

DXMatrix MSSpriteBaseResource::GetMatrixWVP(DXDisplay& pDisplay)
{
	DXMatrix lResult{};
	lResult = *GetMatrix().lock() *
		*pDisplay.GetCamera().lock()->GetMatrix().lock() *
		*pDisplay.GetProjection().lock()->GetMatrix().lock();
	return std::move(lResult);
}

bool MSSpriteBaseResource::IsUpdateBuffering()
{
	return mUpdateFlag;
}

void MSSpriteBaseResource::CreateBuffer()
{
	if (mUpdateFlag == false)return;
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
	mUpdateFlag = false;
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
		{ 0 - lPivotPos.x, 0 - lPivotPos.y ,0}, { mSplitImageX.x,mSplitImageY.x }	//左上
	};
	pPolygon[1] = {
	{ mSize->x - lPivotPos.x,0 - lPivotPos.y ,0 },{ mSplitImageX.y,mSplitImageY.x }	//右上
	};
	pPolygon[2] = {
	{ 0 - lPivotPos.x,mSize->y - lPivotPos.y ,0 },{ mSplitImageX.x,mSplitImageX.y }	//左下
	};
	pPolygon[3] = {
	{ mSize->x - lPivotPos.x,mSize->y - lPivotPos.y,0 },{ mSplitImageX.y,mSplitImageX.y }	//右下
	};


	//位置が確定
	//位置をずらす
	pPolygon[0].Pos.x += mSize->x*mSplitPolygonX.x;
	pPolygon[0].Pos.y += mSize->y*mSplitPolygonY.x;

	pPolygon[1].Pos.x -= mSize->x*(1.0f - mSplitPolygonX.y);
	pPolygon[1].Pos.y += mSize->y*mSplitPolygonY.x;

	pPolygon[2].Pos.x += mSize->x*mSplitPolygonX.x;
	pPolygon[2].Pos.y -= mSize->y*(1.0f - mSplitPolygonY.y);

	pPolygon[3].Pos.x -= mSize->x*(1.0f - mSplitPolygonX.y);
	pPolygon[3].Pos.y -= mSize->y*(1.0f - mSplitPolygonY.y);
}

void MSSprite3DResource::CreatePolygon(SpriteVertex pPolygon[4])
{
	//Pivotの値を使用して中心位置を調整する
	DXVector2 lPivotPos{ mSize->x*mPivot->x,mSize->y*mPivot->y };

	//Direct3D空間上ではY軸は上
	pPolygon[0] = {
		{ 0 - lPivotPos.x, mSize->y - lPivotPos.y,0 },{ 0,0 }
	};
	pPolygon[1] = {
		{ mSize->x - lPivotPos.x,mSize->y - lPivotPos.y,0 },{ 1,0 }
	};
	pPolygon[2] = {
		{ 0 - lPivotPos.x,0 - lPivotPos.y,0 },{ 0,1 }
	};
	pPolygon[3] = {
		{ mSize->x - lPivotPos.x,0 - lPivotPos.y,0 },{ 1,1 }
	};
}
