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

void DXVector3::Set(D3DXVECTOR3 p)
{
	x = p.x;
	y = p.y;
	z = p.z;
}

void DXVector3::SetX(float pX)
{
	x = pX;
}

void DXVector3::SetY(float pY)
{
	y = pY;
}

void DXVector3::SetZ(float pZ)
{
	z = pZ;
}

void DXVector3::Add(float pX, float pY, float pZ)
{
	x += pX;
	y += pY;
	z += pZ;
}

void DXVector3::Add(D3DXVECTOR3 p)
{
	(*this) += p;
}

void DXVector3::AddX(float pX)
{
	x += pX;
}

void DXVector3::AddY(float pY)
{
	y += pY;
}

void DXVector3::AddZ(float pZ)
{
	z = pZ;
}

//DXVector3 & DXVector3::operator=(const D3DXVECTOR3 & pValue)
//{
//	// TODO: return ステートメントをここに挿入します
//	x = pValue.x;
//	y = pValue.y;
//	z = pValue.z;
//	return *this;
//}

DXVector3 & DXVector3::operator=(const DXVector3 & pValue)
{
	// TODO: return ステートメントをここに挿入します
	*this = pValue;
	return *this;
}

