#pragma once

//#include<d3dx9.h>
class DXVector3;
class DXMatrix;
class DXWorld;
class DXCamera {
public:
	static const DXVector3 sUpVector;
	//�J�����̈ړ��^�C�v
	enum TYPEMOVE {
		TYPE_NORMAL = 0,	//�����Ɋ֌W�Ȃ�XYZ�l�Œ��ڈړ�
		TYPE_PARALLEL,		//���݌����Ă����������Ɉړ�
	};
	//�ړ�����
	class DIRECTION_TYPE {
	public:
		static const DXVector3 LEFTRIGHT;	//���E
		static const DXVector3 UPDOWN;		//�㉺
		static const DXVector3 FRONTBACK;	//��O��
	};

	//�R���X�g���N�^
	DXCamera();
	~DXCamera();

	//���_�ƃ^�[�Q�b�g(�����_)�̐ݒ�
	void SetCamera(const DXWorld&pEyePosition, const DXWorld&pLookAtPosition);

	void Translation(TYPEMOVE pType, float pSpeed, const DXVector3&pDirection, bool pLockoned = false);


	//�J�����̉�]
	void Rotate(float pX, float pY, float pZ);

	DXMatrix*GetMatrix();

	DXVector3 *mEyePosition;	//�J�����̈ʒu
	DXVector3 *mLookPosition;//�����_
	DXVector3 *mUpVector;	//�������
	DXVector3 *mRotate;		//�J�����̊p�x
	DXMatrix *mMatrix;
	

	DXMatrix* operator*() {
		return GetMatrix();
	}
private:
	
};