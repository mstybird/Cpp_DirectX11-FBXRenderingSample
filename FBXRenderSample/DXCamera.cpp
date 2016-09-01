#include "DXCamera.h"

void DXCamera::SetEyeT(float pX, float pY, float pZ)
{
	mEyePosition.x = pX;
	mEyePosition.y = pY;
	mEyePosition.z = pZ;
}

void DXCamera::SetEyeT(D3DXVECTOR3 pPosition)
{
}

void DXCamera::SetLookT(float pX, float pY, float pZ)
{
	mLookPosition.x = pX;
	mLookPosition.y = pY;
	mLookPosition.z = pZ;
}

void DXCamera::SetLookT(D3DXVECTOR3 pLookAt)
{
}

void DXCamera::SetUpV(float pX, float pY, float pZ)
{
	mUpVector.x = pX;
	mUpVector.y = pY;
	mUpVector.z = pZ;
}

void DXCamera::SetUpV(D3DXVECTOR3 pVector)
{
}

D3DXMATRIX * DXCamera::GetMatrix()
{
	D3DXMatrixLookAtLH(&mMatrix, &mEyePosition, &mLookPosition, &mUpVector);
	return &mMatrix;
}
