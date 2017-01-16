#pragma once
#include<d3dx9.h>
class DXVector2 :public D3DXVECTOR2 {
public:
	static const DXVector2 sZeroVector;

	DXVector2();
	DXVector2(float pX, float pY);
	DXVector2(D3DXVECTOR2 pvector);


	void Set(float pX, float pY);
	void Set(float pXYZ);
	void Add(float pX, float pY);
	void Add(float pXYZ);
	void Sub(float pX, float pY);
	void Sub(float pXYZ);
	void Mul(float pX, float pY);
	void Mul(float pXYZ);
	void Div(float pX, float pY);
	void Div(float pXYZ);
	//���̃x�N�g���̋������擾
	float GetDistance();
	//2�_�̋������擾
	float GetDistance(DXVector2&pPosition2);
	//���̃x�N�g���𐳋K������
	void Normalize();
	//���K�������x�N�g�����擾����
	void GetNormalize();

	DXVector2 operator+(float pXY)const;
	DXVector2&operator+=(float pXY);
	DXVector2 operator+(const D3DXVECTOR2&pAddVector)const;
	DXVector2&operator+=(const D3DXVECTOR2&pAddVector);

	DXVector2 operator-(float pXY)const;
	DXVector2&operator-=(float pXY);
	DXVector2 operator-(const D3DXVECTOR2&pSubVector)const;
	DXVector2&operator-=(const D3DXVECTOR2&pSubVector);

	DXVector2 operator*(float pXY)const;
	DXVector2&operator*=(float pXY);
	DXVector2 operator*(const D3DXVECTOR2&pMulVector)const;
	DXVector2&operator*=(const D3DXVECTOR2&pMulVector);

	DXVector2 operator/(float pXY)const;
	DXVector2&operator/=(float pXY);
	DXVector2 operator/(const D3DXVECTOR2&pMulVector)const;
	DXVector2&operator/=(const D3DXVECTOR2&pMulVector);
};

#include"DXVector2.inl"