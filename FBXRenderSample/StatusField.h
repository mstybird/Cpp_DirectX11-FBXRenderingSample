#pragma once
#include"GameObjectBase.h"
//フィールド情報
struct StatusField {
	//誰がボールを所持しているか
	GameObjectBase* mBallHoldChara;
	//フィールドにボールがあるかどうか
	bool mBallIsField;
	//ボール情報
	GameObjectBase* mBall;

	StatusField();
	void SetBallHolder(GameObjectBase*pBallHolder);
	void RespawnBall();

};