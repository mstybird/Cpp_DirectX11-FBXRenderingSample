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

float DXVector2::GetDistance()
{
	return D3DXVec2Length(this);
}

float DXVector2::GetDistance(DXVector2 & pPosition2)
{
	DXVector2 mixedPosition = *this - pPosition2;
	return D3DXVec2Length(&mixedPosition);
}

