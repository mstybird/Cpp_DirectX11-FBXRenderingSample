#pragma once
#include"DXVector3.h"
#include"StatusBase.h"

enum class EnemyLockOnType {
	UNKNOWN,	//�ʏ���
	CHARACTER,	//�{�[�����E�����T���Ώۂ�ǂ�������ꍇ
	BALL,		//�����Ă���{�[����ǂ�������ꍇ
	GOAL		//�{�[�����S�[���Ɏ����A��ꍇ
};

struct EnemyStatus:public StatusBase {
	//�G�𑨂��Ă�����true
	bool mTargetting;
	//�Ō�Ƀ^�[�Q�b�g�������ʒu
	DXVector3 mLastLookPosiion;
	EnemyLockOnType mLockOnType;

	bool mInitMoveToGoal;
	bool mInitMoveToBall;
	bool mInitMoveToBallTarget;


	EnemyStatus();

};
