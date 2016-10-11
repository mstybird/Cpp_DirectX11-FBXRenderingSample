#pragma once
class GameObjectBase;
struct StatusBase {

	StatusBase();

	//現在捉えている敵
	GameObjectBase* mTarget;
	//エネルギー残量
	int mEnergy;
	//ボールを所持しているか(所持していたらそのボールのアドレス)
	GameObjectBase* mBall;
	
};