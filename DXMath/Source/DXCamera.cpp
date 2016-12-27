#include "DXCamera.h"
#include"DXVector3.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"DXMath.hh"
#include<d3dx9.h>

const DXVector3 DXCamera::sUpVector{ 0.0f,1.0f,0.0f };
const DXVector3 DXCamera::DIRECTION_TYPE::LEFTRIGHT{ 1.0f,0.0f,0.0f };
const DXVector3 DXCamera::DIRECTION_TYPE::UPDOWN{ 0.0f,1.0f,0.0f };
const DXVector3 DXCamera::DIRECTION_TYPE::FRONTBACK{ 0.0f,0.0f,1.0f };


DXCamera::DXCamera() :
	mUpVector{ sUpVector}
{
	mMatrix.SetIdentity();
}

DXCamera::~DXCamera()
{
}

void DXCamera::SetCamera(const DXWorld&pEyePosition, const DXWorld& pLookAtPosition)
{
	D3DXMATRIX mUp, mView;
	//視点の設定
	*mEyePosition = *pEyePosition.mPosition;
	//注視点の設定
	*mLookPosition = *pLookAtPosition.mPosition;
	//頭上方向の計算
	CreateRay(mUpVector, sUpVector);
//	mMatrix->SetLookAtLH(*mEyePosition, *mLookPosition, sUpVector);
////	D3DXMatrixLookAtLH(mMatrix, mEyePosition, mLookPosition, mUpVector);
//	D3DXMatrixInverse(&mView, NULL, mMatrix);
//	D3DXVec3TransformNormal(mUpVector, &sUpVector, &mView);
}

void DXCamera::SetCamera(DXWorld&pEyePosition, const DXVector3&pDistance)
{
	*mLookPosition = *pEyePosition.mPosition;
	DXMatrix lTmpMatrixTrans;
	DXMatrix lTmpMatrixRotate;

	lTmpMatrixTrans.Translation(mLookPosition);
	lTmpMatrixRotate.RotationXYZ(pEyePosition.mRotationCenter,TYPE_ANGLE::DEGREE);
	D3DXVec3TransformCoord(
		&mEyePosition,
		&pDistance,
		&(lTmpMatrixRotate*lTmpMatrixTrans)
		);
	//頭上方向の計算
	CreateRay(mUpVector, sUpVector);

}

void DXCamera::SetCamera(DXWorld&pEyePosition, const DXVector3&pDistance, const DXVector3&aDistanceOffset)
{
	DXMatrix lTmpMatrixTrans;
	DXMatrix lTmpMatrixRotate;
	//注視点を設定
	mLookPosition = pEyePosition.mPosition;
	//注視点を回転

	//移動回転行列の作成
	lTmpMatrixTrans.Translation(mLookPosition);
	lTmpMatrixRotate.RotationXYZ(pEyePosition.mRotationCenter, TYPE_ANGLE::DEGREE);
	//注視点を回転
	DXVector3 lTmpLookOffset;
	D3DXVec3TransformCoord(
		&lTmpLookOffset,
		&(aDistanceOffset),
		&(lTmpMatrixRotate)
	);

	mLookPosition += lTmpLookOffset;

	//視点を回転
	D3DXVec3TransformCoord(
		&mEyePosition,
		&(pDistance+aDistanceOffset),
		&(lTmpMatrixRotate*lTmpMatrixTrans)
	);
	//頭上方向の計算
	CreateRay(mUpVector, sUpVector);
}

void DXCamera::CreateRay(DXVector3 & pOutRay, const DXVector3 & pRayDirection)
{
	DXMatrix lMatrix;
	D3DXMATRIX mView;
	//頭上方向の計算
	DXVector3 Dir = mLookPosition - *mEyePosition;
	//Y軸角度

	//

	
	mMatrix.SetLookAtLH(mEyePosition, mLookPosition, mUpVector);
	D3DXMatrixInverse(&mView, NULL, &mMatrix);
	D3DXVec3TransformNormal(&pOutRay, &pRayDirection, &mView);
	pOutRay.Normalize();
}

void DXCamera::Translation(TYPEMOVE pType, float pSpeed,const DXVector3&pDirection,bool pLockoned)
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
		mMatrix.SetLookAtLH(mEyePosition, mLookPosition, sUpVector);
		mMatrix.Inverse();
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

void DXCamera::Clone(DXCamera & pOutClone)
{
	*pOutClone.mEyePosition = *mEyePosition;
	*pOutClone.mLookPosition = *mLookPosition;
	*pOutClone.mRotate = *mRotate;
	*pOutClone.mUpVector = *mUpVector;
	*pOutClone.mMatrix = *mMatrix;
}



void DXCamera::Rotate(float pX, float pY, float pZ)
{


}


DXMatrix* DXCamera::GetMatrix()
{
	mMatrix.SetLookAtLH(mEyePosition, mLookPosition, sUpVector);
	//D3DXMatrixLookAtLH(mMatrix, mEyePosition, mLookPosition, mUpVector);
	return &mMatrix;
}

void DXCamera::GetEyeT(DXVector3& pOutTranslate)
{
	pOutTranslate = mEyePosition;
}

void DXCamera::GetLookT(DXVector3& pOutTranslate)
{
	pOutTranslate = mLookPosition;
}
