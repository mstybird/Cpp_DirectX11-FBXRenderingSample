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
	//���_�̐ݒ�
	*mEyePosition = *pEyePosition.mPosition;
	//�����_�̐ݒ�
	*mLookPosition = *pLookAtPosition.mPosition;
	//��������̌v�Z
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
	//��������̌v�Z
	CreateRay(mUpVector, sUpVector);

}

void DXCamera::SetCamera(DXWorld&pEyePosition, const DXVector3&pDistance, const DXVector3&aDistanceOffset)
{
	DXMatrix lTmpMatrixTrans;
	DXMatrix lTmpMatrixRotate;
	//�����_��ݒ�
	mLookPosition = pEyePosition.mPosition;
	//�����_����]

	//�ړ���]�s��̍쐬
	lTmpMatrixTrans.Translation(mLookPosition);
	lTmpMatrixRotate.RotationXYZ(pEyePosition.mRotationCenter, TYPE_ANGLE::DEGREE);
	//�����_����]
	DXVector3 lTmpLookOffset;
	D3DXVec3TransformCoord(
		&lTmpLookOffset,
		&(aDistanceOffset),
		&(lTmpMatrixRotate)
	);

	mLookPosition += lTmpLookOffset;

	//���_����]
	D3DXVec3TransformCoord(
		&mEyePosition,
		&(pDistance+aDistanceOffset),
		&(lTmpMatrixRotate*lTmpMatrixTrans)
	);
	//��������̌v�Z
	CreateRay(mUpVector, sUpVector);
}

void DXCamera::CreateRay(DXVector3 & pOutRay, const DXVector3 & pRayDirection)
{
	DXMatrix lMatrix;
	D3DXMATRIX mView;
	//��������̌v�Z
	DXVector3 Dir = mLookPosition - *mEyePosition;
	//Y���p�x

	//

	
	mMatrix.SetLookAtLH(mEyePosition, mLookPosition, mUpVector);
	D3DXMatrixInverse(&mView, NULL, &mMatrix);
	D3DXVec3TransformNormal(&pOutRay, &pRayDirection, &mView);
	pOutRay.Normalize();
}

void DXCamera::Translation(TYPEMOVE pType, float pSpeed,const DXVector3&pDirection,bool pLockoned)
{
	//���b�N�I���t���O�������Ă����ꍇ�͂��̈ړ��Œ����_��ύX���Ȃ�

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
