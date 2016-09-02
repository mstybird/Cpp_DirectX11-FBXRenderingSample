#pragma once
#include"DXVector3.h"
#include"DXWorld.h"
//class DX11RenderResource;
class DXCamera {
public:
	///カメラの位置
	void SetEyeT(float pX, float pY, float pZ);
	void SetEyeT(D3DXVECTOR3 pPosition);
	void SetEyeT(DXWorld pEyeTarget);
	
	///カメラの注視点
	void SetLookT(float pX, float pY, float pZ);
	void SetLookT(D3DXVECTOR3 pLookAt);
	void SetLookT(DXWorld pEyeTarget);

	///カメラの向き
	void SetUpV(float pX, float pY, float pZ);
	void SetUpV(D3DXVECTOR3 pVector);

	D3DXMATRIX*GetMatrix();

	DXVector3 mEyePosition;	//カメラの位置
	DXVector3 mLookPosition;//注視点
	DXVector3 mUpVector;	//頭上方向
	D3DXMATRIX mMatrix;

	D3DXMATRIX* operator*() {
		return GetMatrix();
	}
private:
	
};