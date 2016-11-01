#pragma once
#include"DXVector3.h"
#include"StatusBase.h"

enum class EnemyLockOnType {
	UNKNOWN,	//通常状態
	CHARACTER,	//ボールを拾った探索対象を追っかける場合
	BALL,		//落ちているボールを追っかける場合
	GOAL		//ボールをゴールに持ち帰る場合
};

struct EnemyStatus:public StatusBase {
	//敵を捉えていたらtrue
	bool mTargetting;
	//最後にターゲットを見た位置
	DXVector3 mLastLookPosiion;

	int mGoalIndex;				//ゴールインデクス
	bool mIsTargetingBall;		//ボールを捉えた(視野に入ったか)
	bool mInitMoveToGoal;		//メソッド初期化用
	bool mInitMoveToBall;		//メソッド初期化用
	bool mInitMoveToBallTarget;	//メソッド初期化用


	EnemyStatus();

};
