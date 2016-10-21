#pragma once
#include<memory>
//重力管理クラス
class MSProgress;
class MSGravity {
public:
	MSGravity();
	~MSGravity();

	void Initialize(
		bool aIsGravity = true,
		float aMaxGravity = 0,
		float aMinGravity = 0,
		float aNowGravity = 0,
		float aRepulsion = 0,
		float aAttraction = 0);

	//上方向に移動(ジャンプ)する
	void UpdateUpGravity();
	//下方向に移動(叩きつけ)する
	void UpdateDownGravity();
	//重力情報を更新
	void UpdateGravity();

	//現在の重力を取得
	float GetGravity();

private:
	bool mIsGravity;	//重力が有効かどうか
	std::unique_ptr<MSProgress>mGravity;
	float mFrameAttraction;	//重力
	float mRepulsion;	//斥力
	float mAttraction;	//引力

};