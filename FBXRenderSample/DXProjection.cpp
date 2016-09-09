#include "DXProjection.h"
#include"DXMatrix.h"
#include"DX11Resrouce.h"
float DXProjection::mAspect;
DXProjection::DXProjection():
	mAngle{45},
	mNear{0.0f},
	mFar{100.0f},
	mMatrix{new DXMatrix}
{
}
DXProjection::~DXProjection()
{
	SAFE_DELETE(mMatrix);
}
void DXProjection::SetAspect(float pWidth, float pHeight)
{
	mAspect = pWidth / pHeight;
}

void DXProjection::SetViewAngle(float pAngle)
{
	mAngle = pAngle;
}

void DXProjection::SetPlaneNear(float pNearPlane)
{
	mNear = pNearPlane;
}

void DXProjection::SetPlaneFar(float pFarPlane)
{
	mFar = pFarPlane;
}

void DXProjection::AddViewAngle(float pAngle)
{
	mAngle += pAngle;
}

void DXProjection::AddPlaneNear(float pNearPlane)
{
	mNear += pNearPlane;
}

void DXProjection::AddPlaneFar(float pFarPlane)
{
	mFar = pFarPlane;
}

DXMatrix * DXProjection::GetMatrix()
{
	D3DXMatrixPerspectiveFovLH(
		mMatrix,
		D3DXToRadian(mAngle),
		mAspect, mNear, mFar);
	return mMatrix;
}
