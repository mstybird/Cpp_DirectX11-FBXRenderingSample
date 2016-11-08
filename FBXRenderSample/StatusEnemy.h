#pragma once
#include"DXVector3.h"
#include"StatusBase.h"
#include"AI\EnemyAI.h"
enum class EnemyLockOnType {
	UNKNOWN,	//�ʏ���
	CHARACTER,	//�{�[�����E�����T���Ώۂ�ǂ�������ꍇ
	BALL,		//�����Ă���{�[����ǂ�������ꍇ
	GOAL		//�{�[�����S�[���Ɏ����A��ꍇ
};

//���݂�AI
enum class eNowAIStatus {

};

struct EnemyStatus:public StatusBase {

	EnemyStatus();
	virtual ~EnemyStatus();
	//�G�𑨂��Ă�����true
	bool mTargetting;
	//�Ō�Ƀ^�[�Q�b�g�������ʒu
	DXVector3 mLastLookPosiion;

	int mGoalIndex;				//�S�[���C���f�N�X
	bool mIsTargetingBall;		//�{�[���𑨂���(����ɓ�������)
	//bool mInitMoveToGoal;		//���\�b�h�������p
	//bool mInitMoveToBall;		//���\�b�h�������p
	//bool mInitMoveToBallTarget;	//���\�b�h�������p
	EnemyAIType::Type mAIType;
	//�����Ǐ]�t���[���J�E���^
	int32_t mAllyComplianceCount;


};
