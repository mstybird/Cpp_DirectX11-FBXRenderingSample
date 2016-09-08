#pragma once
#include"DXVector3.h"
#include"DXWorld.h"
//class DX11RenderResource;
class DXCamera {
public:
	static const D3DXVECTOR3 sUpVector;
	//�J�����̈ړ��^�C�v
	enum TYPEMOVE {
		TYPE_NORMAL = 0,	//�����Ɋ֌W�Ȃ�XYZ�l�Œ��ڈړ�
		TYPE_PARALLEL,		//���݌����Ă����������Ɉړ�
	};
	//�ړ�����
	class DIRECTION_TYPE {
	public:
		static const D3DXVECTOR3 LEFTRIGHT;	//���E
		static const D3DXVECTOR3 UPDOWN;		//�㉺
		static const D3DXVECTOR3 FRONTBACK;	//��O��
	};

	//�R���X�g���N�^
	DXCamera();

	//���_�ƃ^�[�Q�b�g(�����_)�̐ݒ�
	void SetCamera(const DXWorld&pEyePosition, const DXWorld&pLookAtPosition);

	void Translation(TYPEMOVE pType, float pSpeed, const D3DXVECTOR3&pDirection = DIRECTION_TYPE::FRONTBACK, bool pLockoned = false);


	//�J�����̉�]
	void Rotate(float pX, float pY, float pZ);

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