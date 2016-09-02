#include "DXCamera.h"
//#include"DX11RenderResource.h"

void DXCamera::SetEyeT(float pX, float pY, float pZ)
{
	mEyePosition.Set(pX, pY, pZ);
}

void DXCamera::SetEyeT(D3DXVECTOR3 pPosition)
{
	mEyePosition = pPosition;
}

void DXCamera::SetEyeT(DXWorld pEyeTarget)
{
	mEyePosition = pEyeTarget.mPosition;
}

//void DXCamera::SetEyeT(DX11RenderResource pEyeTarget)
//{
//	mEyePosition = pEyeTarget.smWorld.mPosition;
//}

void DXCamera::SetLookT(float pX, float pY, float pZ)
{
	mLookPosition.Set(pX, pY, pZ);
}

void DXCamera::SetLookT(D3DXVECTOR3 pLookAt)
{
	mLookPosition = pLookAt;
}

void DXCamera::SetLookT(DXWorld pEyeTarget)
{
	auto v = *pEyeTarget.GetMatrix();
	mLookPosition.Set(v._41, v._42, v._43);
}

//void DXCamera::SetLookT(DX11RenderResource pLookTarget)
//{
//	mLookPosition = pLookTarget.smWorld.mPosition;
//}

void DXCamera::SetUpV(float pX, float pY, float pZ)
{
	mUpVector.Set(pX, pY, pZ);
}

void DXCamera::SetUpV(D3DXVECTOR3 pVector)
{
	mUpVector = pVector;
}

D3DXMATRIX * DXCamera::GetMatrix()
{
	D3DXMatrixLookAtLH(&mMatrix, &mEyePosition, &mLookPosition, &mUpVector);
	return &mMatrix;
}
