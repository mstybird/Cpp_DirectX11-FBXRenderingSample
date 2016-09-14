#pragma once
#include<memory>
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
	void SetCamera(const std::weak_ptr<DXWorld>&pEyePosition, const std::weak_ptr<DXWorld>&pLookAtPosition);

	void CreateRay(DXVector3&pOutRay, const DXVector3&pRayDirection)const;

	void Translation(TYPEMOVE pType, float pSpeed, const DXVector3&pDirection, bool pLockoned = false);


	//�J�����̉�]
	void Rotate(float pX, float pY, float pZ);

	std::weak_ptr<DXMatrix>GetMatrix();

	std::shared_ptr<DXVector3>mEyePosition;	//�J�����̈ʒu
	std::shared_ptr<DXVector3>mLookPosition;//�����_
	std::shared_ptr<DXVector3>mUpVector;	//�������
	std::shared_ptr<DXVector3>mRotate;		//�J�����̊p�x
	std::shared_ptr<DXMatrix>mMatrix;
	

	std::weak_ptr<DXMatrix> operator*() {
		return GetMatrix();
	}
private:
	
};