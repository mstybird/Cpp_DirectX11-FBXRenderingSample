#pragma once
#include"MSCollisionBase.h"
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