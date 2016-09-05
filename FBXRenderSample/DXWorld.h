#pragma once
#include"DXVector3.h"
//���[���h��ԍ��W�n�N���X
class DXWorld {
public:
	//���W�ړ��p�^�[��
	enum TYPEMOVE {
		TYPE_PARALLEL=0,	//���s�ړ�
		TYPE_ROTATE=1	//��]�ړ�
	};

	DXWorld();

	///�ړ��s�񑀍�
	void SetT(float pX, float pY, float pZ);
	void SetT(D3DXVECTOR3 pPosition);
	void AddT(float pX, float pY, float pZ);
	void AddT(D3DXVECTOR3 pPosition);
	void AddT(TYPEMOVE pType, float pSpeed, DXVector3 pDirection = { 0,0,1 });
	void AddTRotated(float pSpeed, DXVector3 pDirection = { 0,0,1 });


	void SubT(float pX, float pY, float pZ);
	void SubT(D3DXVECTOR3 pPosition);
	void MulT(float pX, float pY, float pZ);
	void MulT(D3DXVECTOR3 pPosition);
	void DivT(float pX, float pY, float pZ);
	void DivT(D3DXVECTOR3 pPosition);

	///�ړ��O�̉�]�s��
	void SetRC(float pX, float pY, float pZ);
	void SetRC(D3DXVECTOR3 pPosition);
	void AddRC(float pX, float pY, float pZ);
	void AddRC(D3DXVECTOR3 pPosition);
	void SubRC(float pX, float pY, float pZ);
	void SubRC(D3DXVECTOR3 pPosition);
	void MulRC(float pX, float pY, float pZ);
	void MulRC(D3DXVECTOR3 pPosition);
	void DivRC(float pX, float pY, float pZ);
	void DivRC(D3DXVECTOR3 pPosition);

	//�ړ���̉�]�s��
	void SetRT(float pX, float pY, float pZ);
	void SetRT(D3DXVECTOR3 pPosition);
	void AddRT(float pX, float pY, float pZ);
	void AddRT(D3DXVECTOR3 pPosition);
	void SubRT(float pX, float pY, float pZ);
	void SubRT(D3DXVECTOR3 pPosition);
	void MulRT(float pX, float pY, float pZ);
	void MulRT(D3DXVECTOR3 pPosition);
	void DivRT(float pX, float pY, float pZ);
	void DivRT(D3DXVECTOR3 pPosition);

	//�g��s��
	void SetS(float pX, float pY, float pZ);
	void SetS(D3DXVECTOR3 pScaling);
	void AddS(float pX, float pY, float pZ);
	void AddS(D3DXVECTOR3 pScaling);
	void SubS(float pX, float pY, float pZ);
	void SubS(D3DXVECTOR3 pScaling);
	void MulS(float pX, float pY, float pZ);
	void MulS(D3DXVECTOR3 pScaling);
	void DivS(float pX, float pY, float pZ);
	void DivS(D3DXVECTOR3 pScaling);

	D3DXMATRIX* GetMatrix();
	DXVector3 mPosition;			//�ʒu
	DXVector3 mRotationCenter;	//��](�ړ��s��K�p�O)
	DXVector3 mRotationTransed;	//��]�ړ��s��K�p��)
	DXVector3 mScale;				//�g�嗦

	D3DXMATRIX mMatrix;			//�v�Z�p


};
