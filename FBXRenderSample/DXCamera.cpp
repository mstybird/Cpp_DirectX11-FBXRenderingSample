#include "DXCamera.h"
#include"DXVector3.h"
#include"DXWorld.h"
#include"DX11Resrouce.h"

const DXVector3 DXCamera::sUpVector{ 0.0f,1.0f,0.0f };
const DXVector3 DXCamera::DIRECTION_TYPE::LEFTRIGHT{ 1.0f,0.0f,0.0f };
const DXVector3 DXCamera::DIRECTION_TYPE::UPDOWN{ 0.0f,1.0f,0.0f };
const DXVector3 DXCamera::DIRECTION_TYPE::FRONTBACK{ 0.0f,0.0f,1.0f };


DXCamera::DXCamera() :
	mEyePosition{ new DXVector3 },
	mLookPosition{ new DXVector3 },
	mUpVector{ new DXVector3(sUpVector)},
	mRotate{ new DXVector3 },
	mMatrix{new D3DXMATRIX }
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
	//���_�̐ݒ�
	*mEyePosition = *pEyePosition.mPosition;
	//�����_�̐ݒ�
	*mLookPosition = *pLookAtPosition.mPosition;
	//��������̌v�Z
	D3DXMatrixLookAtLH(mMatrix, mEyePosition, mLookPosition, mUpVector);
	D3DXMatrixInverse(&mView, NULL, mMatrix);
	D3DXVec3TransformNormal(mUpVector, &sUpVector, &mView);
}

void DXCamera::Translation(TYPEMOVE pType, float pSpeed,const D3DXVECTOR3&pDirection,bool pLockoned)
{
	//���b�N�I���t���O�������Ă����ꍇ�͂��̈ړ��Œ����_��ύX���Ȃ�

	DXVector3 lDirection = pDirection;
	switch (pType)
	{
	case DXCamera::TYPE_NORMAL:
		lDirection *= pSpeed;
		*mEyePosition += lDirection;
		*mLookPosition += pLockoned ? DXVector3::sZeroVector : lDirection;
		break;
	case DXCamera::TYPE_PARALLEL:
		D3DXMatrixLookAtLH(mMatrix, mEyePosition, mLookPosition, mUpVector);
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


D3DXMATRIX * DXCamera::GetMatrix()
{
	D3DXMatrixLookAtLH(mMatrix, mEyePosition, mLookPosition, mUpVector);
	return mMatrix;
}
