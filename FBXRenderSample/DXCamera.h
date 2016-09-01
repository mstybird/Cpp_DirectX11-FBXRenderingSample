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

	D3DXMATRIX*GetMatrix();

	DXVector3 mEyePosition;	//ƒJƒƒ‰‚ÌˆÊ’u
	DXVector3 mLookPosition;//’‹“_
	DXVector3 mUpVector;	//“ªã•ûŒü
	D3DXMATRIX mMatrix;

	D3DXMATRIX* operator*() {
		return GetMatrix();
	}
private:
	
};