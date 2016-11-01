#pragma once
#include"CharacterBase.h"
#include"StatusEnemy.h"
class EnemyAI;
class NcgLuaManager;

/*
	タスク：
	敵の必要な動き全て実装

*/

//各敵がAIを持つ
class Enemy :public CharacterBase {
public:
	Enemy();
	virtual ~Enemy();

	//データの初期化(一回の呼び出しが必要)
	void Initialize(StatusField&pSetStatus)override;
	//全ての初期化が終わった後に一度だけ呼ぶ
	void InitFinal();

	//必ず一回は呼ぶ必要がある
	void SetAI(NcgLuaManager*aAI);
	//ゴール地点を設定する
	void SetGoalIndex(int aIndex);

	//ステータスの参照を取得する
	//EnemyStatus* GetStatus();

	//毎フレーム行うエネミーの更新処理
	void Update()override;
	void UpdateAlive()override;

	void Render()override;

private:
	//ステータスの初期化
	void InitStatus()override;
	//AIを更新する
	void UpdateAI();

	//探索(無計画)
	void UpdateSearching();
	//ターゲットの位置に移動する
	void UpdateMoveToTarget();
	//エネルギーチャージ
	void UpdateChargeEnergy();
	//隠れる
	void UpdateMovingHide();
	//攻撃
	void UpdateEnergyShot();
	//ゴールへと移動する
	void UpdateMoveToGoal();
	//ボールを手に入れにいく
	void UpdateMoveToBall();
	//ボールを持っている敵を攻撃しにいく
	void UpdateMoveToBallTarget();

	/*
		最新AI
	*/
	//ボールを取りに行く
	void BetaMoveToBall();
	//ボールをゴールに持っていく
	void BetaMoveToGoal();
	//隠れる
	void BetaMoveToHide();
	//エネルギーチャージ
	void BetaChargeEnergy();
	//攻撃
	void BetaInSightAttack();
	//ボールを持っている味方の付近に移動する
	void BetaMoveToBallHoldAlly();
	//味方からあまり離れずに敵を探す
	void BetaSearchEnemyShort();
	//無心で敵を探す。無差別マン
	void BetaSerachEnemyAll();
	//捕捉済みの敵を追いかける
	void BetaMoveToLookingTarget();
	//ボールを所持している敵を追いかける
	void BetaMoveToBallTarget();
	//敵のエリアを探索する
	void BetaSearchForEnemyArea();
	//味方エリアを探索する
	void BetaSearchForAllyArea();






	//ノード間移動処理
	bool MoveNode();
	//視界処理
	GameObjectBase* IsCulling();

private:


	//行列データ、境界球が入る
	
	
	std::unique_ptr<EnemyAI>mAI;


	//EnemyStatus mStatus;
};