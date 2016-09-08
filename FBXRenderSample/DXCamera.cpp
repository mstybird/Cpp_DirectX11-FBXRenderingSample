#include "DXCamera.h"

const D3DXVECTOR3 DXCamera::sUpVector{ 0.0f,1.0f,0.0f };
const D3DXVECTOR3 DXCamera::DIRECTION_TYPE::LEFTRIGHT{ 1.0f,0.0f,0.0f };
const D3DXVECTOR3 DXCamera::DIRECTION_TYPE::UPDOWN{ 0.0f,1.0f,0.0f };
const D3DXVECTOR3 DXCamera::DIRECTION_TYPE::FRONTBACK{ 0.0f,0.0f,1.0f };


DXCamera::DXCamera() :
	mEyePosition{ 0.0f,0.0f,0.0f },
	mLookPosition{0.0f,0.0f,0.0f},
	mUpVector{sUpVector},
	mRotate{0.0f,0.0f,0.0f}
{
	D3DXMatrixIdentity(&mMatrix);
}

void DXCamera::SetCamera(const DXWorld & pEyePosition, const DXWorld & pLookAtPosition)
{
	D3DXMATRIX mUp, mView;
	//視点の設定
	mEyePosition = pEyePosition.mPosition;
	//注視点の設定
	mLookPosition = pLookAtPosition.mPosition;
	//頭上方向の計算
	D3DXMatrixLookAtLH(&mMatrix, &mEyePosition, &mLookPosition, &mUpVector);
	D3DXMatrixInverse(&mView, NULL, &mMatrix);
	D3DXVec3TransformNormal(&mUpVector, &sUpVector, &mView);
}

void DXCamera::Translation(TYPEMOVE pType, float pSpeed,const D3DXVECTOR3&pDirection,bool pLockoned)
{
	//ロックオンフラグが立っていた場合はこの移動で注視点を変更しない

	DXVector3 lDirection = pDirection;
	switch (pType)
	{
	case DXCamera::TYPE_NORMAL:
		lDirection *= pSpeed;
		mEyePosition += lDirection;
		mLookPosition += pLockoned ? DXVector3::sZeroVector : lDirection;
		break;
	case DXCamera::TYPE_PARALLEL:
		D3DXMatrixLookAtLH(&mMatrix, &mEyePosition, &mLookPosition, &mUpVector);
		D3DXMatrixInverse(&mMatrix, nullptr, &mMatrix);
		D3DXVec3TransformNormal(&lDirection, &lDirection, &mMatrix);
		D3DXVec3TransformNormal(&mUpVector, &sUpVector, &mMatrix);
		lDirection *= pSpeed;
		mEyePosition += lDirection;
		mLookPosition += pLockoned ? DXVector3::sZeroVector : lDirection;
		break;
	default:
		break;
	}
}



void DXCamera::Rotate(float pX, float pY, float pZ)
{


}


D3DXMATRIX * DXCamera::GetMatrix()
{
	D3DXMatrixLookAtLH(&mMatrix, &mEyePosition, &mLookPosition, &mUpVector);
	return &mMatrix;
}
