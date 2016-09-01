#pragma once
#include"DXVector3.h"
class DXCamera {
public:
	void SetEyeT(float pX, float pY, float pZ);
	void SetEyeT(D3DXVECTOR3 pPosition);
	void SetLookT(float pX, float pY, float pZ);
	void SetLookT(D3DXVECTOR3 pLookAt);
	void SetUpV(float pX, float pY, float pZ);
	void SetUpV(D3DXVECTOR3 pVector);

	void AddEyeT(float pX, float pY, float pZ);
	void AddEyeT(D3DXVECTOR3 pPosition);
	void AddLookT(float pX, float pY, float pZ);
	void AddLookT(D3DXVECTOR3 pLookAt);
	void AddUpV(float pX, float pY, float pZ);
	void AddUpV(D3DXVECTOR3 pVector);

	D3DXMATRIX*GetMatrix();

	DXVector3 mEyePosition;	//ÉJÉÅÉâÇÃà íu
	DXVector3 mLookPosition;//íçéãì_
	DXVector3 mUpVector;	//ì™è„ï˚å¸
private:
	
};