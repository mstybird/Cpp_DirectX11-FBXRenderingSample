#pragma once
#include<d3dx9.h>
class DXVector3 :public D3DXVECTOR3 {
public:

	DXVector3();
	DXVector3(float pX, float pY, float pZ);
	DXVector3(D3DXVECTOR3 pvector);


	void Set(float pX, float pY, float pZ); 
	void Set(float pXYZ);
	void Add(float pX, float pY, float pZ);
	void Add(float pXYZ);
	void Sub(float pX, float pY, float pZ);
	void Sub(float pXYZ);
	void Mul(float pX, float pY, float pZ);
	void Mul(float pXYZ);
	void Div(float pX, float pY, float pZ);
	void Div(float pXYZ);


	DXVector3&operator+(float pXYZ);
	DXVector3&operator+=(float pXYZ);
	DXVector3&operator+(D3DXVECTOR3&pAddVector);
	DXVector3&operator+=(D3DXVECTOR3&pAddVector);

	DXVector3&operator-(float pXYZ);
	DXVector3&operator-=(float pXYZ);
	DXVector3&operator-(D3DXVECTOR3&pSubVector);
	DXVector3&operator-=(D3DXVECTOR3&pSubVector);

	DXVector3&operator*(float pXYZ);
	DXVector3&operator*=(float pXYZ);
	DXVector3&operator*(D3DXVECTOR3&pMulVector);
	DXVector3&operator*=(D3DXVECTOR3&pMulVector);

	DXVector3&operator/(float pXYZ);
	DXVector3&operator/=(float pXYZ);
	DXVector3&operator/(D3DXVECTOR3&pMulVector);
	DXVector3&operator/=(D3DXVECTOR3&pMulVector);

};

