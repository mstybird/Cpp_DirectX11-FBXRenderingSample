#pragma once
#include"MSProgress\MSProgressData.hxx"

enum class CharaStateFlag {
	ALIVE,		//生存中
	DEAD,		//死亡中
	RESPAWNWAIT	//リスポーン待機中
};

class GameObjectBase;
struct StatusBase {

	StatusBase();

	//現在捉えている敵
	GameObjectBase* mTarget;
	//ボールを所持しているか(所持していたらそのボールのアドレス)
	GameObjectBase* mBall;

	//HP
	MSProgress mHp;

	//エネルギー残量
	MSProgress mEnergy;

	//生存関係フラグ
	CharaStateFlag mLive;

//	int mEnergy;

};