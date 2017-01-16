
#include<DXVector2.h>
inline DXVector3::DXVector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

inline DXVector3::DXVector3(int pX, int pY, int pZ) :D3DXVECTOR3(
	static_cast<float>(pX),
	static_cast<float>(pY),
	static_cast<float>(pZ)
)
{
}

inline DXVector3::DXVector3(float pX, float pY, float pZ) :D3DXVECTOR3(pX, pY, pZ)
{
}

inline DXVector3::DXVector3(double pX, double pY, double pZ) :
	D3DXVECTOR3(
	(float)pX,
		(float)pY,
		(float)pZ)
{
}

inline DXVector3::DXVector3(D3DXVECTOR3 pVector) :D3DXVECTOR3(pVector)
{
}

inline void DXVector3::Set(float pX, float pY, float pZ)
{
	x = pX;
	y = pY;
	z = pZ;
}

inline void DXVector3::Set(float pXYZ)
{
	x = pXYZ;
	y = pXYZ;
	z = pXYZ;
}

inline void DXVector3::Add(float pX, float pY, float pZ)
{
	x += pX;
	y += pY;
	z += pZ;
}

inline void DXVector3::Add(float pXYZ)
{
	x += pXYZ;
	y += pXYZ;
	z += pXYZ;
}

inline void DXVector3::Sub(float pX, float pY, float pZ)
{
	x -= pX;
	y -= pY;
	z -= pZ;
}

inline void DXVector3::Sub(float pXYZ)
{
	x -= pXYZ;
	y -= pXYZ;
	z -= pXYZ;
}

inline void DXVector3::Mul(float pX, float pY, float pZ)
{
	x *= pX;
	y *= pY;
	z *= pZ;
}

inline void DXVector3::Mul(float pXYZ)
{
	x *= pXYZ;
	y *= pXYZ;
	z *= pXYZ;
}

inline void DXVector3::Div(float pX, float pY, float pZ)
{
	x /= pX;
	y /= pY;
	z /= pZ;
}

inline void DXVector3::Div(float pXYZ)
{
	x /= pXYZ;
	y /= pXYZ;
	z /= pXYZ;
}

inline DXVector3 & DXVector3::operator=(const DXVector2 & pVector2D)
{
	// TODO: return ステートメントをここに挿入します
	x = pVector2D.x;
	y = pVector2D.y;
	z = 0.0f;
	return *this;
}

inline DXVector3 DXVector3::operator+(float pXYZ)const
{
	DXVector3 lResult{ *this };
	lResult.x += pXYZ;
	lResult.y += pXYZ;
	lResult.z += pXYZ;
	return lResult;
}

inline DXVector3 & DXVector3::operator+=(float pXYZ)
{
	x += pXYZ;
	y += pXYZ;
	z += pXYZ;
	return *this;
}

inline DXVector3 DXVector3::operator+(const D3DXVECTOR3 & pAddVector)const
{
	DXVector3 lResult{ *this };
	lResult.x += pAddVector.x;
	lResult.y += pAddVector.y;
	lResult.z += pAddVector.z;
	return lResult;
}

inline DXVector3 & DXVector3::operator+=(const D3DXVECTOR3 & pAddVector)
{
	x += pAddVector.x;
	y += pAddVector.y;
	z += pAddVector.z;
	return *this;
}



inline DXVector3 DXVector3::operator-(float pXYZ)const
{
	DXVector3 lResult{ *this };
	lResult.x -= pXYZ;
	lResult.y -= pXYZ;
	lResult.z -= pXYZ;
	return lResult;
}

inline DXVector3 & DXVector3::operator-=(float pXYZ)
{
	x -= pXYZ;
	y -= pXYZ;
	z -= pXYZ;
	return *this;
}

inline DXVector3 DXVector3::operator-(const D3DXVECTOR3 & pSubVector)const
{
	DXVector3 lResult{ *this };
	lResult.x -= pSubVector.x;
	lResult.y -= pSubVector.y;
	lResult.z -= pSubVector.z;
	return lResult;
}



inline DXVector3 & DXVector3::operator-=(const D3DXVECTOR3 & pSubVector)
{
	x -= pSubVector.x;
	y -= pSubVector.y;
	z -= pSubVector.z;
	return *this;
}

inline DXVector3 DXVector3::operator*(float pXYZ)const
{
	DXVector3 lResult{ *this };
	lResult.x *= pXYZ;
	lResult.y *= pXYZ;
	lResult.z *= pXYZ;
	return lResult;
}

inline DXVector3 & DXVector3::operator*=(float pXYZ)
{
	x *= pXYZ;
	y *= pXYZ;
	z *= pXYZ;
	return *this;
}

inline DXVector3 DXVector3::operator*(const D3DXVECTOR3 & pMulVector)const
{
	DXVector3 lResult{ *this };
	lResult.x *= pMulVector.x;
	lResult.y *= pMulVector.y;
	lResult.z *= pMulVector.z;
	return lResult;
}

inline DXVector3 & DXVector3::operator*=(const D3DXVECTOR3 & pMulVector)
{
	x *= pMulVector.x;
	y *= pMulVector.y;
	z *= pMulVector.z;
	return *this;
}

inline DXVector3 DXVector3::operator/(float pXYZ)const
{
	DXVector3 lResult{ *this };
	lResult.x /= pXYZ;
	lResult.y /= pXYZ;
	lResult.z /= pXYZ;
	return lResult;
}

inline DXVector3 & DXVector3::operator/=(float pXYZ)
{
	x /= pXYZ;
	y /= pXYZ;
	z /= pXYZ;
	return *this;
}

inline DXVector3 DXVector3::operator/(const D3DXVECTOR3 & pMulVector)const
{
	DXVector3 lResult{ *this };
	lResult.x /= pMulVector.x;
	lResult.y /= pMulVector.y;
	lResult.z /= pMulVector.z;
	return lResult;
}

inline DXVector3 & DXVector3::operator/=(const D3DXVECTOR3 & pMulVector)
{
	x /= pMulVector.x;
	y /= pMulVector.y;
	z /= pMulVector.z;
	return *this;
}
