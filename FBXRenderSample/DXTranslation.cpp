#include "DXTranslation.h"

D3DXMATRIX * DXWorld::GetMatrix()
{

	D3DXMATRIX lTrans;
	D3DXMATRIX lRotateCenter;
	D3DXMATRIX lRotateTransed;
	D3DXMATRIX lScale;

	D3DXMatrixTranslation(&lTrans, mPosition.x, mPosition.y, mPosition.z);
	D3DXMatrixRotationYawPitchRoll(&lRotateCenter, mRotationCenter.y, mRotationCenter.x, mRotationCenter.z);
	D3DXMatrixRotationYawPitchRoll(&lRotateTransed, mRotationTransed.y, mRotationTransed.x, mRotationTransed.z);
	D3DXMatrixScaling(&lScale, mScale.x, mScale.y, mScale.z);
	mMatrix = lScale*lRotateCenter*lTrans*lRotateTransed;
	return &mMatrix;
}

void DXWorld::SetT(float pX, float pY, float pZ)
{
	mPosition.x = pX;
	mPosition.y = pY;
	mPosition.z = pZ;
}

void DXWorld::SetT(D3DXVECTOR3 pPosition)
{
	
}

void DXWorld::SetRCenter(float pX, float pY, float pZ)
{
}

void DXWorld::SetRCenter(D3DXVECTOR3 pPosition)
{
}

void DXWorld::SetRT(float pX, float pY, float pZ)
{
}

void DXWorld::SetRT(D3DXVECTOR3 pPosition)
{
}

void DXWorld::SetS(float pX, float pY, float pZ)
{
}

void DXWorld::SetS(D3DXVECTOR3 pPosition)
{
}
