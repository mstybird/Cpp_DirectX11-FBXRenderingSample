#pragma once
#include"DXVector3.h"
//���[���h��ԍ��W�n�N���X
class DXWorld {
public:
	void SetT(float pX, float pY, float pZ);
	void SetT(D3DXVECTOR3 pPosition);

	void SetRCenter(float pX, float pY, float pZ);
	void SetRCenter(D3DXVECTOR3 pPosition);
	
	void SetRT(float pX, float pY, float pZ);
	void SetRT(D3DXVECTOR3 pPosition);
	
	void SetS(float pX, float pY, float pZ);
	void SetS(D3DXVECTOR3 pPosition);

	D3DXMATRIX* GetMatrix();
	DXVector3 mPosition;			//�ʒu
	DXVector3 mRotationCenter;	//��](�ړ��s��K�p�O)
	DXVector3 mRotationTransed;	//��]�ړ��s��K�p��)
	DXVector3 mScale;				//�g�嗦

	D3DXMATRIX mMatrix;			//�v�Z�p

};
