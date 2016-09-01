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

DXWorld::DXWorld():
	mPosition{0.0f,0.0f,0.0f},
	mRotationCenter{0.0f,0.0f,0.0f},
	mRotationTransed{0.0f,0.0f,0.0f},
	mScale{1.0f,1.0f,1.0f}
{
}

void DXWorld::SetT(float pX, float pY, float pZ)
{
	mPosition.x = pX;
	mPosition.y = pY;
	mPosition.z = pZ;
}

void DXWorld::SetT(D3DXVECTOR3 pPosition)
{
	mPosition = pPosition;
}

void DXWorld::SetRC(float pX, float pY, float pZ)
{
	mRotationCenter.x = pX;
	mRotationCenter.y = pY;
	mRotationCenter.z = pZ;
}

void DXWorld::SetRC(D3DXVECTOR3 pRotation)
{
	mRotationCenter = pRotation;
}

void DXWorld::SetRT(float pX, float pY, float pZ)
{
	mRotationTransed.x = pX;
	mRotationTransed.y = pY;
	mRotationTransed.z = pZ;
}

void DXWorld::SetRT(D3DXVECTOR3 pRotation)
{
	mRotationTransed = pRotation;
}

void DXWorld::SetS(float pX, float pY, float pZ)
{
	mScale.x = pX;
	mScale.y = pY;
	mScale.z = pZ;
}

void DXWorld::SetS(D3DXVECTOR3 pScaling)
{
	mScale = pScaling;
}
