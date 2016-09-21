#pragma once
#include"MSCollisionBase.h"

//�����̃��C�ƃ��b�V���̏Փ˔���
class MSCollisionRayPlane :public MSCollisionBase {
public:

	MSCollisionRayPlane();
	~MSCollisionRayPlane();
	//�I�u�W�F�N�g����w������Ƀ��C���΂�
	/*
		���C���΂����\�[�X(�����Ă�������ɔ�΂�)
	*/
	bool Collision(
		DX11RenderResource&pRayPosition,
		const DXVector3& pRayLength,
		DX11RenderResource&pRayTarget,
		MSFbxManager&pFbxTarget
		);
private:

	bool IsInside(
		 DXVector3&pvI,
		 DXVector3&pvA,
		 DXVector3&pvB,
		 DXVector3&pvC
		);

	DXVector3* vPoint;
	DXVector3* vDirection;
	DXVector3* vPos;
	float fYaw;
};