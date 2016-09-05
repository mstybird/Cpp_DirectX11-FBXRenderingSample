#include "DXCamera.h"
//#include"DX11RenderResource.h"

void DXCamera::SetEyeT(float pX, float pY, float pZ)
{
	mEyePosition.Set(pX, pY, pZ);
}

void DXCamera::SetEyeT(D3DXVECTOR3 pPosition)
{
	mEyePosition = pPosition;
}

void DXCamera::SetEyeT(DXWorld pEyeTarget)
{
	mEyePosition = pEyeTarget.mPosition;
}

//void DXCamera::SetEyeT(DX11RenderResource pEyeTarget)
//{
//	mEyePosition = pEyeTarget.smWorld.mPosition;
//}

void DXCamera::SetLookT(float pX, float pY, float pZ)
{
	mLookPosition.Set(pX, pY, pZ);
}

void DXCamera::SetLookT(D3DXVECTOR3 pLookAt)
{
	mLookPosition = pLookAt;
}

void DXCamera::SetLookT(DXWorld pEyeTarget)
{
	auto v = *pEyeTarget.GetMatrix();
	mLookPosition.Set(v._41, v._42, v._43);
}

void DXCamera::AddEyeT(float pSpeed, DXVector3 pDirection)
{

	pDirection *= pSpeed;
	mEyePosition += pDirection;

}

void DXCamera::AddLookT(float pSpeed, DXVector3 pDirection)
{
	pDirection *= pSpeed;
	mLookPosition += pDirection;
}

void DXCamera::AddTranslation(TYPEMOVE pType, float pSpeed, DXVector3 pDirection)
{
	D3DXMATRIX lRotation;
	switch (pType)
	{
		//���s�ړ�
	case DXCamera::TYPE_PARALLEL:
		pDirection *= pSpeed;
		mEyePosition += pDirection;
		mLookPosition += pDirection;
		break;
	case DXCamera::TYPE_TARGET:
		//��]�ړ�(�^�[�Q�b�g�̕����𐳖ʂƂ������b�N�I���ړ�)
	{
		float lTargetV, lTargetH;
		lTargetV = DXVector3::GetVertical(DXVector3::TYPE_RADIAN, mEyePosition, mLookPosition);
		lTargetH = DXVector3::GetHolizontal(DXVector3::TYPE_RADIAN, mEyePosition, mLookPosition);
		D3DXMatrixRotationYawPitchRoll(&lRotation, lTargetH, lTargetV, 0);
		//�ړ��ʌv�Z
		D3DXVec3TransformCoord(&pDirection, &pDirection, &lRotation);
		pDirection *= pSpeed;
		mEyePosition += pDirection;

	}
		break;
	case DXCamera::TYPE_ROTATE:
		//�J�����̈ʒu�ƒ����_�̊p�x���v�Z
		float x, y, z;
		x = DXVector3::GetAngleX(DXVector3::TYPE_RADIAN, mEyePosition, mLookPosition);
		y = DXVector3::GetAngleY(DXVector3::TYPE_RADIAN, mEyePosition, mLookPosition);
		z = DXVector3::GetAngleZ(DXVector3::TYPE_RADIAN, mEyePosition, mLookPosition);
		D3DXMatrixRotationYawPitchRoll(
			&lRotation,
			y, x, z
			);
		D3DXVec3TransformCoord(&pDirection, &pDirection, &lRotation);
		pDirection *= pSpeed;
		mEyePosition += pDirection;
		mLookPosition += pDirection;
		break;
	default:
		break;
	}
}

void DXCamera::Rotate(float pX, float pY, float pZ)
{
	static D3DXMATRIX lPoseMat;
	D3DXMatrixIdentity(&lPoseMat);
	float Y, X, Z;
	X = 0;
	Y = 0;
	Z = 0;
	D3DXMATRIX lDeltamat;
	D3DXMatrixIdentity(&lDeltamat);


}

//void DXCamera::SetLookT(DX11RenderResource pLookTarget)
//{
//	mLookPosition = pLookTarget.smWorld.mPosition;
//}

void DXCamera::SetUpV(float pX, float pY, float pZ)
{
	mUpVector.Set(pX, pY, pZ);
}

void DXCamera::SetUpV(D3DXVECTOR3 pVector)
{
	mUpVector = pVector;
}

D3DXMATRIX * DXCamera::GetMatrix()
{
	D3DXMatrixLookAtLH(&mMatrix, &mEyePosition, &mLookPosition, &mUpVector);
	return &mMatrix;
}
