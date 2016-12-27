#pragma once
//#include"DXVector3.h"
//���[���h��ԍ��W�n�N���X
#include<DXMath.hpp>
#include<d3dx9.h>
#include<memory>
class DXWorld {
public:
	//���W�ړ��p�^�[��
	enum TYPEMOVE {
		TYPE_PARALLEL=0,	//���s�ړ�
		TYPE_ROTATE=1	//��]�ړ�
	};

	DXWorld();
	~DXWorld();
	///�ړ��s�񑀍�
	void SetT(float pX, float pY, float pZ);
	void SetT(DXVector3 pPosition);
	void AddT(float pX, float pY, float pZ);
	void AddT(DXVector3 pPosition);
	void AddT(TYPEMOVE pType, float pSpeed, DXVector3 pDirection);
	void AddTRotated(float pSpeed, DXVector3 pDirection);


	void SubT(float pX, float pY, float pZ);
	void SubT(DXVector3 pPosition);
	void MulT(float pX, float pY, float pZ);
	void MulT(DXVector3 pPosition);
	void DivT(float pX, float pY, float pZ);
	void DivT(DXVector3 pPosition);

	///�ړ��O�̉�]�s��
	void SetRC(float pX, float pY, float pZ);
	void SetRC(DXVector3 pPosition);
	void AddRC(float pX, float pY, float pZ);
	void AddRC(DXVector3 pPosition);
	void SubRC(float pX, float pY, float pZ);
	void SubRC(DXVector3 pPosition);
	void MulRC(float pX, float pY, float pZ);
	void MulRC(DXVector3 pPosition);
	void DivRC(float pX, float pY, float pZ);
	void DivRC(DXVector3 pPosition);

	//�ړ���̉�]�s��
	void SetRT(float pX, float pY, float pZ);
	void SetRT(DXVector3 pPosition);
	void AddRT(float pX, float pY, float pZ);
	void AddRT(DXVector3 pPosition);
	void SubRT(float pX, float pY, float pZ);
	void SubRT(DXVector3 pPosition);
	void MulRT(float pX, float pY, float pZ);
	void MulRT(DXVector3 pPosition);
	void DivRT(float pX, float pY, float pZ);
	void DivRT(DXVector3 pPosition);

	//�g��s��
	void SetS(float pX, float pY, float pZ);
	void SetS(DXVector3 pScaling);
	void AddS(float pX, float pY, float pZ);
	void AddS(DXVector3 pScaling);
	void SubS(float pX, float pY, float pZ);
	void SubS(DXVector3 pScaling);
	void MulS(float pX, float pY, float pZ);
	void MulS(DXVector3 pScaling);
	void DivS(float pX, float pY, float pZ);
	void DivS(DXVector3 pScaling);

	DXMatrix* GetMatrix();
	DXVector3 mPosition;			//�ʒu
	DXVector3 mRotationCenter;	//��](�ړ��s��K�p�O)
	DXVector3 mRotationTransed;	//��]�ړ��s��K�p��)
	DXVector3 mScale;				//�g�嗦

	DXMatrix mMatrix;			//�v�Z�p


};
