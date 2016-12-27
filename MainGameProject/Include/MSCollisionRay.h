#pragma once
#include"MSCollisionBase.h"
class MSFbxObject;
class GameObjectBase;
//���C������p
class MSCollisionRay :public MSCollisionBase {
public:
	MSCollisionRay();
	//�w�肵�������Ƀ��C���΂�
	void SetRay(const DXVector3&pRayDirection);
	//�J�����������Ă�������Ń��C���쐬
	void SetRay( DXCamera&pRayCamera);

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
		MSFbxObject&pFbxManager
		);

private:
	//���C���˕���
	std::shared_ptr<DXVector3>mRayDirection;
};

class MS2CollisionRay {
	static float Collision(
		DXCamera*aCamera,
		GameObjectBase*aCollision
	);
};