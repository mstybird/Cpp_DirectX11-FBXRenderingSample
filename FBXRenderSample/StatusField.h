#pragma once
#include"GameObjectBase.h"
//�t�B�[���h���
struct StatusField {
	//�N���{�[�����������Ă��邩
	GameObjectBase* mBallHoldChara;
	//�t�B�[���h�Ƀ{�[�������邩�ǂ���
	bool mBallIsField;
	//�{�[�����
	GameObjectBase* mBall;

	StatusField();
	void SetBallHolder(GameObjectBase*pBallHolder);
	void RespawnBall();

};