#include "DXVector3.h"

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
