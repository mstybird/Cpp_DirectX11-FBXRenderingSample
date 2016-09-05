#pragma once
#include<d3dx9.h>
class DXVector3 :public D3DXVECTOR3 {
public:

	enum TYPE_ANGLE {
		TYPE_DEGREE=0,
		TYPE_RADIAN
	};
	static float GetAngleX(TYPE_ANGLE pType, DXVector3&pPosition1, DXVector3&pPosition2);
	static float GetAngleY(TYPE_ANGLE pType, DXVector3&pPosition1, DXVector3&pPosition2);
	static float GetAngleZ(TYPE_ANGLE pType, DXVector3&pPosition1, DXVector3&pPosition2);
	//2点の垂直方向を求める
	static float GetVertical(TYPE_ANGLE pType, DXVector3&pPosition1, DXVector3&pPosition2);
	//2点の水平方向を求める
	static float GetHolizontal(TYPE_ANGLE pType, DXVector3&pPosition1, DXVector3&pPosition2);

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
	//このベクトルの距離を取得
	float GetDistance();
	//2点の距離を取得
	float GetDistance(DXVector3&pPosition2);
	//2本のベクトルの外積を求める
	void GetCross(DXVector3*DstCross, DXVector3&pVector);
	void GetCross(float*DstCross, DXVector3&pVector);
	//このベクトルを正規化する
	void Normalize();
	//正規化したベクトルを取得する
	void GetNormalize();
	//2点の角度を取得する
	float GetAngle(TYPE_ANGLE pType, DXVector3&pPosition2);

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

