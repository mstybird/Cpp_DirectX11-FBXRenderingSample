#pragma once
#include<cstdint>
#include"MSProgressData.hxx"

enum class CharaStateFlag {
	ALIVE,		//生存中
	DEAD,		//死亡中
	RESPAWNWAIT	//リスポーン待機中
};

namespace ValueStatusBase {
	static const char* cHp = "Hp";
	static const char* cEnergy = "Energy";
}

class GameObjectBase;
struct StatusBase {

	StatusBase();
	virtual ~StatusBase() {};
	//現在捉えている敵
	GameObjectBase* mTargetChara;//--[Lua]mLockonEnemy/mInsightEnemy/mBallHoldTarget
	//ボールを所持しているか(所持していたらそのボールのアドレス)
	GameObjectBase* mBall;	//--[Lua]mBallHoldMe

	//味方が近くにいるかどうか
	bool mAllyNear;
	//ボールを持っている味方が近くにいるか
	bool mBallAllyNear;
	//HP
	MSProgress mHp;

	//エネルギー残量
	MSProgress mEnergy;		//--[Lua]mChargedEnergy

	//生存関係フラグ
	CharaStateFlag mLive;



//	int mEnergy;

};