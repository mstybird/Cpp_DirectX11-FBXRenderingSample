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
	mEyePosition{std::make_shared<DXVector3>()},
	mLookPosition{ std::make_shared<DXVector3>() },
	mUpVector{ std::make_shared<DXVector3>(sUpVector)},
	mRotate{ std::make_shared<DXVector3>() },
	mMatrix{ std::make_shared<DXMatrix>()  }
{
	mMatrix->SetIdentity();
}

DXCamera::~DXCamera()
{
}

void DXCamera::SetCamera(const std::weak_ptr<DXWorld>&pEyePosition, const std::weak_ptr<DXWorld>&pLookAtPosition)
{
	D3DXMATRIX mUp, mView;
	//���_�̐ݒ�
	*mEyePosition = *pEyePosition.lock()->mPosition;
	//�����_�̐ݒ�
	*mLookPosition = *pLookAtPosition.lock()->mPosition;
	//��������̌v�Z
	CreateRay(*mUpVector.get(), sUpVector);
//	mMatrix->SetLookAtLH(*mEyePosition, *mLookPosition, sUpVector);
////	D3DXMatrixLookAtLH(mMatrix, mEyePosition, mLookPosition, mUpVector);
//	D3DXMatrixInverse(&mView, NULL, mMatrix.get());
//	D3DXVec3TransformNormal(mUpVector.get(), &sUpVector, &mView);
}

void DXCamera::SetCamera(const std::weak_ptr<DXWorld>& pEyePosition, const DXVector3 & pDistance)
{
	*mLookPosition = *pEyePosition.lock()->mPosition;
	DXMatrix lTmpMatrixTrans;
	DXMatrix lTmpMatrixRotate;

	lTmpMatrixTrans.Translation(*mLookPosition);
	lTmpMatrixRotate.RotationXYZ(*pEyePosition.lock()->mRotationCenter,TYPE_ANGLE::DEGREE);
	D3DXVec3TransformCoord(
		mEyePosition.get(),
		&pDistance,
		&(lTmpMatrixRotate*lTmpMatrixTrans)
		);
	//��������̌v�Z
	CreateRay(*mUpVector.get(), sUpVector);

}

void DXCamera::CreateRay(DXVector3 & pOutRay, const DXVector3 & pRayDirection)const
{
	DXMatrix lMatrix;
	D3DXMATRIX mView;
	//��������̌v�Z
	mMatrix->SetLookAtLH(*mEyePosition, *mLookPosition, *mUpVector);
	D3DXMatrixInverse(&mView, NULL, mMatrix.get());
	D3DXVec3TransformNormal(&pOutRay, &pRayDirection, &mView);

}

void DXCamera::Translation(TYPEMOVE pType, float pSpeed,const DXVector3&pDirection,bool pLockoned)
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
		mMatrix->SetLookAtLH(*mEyePosition, *mLookPosition, sUpVector);
		mMatrix->Inverse();
		D3DXVec3TransformNormal(&lDirection, &lDirection, mMatrix.get());
		D3DXVec3TransformNormal(mUpVector.get(), &sUpVector, mMatrix.get());
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


std::weak_ptr<DXMatrix> DXCamera::GetMatrix()
{
	mMatrix->SetLookAtLH(*mEyePosition, *mLookPosition, sUpVector);
	//D3DXMatrixLookAtLH(mMatrix, mEyePosition, mLookPosition, mUpVector);
	return mMatrix;
}

void DXCamera::GetEyeT(DXVector3& pOutTranslate)
{
	pOutTranslate = *mEyePosition;
}

void DXCamera::GetLookT(DXVector3& pOutTranslate)
{
	pOutTranslate = *mLookPosition;
}
