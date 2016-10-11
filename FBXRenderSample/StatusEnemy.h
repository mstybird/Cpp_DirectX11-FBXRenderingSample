#pragma once
#include"DXVector3.h"
#include"StatusBase.h"
struct EnemyStatus:public StatusBase {
	//敵を捉えていたらtrue
	bool mTargetting;
	//最後にターゲットを見た位置
	DXVector3 mLastLookPosiion;

	EnemyStatus();

};
