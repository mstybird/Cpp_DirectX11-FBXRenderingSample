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

void DXCamera::SetCamera(const DXWorld&pEyePosition, const DXWorld& pLookAtPosition)
{
	D3DXMATRIX mUp, mView;
	//Ž‹“_‚ÌÝ’è
	*mEyePosition = *pEyePosition.mPosition;
	//’Ž‹“_‚ÌÝ’è
	*mLookPosition = *pLookAtPosition.mPosition;
	//“ªã•ûŒü‚ÌŒvŽZ
	CreateRay(*mUpVector.get(), sUpVector);
//	mMatrix->SetLookAtLH(*mEyePosition, *mLookPosition, sUpVector);
////	D3DXMatrixLookAtLH(mMatrix, mEyePosition, mLookPosition, mUpVector);
//	D3DXMatrixInverse(&mView, NULL, mMatrix.get());
//	D3DXVec3TransformNormal(mUpVector.get(), &sUpVector, &mView);
}

void DXCamera::SetCamera(DXWorld&pEyePosition, const DXVector3&pDistance)
{
	*mLookPosition = *pEyePosition.mPosition;
	DXMatrix lTmpMatrixTrans;
	DXMatrix lTmpMatrixRotate;

	lTmpMatrixTrans.Translation(*mLookPosition);
	lTmpMatrixRotate.RotationXYZ(*pEyePosition.mRotationCenter,TYPE_ANGLE::DEGREE);
	D3DXVec3TransformCoord(
		mEyePosition.get(),
		&pDistance,
		&(lTmpMatrixRotate*lTmpMatrixTrans)
		);
	//“ªã•ûŒü‚ÌŒvŽZ
	CreateRay(*mUpVector.get(), sUpVector);

}

void DXCamera::SetCamera(DXWorld&pEyePosition, const DXVector3&pDistance, const DXVector3&aDistanceOffset)
{
	DXMatrix lTmpMatrixTrans;
	DXMatrix lTmpMatrixRotate;
	//’Ž‹“_‚ðÝ’è
	*mLookPosition = *pEyePosition.mPosition;
	//’Ž‹“_‚ð‰ñ“]

	//ˆÚ“®‰ñ“]s—ñ‚Ìì¬
	lTmpMatrixTrans.Translation(*mLookPosition);
	lTmpMatrixRotate.RotationXYZ(*pEyePosition.mRotationCenter, TYPE_ANGLE::DEGREE);
	//’Ž‹“_‚ð‰ñ“]
	DXVector3 lTmpLookOffset;
	D3DXVec3TransformCoord(
		&lTmpLookOffset,
		&(aDistanceOffset),
		&(lTmpMatrixRotate)
	);

	*mLookPosition += lTmpLookOffset;

	//Ž‹“_‚ð‰ñ“]
	D3DXVec3TransformCoord(
		mEyePosition.get(),
		&(pDistance+aDistanceOffset),
		&(lTmpMatrixRotate*lTmpMatrixTrans)
	);
	//“ªã•ûŒü‚ÌŒvŽZ
	CreateRay(*mUpVector.get(), sUpVector);
}

void DXCamera::CreateRay(DXVector3 & pOutRay, const DXVector3 & pRayDirection)const
{
	DXMatrix lMatrix;
	D3DXMATRIX mView;
	//“ªã•ûŒü‚ÌŒvŽZ
	DXVector3 Dir = *mLookPosition - *mEyePosition;
	//YŽ²Šp“x

	//


	mMatrix->SetLookAtLH(*mEyePosition, *mLookPosition, *mUpVector);
	D3DXMatrixInverse(&mView, NULL, mMatrix.get());
	D3DXVec3TransformNormal(&pOutRay, &pRayDirection, &mView);
	pOutRay.Normalize();
}

void DXCamera::Translation(TYPEMOVE pType, float pSpeed,const DXVector3&pDirection,bool pLockoned)
{
	//ƒƒbƒNƒIƒ“ƒtƒ‰ƒO‚ª—§‚Á‚Ä‚¢‚½ê‡‚Í‚±‚ÌˆÚ“®‚Å’Ž‹“_‚ð•ÏX‚µ‚È‚¢

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
