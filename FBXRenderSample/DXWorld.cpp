#include "DXWorld.h"
#include"DXVector3.h"
#include"DX11Resrouce.h"
D3DXMATRIX * DXWorld::GetMatrix()
{

	D3DXMATRIX lTrans;
	D3DXMATRIX lRotateCenter;
	D3DXMATRIX lRotateTransed;
	D3DXMATRIX lScale;

	D3DXMatrixTranslation(&lTrans, mPosition->x, mPosition->y, mPosition->z);
	D3DXMatrixRotationYawPitchRoll(&lRotateCenter, D3DXToRadian(mRotationCenter->y), D3DXToRadian(mRotationCenter->x), D3DXToRadian(mRotationCenter->z));
	D3DXMatrixRotationYawPitchRoll(&lRotateTransed, D3DXToRadian(mRotationTransed->y), D3DXToRadian(mRotationTransed->x), D3DXToRadian(mRotationTransed->z));
	D3DXMatrixScaling(&lScale, mScale->x, mScale->y, mScale->z);
	*mMatrix = lScale*lRotateCenter*lTrans*lRotateTransed;
	return mMatrix;
}

DXWorld::DXWorld():
	mPosition{new DXVector3},
	mRotationCenter{ new DXVector3 },
	mRotationTransed{ new DXVector3 },
	mScale{ new DXVector3 },
	mMatrix{new D3DXMATRIX}
{
}

DXWorld::~DXWorld()
{
	SAFE_DELETE(mPosition);
	SAFE_DELETE(mRotationCenter);
	SAFE_DELETE(mRotationTransed);
	SAFE_DELETE(mScale);
	SAFE_DELETE(mMatrix);
}

void DXWorld::SetT(float pX, float pY, float pZ)
{
	mPosition->Set(pX, pY, pZ);
}

void DXWorld::SetT(D3DXVECTOR3 pPosition)
{
	*mPosition = pPosition;
}

void DXWorld::AddT(float pX, float pY, float pZ)
{
	mPosition->Add(pX, pY, pZ);
}

void DXWorld::AddT(D3DXVECTOR3 pPosition)
{
	*mPosition += pPosition;
}

void DXWorld::AddT(TYPEMOVE pType, float pSpeed, DXVector3 pDirection)
{

	D3DXMATRIX lRotation;
	switch (pType)
	{
	case DXWorld::TYPE_PARALLEL:
		pDirection *= pSpeed;
		*mPosition += pDirection;
		break;
	case DXWorld::TYPE_ROTATE:
		D3DXMatrixRotationYawPitchRoll(
			&lRotation,
			D3DXToRadian(mRotationCenter->y),
			D3DXToRadian(mRotationCenter->x),
			D3DXToRadian(mRotationCenter->z)
			);
		//ˆÚ“®—ÊŒvŽZ
		D3DXVec3TransformCoord(&pDirection, &pDirection, &lRotation);
		pDirection *= pSpeed;
		*mPosition += pDirection;
		break;
	default:
		break;
	}
}

void DXWorld::AddTRotated(float pSpeed, DXVector3 pDirection)
{

}

void DXWorld::SubT(float pX, float pY, float pZ)
{
	mPosition->Sub(pX, pY, pZ);
}

void DXWorld::SubT(D3DXVECTOR3 pPosition)
{
	*mPosition -= pPosition;
}

void DXWorld::MulT(float pX, float pY, float pZ)
{
	mPosition->Mul(pX, pY, pZ);
}

void DXWorld::MulT(D3DXVECTOR3 pPosition)
{
	*mPosition *= pPosition;

}

void DXWorld::DivT(float pX, float pY, float pZ)
{
	mPosition->Div(pX, pY, pZ);
}

void DXWorld::DivT(D3DXVECTOR3 pPosition)
{
	*mPosition /= pPosition;
}

void DXWorld::SetRC(float pX, float pY, float pZ)
{
	mRotationCenter->Set(pX, pY, pZ);
}

void DXWorld::SetRC(D3DXVECTOR3 pRotation)
{
	*mRotationCenter = pRotation;
}

void DXWorld::AddRC(float pX, float pY, float pZ)
{
	mRotationCenter->Add(pX, pY, pZ);
}

void DXWorld::AddRC(D3DXVECTOR3 pRotation)
{
	*mRotationCenter += pRotation;
}

void DXWorld::SubRC(float pX, float pY, float pZ)
{
	mRotationCenter->Sub(pX, pY, pZ);
}

void DXWorld::SubRC(D3DXVECTOR3 pRotation)
{
	*mRotationCenter -= pRotation;
}

void DXWorld::MulRC(float pX, float pY, float pZ)
{
	mRotationCenter->Mul(pX, pY, pZ);
}

void DXWorld::MulRC(D3DXVECTOR3 pRotation)
{
	*mRotationCenter *= pRotation;
}

void DXWorld::DivRC(float pX, float pY, float pZ)
{
	mRotationCenter->Div(pX, pY, pZ);
}

void DXWorld::DivRC(D3DXVECTOR3 pRotation)
{
	*mRotationCenter /= pRotation;
}

void DXWorld::SetRT(float pX, float pY, float pZ)
{
	mRotationTransed->Set(pX, pY, pZ);
}

void DXWorld::SetRT(D3DXVECTOR3 pRotation)
{
	*mRotationTransed = pRotation;
}

void DXWorld::AddRT(float pX, float pY, float pZ)
{
	mRotationTransed->Add(pX, pY, pZ);
}

void DXWorld::AddRT(D3DXVECTOR3 pRotation)
{
	*mRotationTransed += pRotation;
}

void DXWorld::SubRT(float pX, float pY, float pZ)
{
	mRotationTransed->Sub(pX, pY, pZ);
}

void DXWorld::SubRT(D3DXVECTOR3 pRotation)
{
	*mRotationTransed -= pRotation;
}

void DXWorld::MulRT(float pX, float pY, float pZ)
{
	mRotationTransed->Mul(pX, pY, pZ);
}

void DXWorld::MulRT(D3DXVECTOR3 pRotation)
{
	*mRotationTransed *= pRotation;
}

void DXWorld::DivRT(float pX, float pY, float pZ)
{
	mRotationTransed->Div(pX, pY, pZ);
}

void DXWorld::DivRT(D3DXVECTOR3 pRotation)
{
	*mRotationTransed /= pRotation;
}

void DXWorld::SetS(float pX, float pY, float pZ)
{
	mScale->Set(pX, pY, pZ);
}

void DXWorld::SetS(D3DXVECTOR3 pScaling)
{
	*mScale = pScaling;
}

void DXWorld::AddS(float pX, float pY, float pZ)
{
	mScale->Add(pX, pY, pZ);
}

void DXWorld::AddS(D3DXVECTOR3 pScaling)
{
	*mScale += pScaling;
}

void DXWorld::SubS(float pX, float pY, float pZ)
{
	mScale->Sub(pX, pY, pZ);
}

void DXWorld::SubS(D3DXVECTOR3 pScaling)
{
	*mScale -= pScaling;
}

void DXWorld::MulS(float pX, float pY, float pZ)
{
	mScale->Mul(pX, pY, pZ);
}

void DXWorld::MulS(D3DXVECTOR3 pScaling)
{
	*mScale *= pScaling;
}

void DXWorld::DivS(float pX, float pY, float pZ)
{
	mScale->Div(pX, pY, pZ);
}

void DXWorld::DivS(D3DXVECTOR3 pScaling)
{
	*mScale /= pScaling;
}
