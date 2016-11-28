

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