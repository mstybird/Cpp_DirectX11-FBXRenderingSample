#pragma once
#include"CharacterBase.h"
#include"DXVector3.h"
class EnemyAI;
class NcgLuaManager;
struct EnemyStatus {
	//敵を捉えていたらtrue
	bool mTargetting;
	//現在捉えている敵
	DX11RenderResource* mTarget;
	//最後にターゲットを見た位置
	DXVector3 mLastLookPosiion;
	//エネルギー残量
	int mEnergy;

	EnemyStatus() :
		mTargetting{ false },
		mTarget{ nullptr },
		mEnergy{ 0 } {};

};

/*
	タスク：
	敵の必要な動き全て実装

*/

//各敵がAIを持つ
class Enemy :public CharacterBase {
public:
	Enemy();
	~Enemy();

	//データの初期化(一回の呼び出しが必要)
	void Initialize()override;
	//全ての初期化が終わった後に一度だけ呼ぶ
	void InitFinal();

	//必ず一回は呼ぶ必要がある
	void SetAI(NcgLuaManager*aAI);
	void AddSearchTarget(DX11RenderResource*aCollisionTarget);
	void ClearSearchTarget();
	//毎フレーム行うエネミーの更新処理
	void Update()override;

	void Render()override;

private:
	//ステータスの初期化
	void InitStatus()override;
	//AIを更新する
	void UpdateAI();

	//捜索時の処理
	void UpdateSearching();
	//ターゲットに向かって移動する処理
	void UpdateMoveToTarget();
	//エネルギーチャージ
	void UpdateChargeEnergy();
	//隠れる
	void UpdateMovingHide();
	//攻撃
	void UpdateEnergyShot();

	//ノード間移動処理
	bool MoveNode();
	//視界処理
	DX11RenderResource* IsCulling();

private:


	//行列データ、境界球が入る
	
	
	std::vector<DX11RenderResource*>mSearchTargets;
	std::unique_ptr<EnemyAI>mAI;


	EnemyStatus mStatus;
};