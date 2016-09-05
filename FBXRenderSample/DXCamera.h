#pragma once
#include"DXVector3.h"
#include"DXWorld.h"
//class DX11RenderResource;
class DXCamera {
public:
	//�J�����̈ړ��^�C�v
	enum TYPEMOVE {
		TYPE_PARALLEL = 0,	//���s�ړ�
		TYPE_TARGET = 1,	//��]�ړ�
		TYPE_ROTATE = 2		//���݌����Ă�������Ɉړ�
	};

	
	///�J�����̈ʒu
	void SetEyeT(float pX, float pY, float pZ);
	void SetEyeT(D3DXVECTOR3 pPosition);
	//���[���h�s�񂩂�ʒu�擾
	void SetEyeT(DXWorld pEyeTarget);
	
	///�J�����̒����_
	void SetLookT(float pX, float pY, float pZ);
	void SetLookT(D3DXVECTOR3 pLookAt);
	//���[���h�s�񂩂璍���_�擾
	void SetLookT(DXWorld pEyeTarget);

	//�J�����̈ړ�
	void AddEyeT(float pSpeed, DXVector3 pDirection = { 0,0,1 });
	void AddLookT(float pSpeed, DXVector3 pDirection = { 0,0,1 });
	//���s�ړ�
	void AddTranslation(TYPEMOVE pType,float pSpeed, DXVector3 pDirection = { 0,0,1 });
	//�J�����̉�]
	void Rotate(float pX, float pY, float pZ);

	///�J�����̌���
	void SetUpV(float pX, float pY, float pZ);
	void SetUpV(D3DXVECTOR3 pVector);

	D3DXMATRIX*GetMatrix();

	DXVector3 mEyePosition;	//�J�����̈ʒu
	DXVector3 mLookPosition;//�����_
	DXVector3 mUpVector;	//�������
	DXVector3 mRotate;		//�J�����̊p�x
	D3DXMATRIX mMatrix;

	D3DXMATRIX* operator*() {
		return GetMatrix();
	}
private:
	
};