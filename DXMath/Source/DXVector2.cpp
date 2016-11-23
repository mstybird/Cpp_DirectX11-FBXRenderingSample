#include "DXVector2.h"



const DXVector2 DXVector2::sZeroVector{};

DXVector2::DXVector2()
{
	x = 0.0f;
	y = 0.0f;
}


DXVector2::DXVector2(float pX, float pY):D3DXVECTOR2(pX,pY)
{
}

DXVector2::DXVector2(D3DXVECTOR2 pVector):D3DXVECTOR2(pVector)
{
}

void DXVector2::Set(float pX, float pY)
{
	x = pX;
	y = pY;
}

void DXVector2::Set(float pXY)
{
	x = pXY;
	y = pXY;
}

void DXVector2::Add(float pX, float pY)
{
	x += pX;
	y += pY;
}

void DXVector2::Add(float pXY)
{
	x += pXY;
	y += pXY;
}

void DXVector2::Sub(float pX, float pY)
{
	x -= pX;
	y -= pY;
}

void DXVector2::Sub(float pXY)
{
	x -= pXY;
	y -= pXY;
}

void DXVector2::Mul(float pX, float pY)
{
	x *= pX;
	y *= pY;
}

void DXVector2::Mul(float pXY)
{
	x *= pXY;
	y *= pXY;
}

void DXVector2::Div(float pX, float pY)
{
	x /= pX;
	y /= pY;
}

void DXVector2::Div(float pXY)
{
	x /= pXY;
	y /= pXY;
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

DXVector2 DXVector2::operator+(float pXY)const
{
	DXVector2 lClone{ *this };
	lClone.x += pXY;
	lClone.y += pXY;
	return lClone;
}

DXVector2 & DXVector2::operator+=(float pXY)
{
	x += pXY;
	y += pXY;
	return *this;
}

DXVector2 DXVector2::operator+(const D3DXVECTOR2 & pAddVector)const
{
	DXVector2 lClone{ *this };
	lClone.x += pAddVector.x;
	lClone.y += pAddVector.y;
	return lClone;
}

DXVector2 & DXVector2::operator+=(const D3DXVECTOR2 & pAddVector)
{
	x += pAddVector.x;
	y += pAddVector.y;
	return *this;
}



DXVector2 DXVector2::operator-(float pXY)const
{
	DXVector2 lClone{ *this };
	lClone.x -= pXY;
	lClone.y -= pXY;
	return lClone;
}

DXVector2 & DXVector2::operator-=(float pXY)
{
	x -= pXY;
	y -= pXY;
	return *this;
}

DXVector2 DXVector2::operator-(const D3DXVECTOR2 & pSubVector)const
{
	DXVector2 lClone{ *this };
	lClone.x -= pSubVector.x;
	lClone.y -= pSubVector.y;
	return lClone;
}

DXVector2 & DXVector2::operator-=(const D3DXVECTOR2 & pSubVector)
{
	x -= pSubVector.x;
	y -= pSubVector.y;
	return *this;
}

DXVector2 DXVector2::operator*(float pXY)const
{
	DXVector2 lClone{ *this };
	lClone.x *= pXY;
	lClone.y *= pXY;
	return lClone;
}

DXVector2 & DXVector2::operator*=(float pXY)
{
	x *= pXY;
	y *= pXY;
	return *this;
}

DXVector2 DXVector2::operator*(const D3DXVECTOR2 & pMulVector)const
{
	DXVector2 lClone{ *this };
	lClone.x *= pMulVector.x;
	lClone.y *= pMulVector.y;
	return lClone;
}

DXVector2 & DXVector2::operator*=(const D3DXVECTOR2 & pMulVector)
{
	x *= pMulVector.x;
	y *= pMulVector.y;
	return *this;
}

DXVector2 DXVector2::operator/(float pXY)const
{
	DXVector2 lClone{ *this };
	lClone.x /= pXY;
	lClone.y /= pXY;
	return lClone;
}

DXVector2 & DXVector2::operator/=(float pXY)
{
	this->x /= pXY;
	this->y /= pXY;
	return *this;
}

DXVector2 DXVector2::operator/(const D3DXVECTOR2 & pMulVector)const
{
	DXVector2 lClone{ *this };
	lClone.x /= pMulVector.x;
	lClone.y /= pMulVector.y;
	return lClone;

}

DXVector2 & DXVector2::operator/=(const D3DXVECTOR2 & pMulVector)
{
	x /= pMulVector.x;
	y /= pMulVector.y;
	return *this;
}