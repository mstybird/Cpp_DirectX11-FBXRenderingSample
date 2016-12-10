#include "DXVector3.h"
#include"DXVector2.h"
const DXVector3 DXVector3::sZeroVector{};




float DXVector3::GetDistance()const
{
	return D3DXVec3Length(this);
}

float DXVector3::GetDistance(DXVector3 & pPosition2)
{
	DXVector3 mixedPosition = *this - pPosition2;
	return D3DXVec3Length(&mixedPosition);
}

void DXVector3::GetCross(DXVector3 * DstCross, DXVector3 & pVector)
{
	D3DXVec3Cross(DstCross, this, &pVector);
}

void DXVector3::GetCross(float * DstCross, DXVector3 & pVector)
{
	DXVector3 lCross;
	D3DXVec3Cross(&lCross, this, &pVector);
	*DstCross = x*pVector.x +
		y*pVector.y +
		z*pVector.z;
}

void DXVector3::Normalize()
{
	D3DXVec3Normalize(this, this);
}

float DXVector3::GetMedian()
{
	return (x + y + z) / 3.0f;
}

float DXVector3::GetAngle(TYPE_ANGLE pType, DXVector3 & pPosition2)
{
	float lengthA=GetDistance();
	float lengthB = GetDistance(pPosition2);
	float lCross;
	//���ς����߂�
	GetCross(&lCross, pPosition2);
	//�R�T�C�������߂�
	float lCos = lCross / (lengthA*lengthB);
	//�V�[�^�����߂�
	float lSita = acosf(lCos);

	if (pType == TYPE_DEGREE) {
		lSita = D3DXToDegree(lSita);
	}

	//XY(Z��])
	//XZ(Y��])
	//YZ(X��])




	return lSita;
}

DXVector3 & DXVector3::operator=(const DXVector2 & pVector2D)
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	x = pVector2D.x;
	y = pVector2D.y;
	z = 0.0f;
	return *this;
}

DXVector3 DXVector3::operator+(float pXYZ)const
{
	DXVector3 lResult{ *this };
	lResult.x += pXYZ;
	lResult.y += pXYZ;
	lResult.z += pXYZ;
	return lResult;
}

DXVector3 & DXVector3::operator+=(float pXYZ)
{
	x += pXYZ;
	y += pXYZ;
	z += pXYZ;
	return *this;
}

DXVector3 DXVector3::operator+(const D3DXVECTOR3 & pAddVector)const
{
	DXVector3 lResult{ *this };
	lResult.x += pAddVector.x;
	lResult.y += pAddVector.y;
	lResult.z += pAddVector.z;
	return lResult;
}

DXVector3 & DXVector3::operator+=(const D3DXVECTOR3 & pAddVector)
{
	x += pAddVector.x;
	y += pAddVector.y;
	z += pAddVector.z;
	return *this;
}



DXVector3 DXVector3::operator-(float pXYZ)const
{
	DXVector3 lResult{ *this };
	lResult.x -= pXYZ;
	lResult.y -= pXYZ;
	lResult.z -= pXYZ;
	return lResult;
}

DXVector3 & DXVector3::operator-=(float pXYZ)
{
	x -= pXYZ;
	y -= pXYZ;
	z -= pXYZ;
	return *this;
}

DXVector3 DXVector3::operator-(const D3DXVECTOR3 & pSubVector)const
{
	DXVector3 lResult{ *this };
	lResult.x -= pSubVector.x;
	lResult.y -= pSubVector.y;
	lResult.z -= pSubVector.z;
	return lResult;
}



DXVector3 & DXVector3::operator-=(const D3DXVECTOR3 & pSubVector)
{
	x -= pSubVector.x;
	y -= pSubVector.y;
	z -= pSubVector.z;
	return *this;
}

DXVector3 DXVector3::operator*(float pXYZ)const
{
	DXVector3 lResult{ *this };
	lResult.x *= pXYZ;
	lResult.y *= pXYZ;
	lResult.z *= pXYZ;
	return lResult;
}

DXVector3 & DXVector3::operator*=(float pXYZ)
{
	x *= pXYZ;
	y *= pXYZ;
	z *= pXYZ;
	return *this;
}

DXVector3 DXVector3::operator*(const D3DXVECTOR3 & pMulVector)const
{
	DXVector3 lResult{*this};
	lResult.x *= pMulVector.x;
	lResult.y *= pMulVector.y;
	lResult.z *= pMulVector.z;
	return lResult;
}

DXVector3 & DXVector3::operator*=(const D3DXVECTOR3 & pMulVector)
{
	x *= pMulVector.x;
	y *= pMulVector.y;
	z *= pMulVector.z;
	return *this;
}

DXVector3 DXVector3::operator/(float pXYZ)const
{
	DXVector3 lResult{ *this };
	lResult.x /= pXYZ;
	lResult.y /= pXYZ;
	lResult.z /= pXYZ;
	return lResult;
}

DXVector3 & DXVector3::operator/=(float pXYZ)
{
	x /= pXYZ;
	y /= pXYZ;
	z /= pXYZ;
	return *this;
}

DXVector3 DXVector3::operator/(const D3DXVECTOR3 & pMulVector)const
{
	DXVector3 lResult{ *this };
	lResult.x /= pMulVector.x;
	lResult.y /= pMulVector.y;
	lResult.z /= pMulVector.z;
	return lResult;
}

DXVector3 & DXVector3::operator/=(const D3DXVECTOR3 & pMulVector)
{
	x /= pMulVector.x;
	y /= pMulVector.y;
	z /= pMulVector.z;
	return *this;
}
