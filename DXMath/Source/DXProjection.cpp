#include "DXProjection.h"
#include"DXMatrix.h"
#include"DXMath.hh"
float DXProjection::mAspect;
DXProjection::DXProjection():
	mAngle{45},
	mNear{0.0f},
	mFar{100.0f},
	mMatrix{std::make_shared<DXMatrix>()}
{
}
DXProjection::~DXProjection()
{
}
void DXProjection::SetAspect(float pWidth, float pHeight)
{
	mAspect = pWidth / pHeight;
}

void DXProjection::SetProjection(float pAngle, float pNearPlane, float pFarPlane)
{
	mAngle = pAngle;
	mNear = pNearPlane;
	mFar = pFarPlane;
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

void DXProjection::Clone(DXProjection & pOutClone)
{
	pOutClone.mAngle = mAngle;
	pOutClone.mAspect = mAspect;
	pOutClone.mFar = mFar;
	pOutClone.mNear = mNear;
}

std::weak_ptr<DXMatrix> DXProjection::GetMatrix()
{
	D3DXMatrixPerspectiveFovLH(
		mMatrix.get(),
		D3DXToRadian(mAngle),
		mAspect, mNear, mFar);
	return mMatrix;
}
