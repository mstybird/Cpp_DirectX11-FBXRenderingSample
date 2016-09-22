#pragma once
#include"MSCollisionBase.h"

//�����̃��C�ƃ��b�V���̏Փ˔���
class MSCollisionRayPlane :public MSCollisionBase {
public:


	struct T_HitPolygon {

	};

	MSCollisionRayPlane();
	~MSCollisionRayPlane();
	//�I�u�W�F�N�g����w������Ƀ��C���΂�
	/*
		���C���΂����\�[�X(�����Ă�������ɔ�΂�)
		pRayPosition	:	���C�𔭎˂���ʒu���܂ރ��\�[�X
		pRayLength		:	���C�̕����ƒ���
		pRayTarget		:	���C�𓖂Ă�^�[�Q�b�g�̃��\�[�X
		pFbxTarget		:	���C�𓖂Ă�^�[�Q�b�g�̃��b�V��
		pOutDistance	:	���C�����������ꍇ�A��ԋ߂��|���S���Ƃ̋�����Ԃ�
	*/
	bool Collision(
		DX11RenderResource&pRayPosition,
		const DXVector3& pRayLength,
		DX11RenderResource&pRayTarget,
		MSFbxManager&pFbxTarget,
		float& pOutDistance,
		DXVector3&pNormal
		);

	//�L�����Ă�����W����ړ����������Ɍ������ă��C���΂�
	/*
		���C���΂����\�[�X(�����Ă�������ɔ�΂�)
		pRayPosition	:	���C�𔭎˂���ʒu���܂ރ��\�[�X
		pRayLength		:	���C�̕����ƒ���
		pRayTarget		:	���C�𓖂Ă�^�[�Q�b�g�̃��\�[�X
		pFbxTarget		:	���C�𓖂Ă�^�[�Q�b�g�̃��b�V��
		pOutDistance	:	���C�����������ꍇ�A��ԋ߂��|���S���Ƃ̋�����Ԃ�

	*/
	bool Collision(DX11RenderResource&pRayPosition,
		const float& pRayLength,
		DX11RenderResource&pRayTarget,
		MSFbxManager&pFbxTarget,
		float& pOutDistance,
		DXVector3&pOutNormal
	);

	//�����x�N�g�����v�Z���邽�߂Ƀt���[���̈ʒu���L��������
	void SetFramePosition(DX11RenderResource&pRayPosition);

	//���点�郊�\�[�X
	void Slip(DXVector3&pOutSlipVector, DX11RenderResource&pUpdPosition,DXVector3&pNormal);

private:

	//�L�������ʔ��������
	bool IsInside(
		 DXVector3&pvI,
		 DXVector3&pvA,
		 DXVector3&pvB,
		 DXVector3&pvC
		);

	std::unique_ptr<DXVector3> mFramePosition;
	//�Ō�ɓ��������|���S���̖@��
//	std::unique_ptr<DXVector3> mLastHitNormal;
};

//���C����ŋ������擾����