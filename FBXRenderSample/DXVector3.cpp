#include "DXVector3.h"

float DXVector3::GetAngleX(TYPE_ANGLE pType, DXVector3 & pPosition1, DXVector3 & pPosition2)
{
	float radian = atan2f(
		pPosition2.y - pPosition1.y,
		pPosition2.z - pPosition1.z
		);

	if (pType == TYPE_ANGLE::TYPE_DEGREE) {
		return D3DXToDegree(radian);
	}
	else {
		return radian;

	}
}

float DXVector3::GetAngleY(TYPE_ANGLE pType, DXVector3 & pPosition1, DXVector3 & pPosition2)
{
	float radian = atan2f(
		pPosition2.x - pPosition1.x,
		pPosition2.z - pPosition1.z
		);

	if (pType == TYPE_ANGLE::TYPE_DEGREE) {
		return D3DXToDegree(radian);
	}
	else {
		return radian;

	}
}

float DXVector3::GetAngleZ(TYPE_ANGLE pType, DXVector3 & pPosition1, DXVector3 & pPosition2)
{
	float radian = atan2f(
		pPosition2.y - pPosition1.y,
		pPosition2.x - pPosition1.x
		);

	if (pType == TYPE_ANGLE::TYPE_DEGREE) {
		return D3DXToDegree(radian);
	}
	else {
		return radian;

	}

}

float DXVector3::GetVertical(TYPE_ANGLE pType, DXVector3 & pPosition1, DXVector3 & pPosition2)
{
	//Xäpìx
	float lVertical = atan2f(
		pPosition2.y - pPosition1.y,
		pPosition2.z - pPosition1.z
		);

	if (pType == TYPE_ANGLE::TYPE_DEGREE) {
		return D3DXToDegree(lVertical);
	}
	else {
		return lVertical;

	}
}

float DXVector3::GetHolizontal(TYPE_ANGLE pType, DXVector3 & pPosition1, DXVector3 & pPosition2)
{
	//Yäpìx
	float lHolizuntal = atan2f(
		pPosition2.x - pPosition1.x,
		pPosition2.z - pPosition1.z
		);

	if (pType == TYPE_ANGLE::TYPE_DEGREE) {
		return D3DXToDegree(lHolizuntal);
	}
	else {
		return lHolizuntal;

	}
}

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

float DXVector3::GetDistance()
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

float DXVector3::GetAngle(TYPE_ANGLE pType, DXVector3 & pPosition2)
{
	float lengthA=GetDistance();
	float lengthB = GetDistance(pPosition2);
	float lCross;
	//ì‡êœÇãÅÇﬂÇÈ
	GetCross(&lCross, pPosition2);
	//ÉRÉTÉCÉìÇãÅÇﬂÇÈ
	float lCos = lCross / (lengthA*lengthB);
	//ÉVÅ[É^ÇãÅÇﬂÇÈ
	float lSita = acosf(lCos);

	if (pType == TYPE_DEGREE) {
		lSita = D3DXToDegree(lSita);
	}

	//XY(ZâÒì])
	//XZ(YâÒì])
	//YZ(XâÒì])


	return lSita;
}

DXVector3 & DXVector3::operator+(float pXYZ)
{
	x += pXYZ;
	y += pXYZ;
	z += pXYZ;
	return *this;
}

DXVector3 & DXVector3::operator+=(float pXYZ)
{
	x += pXYZ;
	y += pXYZ;
	z += pXYZ;
	return *this;
}

DXVector3 & DXVector3::operator+(D3DXVECTOR3 & pAddVector)
{
	x += pAddVector.x;
	y += pAddVector.y;
	z += pAddVector.z;
	return *this;
}

DXVector3 & DXVector3::operator+=(D3DXVECTOR3 & pAddVector)
{
	x += pAddVector.x;
	y += pAddVector.y;
	z += pAddVector.z;
	return *this;
}

DXVector3 & DXVector3::operator-(float pXYZ)
{
	x -= pXYZ;
	y -= pXYZ;
	z -= pXYZ;
	return *this;
}

DXVector3 & DXVector3::operator-=(float pXYZ)
{
	x -= pXYZ;
	y -= pXYZ;
	z -= pXYZ;
	return *this;
}

DXVector3 & DXVector3::operator-(D3DXVECTOR3 & pSubVector)
{
	x -= pSubVector.x;
	y -= pSubVector.y;
	z -= pSubVector.z;
	return *this;
}

DXVector3 & DXVector3::operator-=(D3DXVECTOR3 & pSubVector)
{
	x -= pSubVector.x;
	y -= pSubVector.y;
	z -= pSubVector.z;
	return *this;
}

DXVector3 & DXVector3::operator*(float pXYZ)
{
	x *= pXYZ;
	y *= pXYZ;
	z *= pXYZ;
	return *this;
}

DXVector3 & DXVector3::operator*=(float pXYZ)
{
	x *= pXYZ;
	y *= pXYZ;
	z *= pXYZ;
	return *this;
}

DXVector3 & DXVector3::operator*(D3DXVECTOR3 & pMulVector)
{
	x *= pMulVector.x;
	y *= pMulVector.y;
	z *= pMulVector.z;
	return *this;
}

DXVector3 & DXVector3::operator*=(D3DXVECTOR3 & pMulVector)
{
	x *= pMulVector.x;
	y *= pMulVector.y;
	z *= pMulVector.z;
	return *this;
}

DXVector3 & DXVector3::operator/(float pXYZ)
{
	x /= pXYZ;
	y /= pXYZ;
	z /= pXYZ;
	return *this;
}

DXVector3 & DXVector3::operator/=(float pXYZ)
{
	x /= pXYZ;
	y /= pXYZ;
	z /= pXYZ;
	return *this;
}

DXVector3 & DXVector3::operator/(D3DXVECTOR3 & pMulVector)
{
	x /= pMulVector.x;
	y /= pMulVector.y;
	z /= pMulVector.z;
	return *this;
}

DXVector3 & DXVector3::operator/=(D3DXVECTOR3 & pMulVector)
{
	x /= pMulVector.x;
	y /= pMulVector.y;
	z /= pMulVector.z;
	return *this;
}
