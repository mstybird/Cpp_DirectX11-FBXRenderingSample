#pragma once
#include"DXVector3.h"
#include"StatusBase.h"
struct EnemyStatus:public StatusBase {
	//�G�𑨂��Ă�����true
	bool mTargetting;
	//�Ō�Ƀ^�[�Q�b�g�������ʒu
	DXVector3 mLastLookPosiion;

	EnemyStatus();

};
