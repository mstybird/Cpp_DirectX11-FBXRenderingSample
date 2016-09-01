#pragma once
#include<d3dx9.h>
class DXVector3 :public D3DXVECTOR3 {
public:

	DXVector3();
	DXVector3(float pX, float pY, float pZ);
	DXVector3(D3DXVECTOR3 pvector);


	void Set(float pX, float pY, float pZ);
	void Set(D3DXVECTOR3 p);
	void SetX(float pX);
	void SetY(float pY);
	void SetZ(float pZ);
	void Add(float pX, float pY, float pZ);
	void Add(D3DXVECTOR3 p);
	void AddX(float pX);
	void AddY(float pY);
	void AddZ(float pZ);
};

