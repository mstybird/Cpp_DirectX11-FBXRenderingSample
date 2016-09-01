#include "DXProjection.h"

float DXProjection::mAspect;
DXProjection::DXProjection():
	mAngle{45},
	mNear{0.0f},
	mFar{100.0f}
{
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
}

void DXProjection::AddPlaneNear(float pNearPlane)
{
}

void DXProjection::AddPlaneFar(float pFarPlane)
{
}

D3DXMATRIX * DXProjection::GetMatrix()
{
	D3DXMatrixPerspectiveFovLH(
		&mMatrix,
		D3DXToRadian(mAngle),
		mAspect, mNear, mFar);
	return &mMatrix;
}
