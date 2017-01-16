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

float DXVector3::GetMedian()const
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

