#include "DXVector3.h"
#include"DXVector2.h"
const DXVector3 DXVector3::sZeroVector{};

DXVector3::DXVector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

DXVector3::DXVector3(float pX, float pY, float pZ):D3DXVECTOR3(pX,pY,pZ)
{
}

DXVector3::DXVector3(D3DXVECTOR3 pVector):D3DXVECTOR3(pVector)
{
}

void DXVector3::Set(float pX, float pY, float pZ)
{
	x = pX;
	y = pY;
	z = pZ;
}

void DXVector3::Set(float pXYZ)
{
	x = pXYZ;
	y = pXYZ;
	z = pXYZ;
}

void DXVector3::Add(float pX, float pY, float pZ)
{
	x += pX;
	y += pY;
	z += pZ;
}

void DXVector3::Add(float pXYZ)
{
	x += pXYZ;
	y += pXYZ;
	z += pXYZ;
}

void DXVector3::Sub(float pX, float pY, float pZ)
{
	x -= pX;
	y -= pY;
	z -= pZ;
}

void DXVector3::Sub(float pXYZ)
{
	x -= pXYZ;
	y -= pXYZ;
	z -= pXYZ;
}

void DXVector3::Mul(float pX, float pY, float pZ)
{
	x *= pX;
	y *= pY;
	z *= pZ;
}

void DXVector3::Mul(float pXYZ)
{
	x *= pXYZ;
	y *= pXYZ;
	z *= pXYZ;
}

void DXVector3::Div(float pX, float pY, float pZ)
{
	x /= pX;
	y /= pY;
	z /= pZ;
}

void DXVector3::Div(float pXYZ)
{
	x /= pXYZ;
	y /= pXYZ;
	z /= pXYZ;
}

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
	//内積を求める
	GetCross(&lCross, pPosition2);
	//コサインを求める
	float lCos = lCross / (lengthA*lengthB);
	//シータを求める
	float lSita = acosf(lCos);

	if (pType == TYPE_DEGREE) {
		lSita = D3DXToDegree(lSita);
	}

	//XY(Z回転)
	//XZ(Y回転)
	//YZ(X回転)


	return lSita;
}

DXVector3 & DXVector3::operator=(const DXVector2 & pVector2D)
{
	// TODO: return ステートメントをここに挿入します
	x = pVector2D.x;
	y = pVector2D.y;
	z = 0.0f;
	return *this;
}

DXVector3 DXVector3::operator+(float pXYZ)
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

DXVector3 DXVector3::operator+(const D3DXVECTOR3 & pAddVector)
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



DXVector3 DXVector3::operator-(float pXYZ)
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

DXVector3 DXVector3::operator-(const D3DXVECTOR3 & pSubVector)
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

DXVector3 DXVector3::operator*(float pXYZ)
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

DXVector3 DXVector3::operator*(const D3DXVECTOR3 & pMulVector)
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

DXVector3 DXVector3::operator/(float pXYZ)
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

DXVector3 DXVector3::operator/(const D3DXVECTOR3 & pMulVector)
{
	DXVector3 lResult{ *this };
	x /= pMulVector.x;
	y /= pMulVector.y;
	z /= pMulVector.z;
	return lResult;
}

DXVector3 & DXVector3::operator/=(const D3DXVECTOR3 & pMulVector)
{
	x /= pMulVector.x;
	y /= pMulVector.y;
	z /= pMulVector.z;
	return *this;
}
