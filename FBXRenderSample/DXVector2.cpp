#include "DXVector2.h"


const DXVector2 DXVector2::sZeroVector{};

DXVector2::DXVector2()
{
	x = 0.0f;
	y = 0.0f;
}

DXVector2::DXVector2(float pX, float pY, float pZ):D3DXVECTOR2(pX,pY)
{
}

DXVector2::DXVector2(D3DXVECTOR2 pVector):D3DXVECTOR2(pVector)
{
}

void DXVector2::Set(float pX, float pY, float pZ)
{
	x = pX;
	y = pY;
}

void DXVector2::Set(float pXYZ)
{
	x = pXYZ;
	y = pXYZ;
}

void DXVector2::Add(float pX, float pY, float pZ)
{
	x += pX;
	y += pY;
}

void DXVector2::Add(float pXYZ)
{
	x += pXYZ;
	y += pXYZ;
}

void DXVector2::Sub(float pX, float pY, float pZ)
{
	x -= pX;
	y -= pY;
}

void DXVector2::Sub(float pXYZ)
{
	x -= pXYZ;
	y -= pXYZ;
}

void DXVector2::Mul(float pX, float pY, float pZ)
{
	x *= pX;
	y *= pY;
}

void DXVector2::Mul(float pXYZ)
{
	x *= pXYZ;
	y *= pXYZ;
}

void DXVector2::Div(float pX, float pY, float pZ)
{
	x /= pX;
	y /= pY;
}

void DXVector2::Div(float pXYZ)
{
	x /= pXYZ;
	y /= pXYZ;
}

float DXVector2::GetDistance()
{
	return D3DXVec2Length(this);
}

float DXVector2::GetDistance(DXVector2 & pPosition2)
{
	DXVector2 mixedPosition = *this - pPosition2;
	return D3DXVec2Length(&mixedPosition);
}

DXVector2 & DXVector2::operator+(float pXYZ)
{
	x += pXYZ;
	y += pXYZ;
	return *this;
}

DXVector2 & DXVector2::operator+=(float pXYZ)
{
	x += pXYZ;
	y += pXYZ;
	return *this;
}

DXVector2 & DXVector2::operator+(const D3DXVECTOR2 & pAddVector)
{
	x += pAddVector.x;
	y += pAddVector.y;
	return *this;
}

DXVector2 & DXVector2::operator+=(const D3DXVECTOR2 & pAddVector)
{
	x += pAddVector.x;
	y += pAddVector.y;
	return *this;
}



DXVector2 & DXVector2::operator-(float pXYZ)
{
	x -= pXYZ;
	y -= pXYZ;
	return *this;
}

DXVector2 & DXVector2::operator-=(float pXYZ)
{
	x -= pXYZ;
	y -= pXYZ;
	return *this;
}

DXVector2 & DXVector2::operator-(const D3DXVECTOR2 & pSubVector)
{
	x -= pSubVector.x;
	y -= pSubVector.y;
	return *this;
}

DXVector2 & DXVector2::operator-=(const D3DXVECTOR2 & pSubVector)
{
	x -= pSubVector.x;
	y -= pSubVector.y;
	return *this;
}

DXVector2 & DXVector2::operator*(float pXYZ)
{
	x *= pXYZ;
	y *= pXYZ;
	return *this;
}

DXVector2 & DXVector2::operator*=(float pXYZ)
{
	x *= pXYZ;
	y *= pXYZ;
	return *this;
}

DXVector2 & DXVector2::operator*(const D3DXVECTOR2 & pMulVector)
{
	x *= pMulVector.x;
	y *= pMulVector.y;
	return *this;
}

DXVector2 & DXVector2::operator*=(const D3DXVECTOR2 & pMulVector)
{
	x *= pMulVector.x;
	y *= pMulVector.y;
	return *this;
}

DXVector2 & DXVector2::operator/(float pXYZ)
{
	x /= pXYZ;
	y /= pXYZ;
	return *this;
}

DXVector2 & DXVector2::operator/=(float pXYZ)
{
	x /= pXYZ;
	y /= pXYZ;
	return *this;
}

DXVector2 & DXVector2::operator/(const D3DXVECTOR2 & pMulVector)
{
	x /= pMulVector.x;
	y /= pMulVector.y;
	return *this;
}

DXVector2 & DXVector2::operator/=(const D3DXVECTOR2 & pMulVector)
{
	x /= pMulVector.x;
	y /= pMulVector.y;
	return *this;
}
