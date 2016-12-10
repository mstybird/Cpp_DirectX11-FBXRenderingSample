#pragma once
#include<d3dx9.h>
class DXVector2;
class DXVector3 :public D3DXVECTOR3 {
public:

	enum TYPE_ANGLE {
		TYPE_DEGREE=0,
		TYPE_RADIAN
	};
	static const DXVector3 sZeroVector;

	DXVector3();
	DXVector3(int pX, int pY, int pZ);
	DXVector3(float pX, float pY, float pZ);
	DXVector3(double pX, double pY, double pZ);
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
	//���̃x�N�g���̋������擾
	float GetDistance()const;
	//2�_�̋������擾
	float GetDistance(DXVector3&pPosition2);
	//2�{�̃x�N�g���̊O�ς����߂�
	void GetCross(DXVector3*DstCross, DXVector3&pVector);
	void GetCross(float*DstCross, DXVector3&pVector);
	//���̃x�N�g���𐳋K������
	void Normalize();
	//���K�������x�N�g�����擾����
	void GetNormalize();
	//3�̒l�̕��ς��擾
	float GetMedian();
	//2�_�̊p�x���擾����
	float GetAngle(TYPE_ANGLE pType, DXVector3&pPosition2);

	DXVector3&operator=(const DXVector2&pVector2D);

	DXVector3 operator+(float pXYZ)const;
	DXVector3&operator+=(float pXYZ);
	DXVector3 operator+(const D3DXVECTOR3&pAddVector)const;
	DXVector3&operator+=(const D3DXVECTOR3&pAddVector);

	DXVector3 operator-(float pXYZ)const;
	DXVector3&operator-=(float pXYZ);
	DXVector3 operator-(const D3DXVECTOR3&pSubVector)const;
	DXVector3&operator-=(const D3DXVECTOR3&pSubVector);

	DXVector3 operator*(float pXYZ)const;
	DXVector3&operator*=(float pXYZ);
	DXVector3 operator*(const D3DXVECTOR3&pMulVector)const;
	DXVector3&operator*=(const D3DXVECTOR3&pMulVector);

	DXVector3 operator/(float pXYZ)const;
	DXVector3&operator/=(float pXYZ);
	DXVector3 operator/(const D3DXVECTOR3&pMulVector)const;
	DXVector3&operator/=(const D3DXVECTOR3&pMulVector);

};

#include"DXVector3.inl"