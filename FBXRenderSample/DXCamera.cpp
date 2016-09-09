#include "DXCamera.h"
#include"DXVector3.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"DX11Resrouce.h"
#include<d3dx9.h>

const DXVector3 DXCamera::sUpVector{ 0.0f,1.0f,0.0f };
const DXVector3 DXCamera::DIRECTION_TYPE::LEFTRIGHT{ 1.0f,0.0f,0.0f };
const DXVector3 DXCamera::DIRECTION_TYPE::UPDOWN{ 0.0f,1.0f,0.0f };
const DXVector3 DXCamera::DIRECTION_TYPE::FRONTBACK{ 0.0f,0.0f,1.0f };


DXCamera::DXCamera() :
	mEyePosition{ new DXVector3 },
	mLookPosition{ new DXVector3 },
	mUpVector{ new DXVector3(sUpVector)},
	mRotate{ new DXVector3 },
	mMatrix{new DXMatrix }
{
	D3DXMatrixIdentity(mMatrix);
}

DXCamera::~DXCamera()
{
	SAFE_DELETE(mEyePosition);
	SAFE_DELETE(mLookPosition);
	SAFE_DELETE(mUpVector);
	SAFE_DELETE(mRotate);
	SAFE_DELETE(mMatrix);
}

void DXCamera::SetCamera(const DXWorld & pEyePosition, const DXWorld & pLookAtPosition)
{
	D3DXMATRIX mUp, mView;
	//視点の設定
	*mEyePosition = *pEyePosition.mPosition;
	//注視点の設定
	*mLookPosition = *pLookAtPosition.mPosition;
	//頭上方向の計算
	mMatrix->SetLookAtLH(*mEyePosition, *mLookPosition, sUpVector);
//	D3DXMatrixLookAtLH(mMatrix, mEyePosition, mLookPosition, mUpVector);
	D3DXMatrixInverse(&mView, NULL, mMatrix);
	D3DXVec3TransformNormal(mUpVector, &sUpVector, &mView);
}

void DXCamera::Translation(TYPEMOVE pType, float pSpeed,const DXVector3&pDirection,bool pLockoned)
{
	//ロックオンフラグが立っていた場合はこの移動で注視点を変更しない

	DXVector3 lDirection = pDirection;
	switch (pType)
	{
	case DXCamera::TYPE_NORMAL:
		lDirection *= pSpeed;
		*mEyePosition += lDirection;
		*mLookPosition += pLockoned ? DXVector3::sZeroVector : lDirection;
		break;
	case DXCamera::TYPE_PARALLEL:
		mMatrix->SetLookAtLH(*mEyePosition, *mLookPosition, sUpVector);
		D3DXMatrixInverse(mMatrix, nullptr, mMatrix);
		D3DXVec3TransformNormal(&lDirection, &lDirection, mMatrix);
		D3DXVec3TransformNormal(mUpVector, &sUpVector, mMatrix);
		lDirection *= pSpeed;
		*mEyePosition += lDirection;
		*mLookPosition += pLockoned ? DXVector3::sZeroVector : lDirection;
		break;
	default:
		break;
	}
}



void DXCamera::Rotate(float pX, float pY, float pZ)
{


}


DXMatrix * DXCamera::GetMatrix()
{
	mMatrix->SetLookAtLH(*mEyePosition, *mLookPosition, sUpVector);
	//D3DXMatrixLookAtLH(mMatrix, mEyePosition, mLookPosition, mUpVector);
	return mMatrix;
}
