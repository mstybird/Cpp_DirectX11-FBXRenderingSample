#pragma once
#include"DXVector3.h"
#include"DXWorld.h"
//class DX11RenderResource;
class DXCamera {
public:
	///�J�����̈ʒu
	void SetEyeT(float pX, float pY, float pZ);
	void SetEyeT(D3DXVECTOR3 pPosition);
	void SetEyeT(DXWorld pEyeTarget);
	
	///�J�����̒����_
	void SetLookT(float pX, float pY, float pZ);
	void SetLookT(D3DXVECTOR3 pLookAt);
	void SetLookT(DXWorld pEyeTarget);

	///�J�����̌���
	void SetUpV(float pX, float pY, float pZ);
	void SetUpV(D3DXVECTOR3 pVector);

	D3DXMATRIX*GetMatrix();

	DXVector3 mEyePosition;	//�J�����̈ʒu
	DXVector3 mLookPosition;//�����_
	DXVector3 mUpVector;	//�������
	D3DXMATRIX mMatrix;

	D3DXMATRIX* operator*() {
		return GetMatrix();
	}
private:
	
};