#pragma once
#include<memory>
//�Փ˔���֌W
class DXVector3;
class MSFbxManager;
struct FBXModelData;
class DXMatrix;
class DXWorld;
class DXCamera;
class DX11RenderResource;
class MSCollisionBase {
public:
	MSCollisionBase();
	MSCollisionBase(const MSCollisionBase& _co);
	~MSCollisionBase();
protected:
	std::unique_ptr<DXMatrix>mGlobalPosition;
};

//���E������p
class MSCollisionSphere :public MSCollisionBase {
public:
	MSCollisionSphere();
	//�w�肵��FBX���b�V�����g���ċ��E�����쐬����
	void CreateCollision(const FBXModelData&pMeshmconst,const DXMatrix&pGlobalPosition);
	//���E���Ɠ����蔻����s��
	static bool Collision(
		MSCollisionSphere&pCollision1,
		const DXWorld&pWorldMatrix1,
		MSCollisionSphere&pCollision2,
		const DXWorld&pWorldMatrix2
		);
private:
	float mRadius;//�����蔻�蔼�a
	std::unique_ptr<DXVector3>mCenter;//���S���W
};

//���C������p
class MSCollisionRay :public MSCollisionBase {
public:
	MSCollisionRay();
	//�w�肵�������Ƀ��C���΂�
	void SetRay(const DXVector3&pRayDirection);
	//�J�����������Ă�������Ń��C���쐬
	void SetRay(const DXCamera&pRayCamera);

	/*
		���C����
		pRay			:
		pRayPosition	:���C���ˈʒu���܂ރ��\�[�X
		pResource		:�^�[�Q�b�g�̃��\�[�X
		pFbxManager		:�^�[�Q�b�g�̃��b�V�����܂܂��FBX�}�l�[�W��
	*/
	static bool Collision(
		const MSCollisionRay&pRay,
		DX11RenderResource&pRayPosition,
		DX11RenderResource&pResource,
		MSFbxManager&pFbxManager
		);

private:
	//���C���˕���
	std::shared_ptr<DXVector3>mRayDirection;
};

//���C�s�b�L���O����p
class MSCollisionRayPicking :public MSCollisionBase {
public:

	MSCollisionRayPicking();

	//���݂̈ʒu���L������
	void SetFramePosition(DX11RenderResource&pRayPosion);
	//���ׂ点��ꍇ��true��^����
	void SetSlipFlag(bool pSlipFlag);
	//���݂̊���t���O���擾
	bool GetSlipFlag();

	//���O�̃t���[���̈ʒu���g���ă��C�s�b�L���O����
	/*
		pResultPos:	SlipFlag=true	:��������̍ŏI�I�ȍ��W
					SlipFlag=false	:�Փˈʒu��Ԃ��̂�
		pRayAfterPos:�^�[�Q�b�g�ɏՓ˗\��̃��\�[�X
		pRayTarget:�^�[�Q�b�g�ƂȂ郊�\�[�X
		pFbxTarget:�^�[�Q�b�g�ƂȂ�FBX�f�[�^
	*/
	bool Collision(
		DXVector3&pResultPosition,
		DX11RenderResource&pRayPosition,
		DX11RenderResource&pRayTarget,
		MSFbxManager&pFbxTarget
	);
private:
	//���点�邩�ǂ���
	bool mSlipFlag;
	//���O�̃t���[���̍��W
	std::unique_ptr<DXVector3>mPosBefore;
};