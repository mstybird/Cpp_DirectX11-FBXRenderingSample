#pragma once
#include<vector>
#include"Dijkstra.h"
#include"StaticObject.h"
enum class eTeamType {
	White,
	Black
};

const int TeamIdWhite = 0;
const int TeamIdBlack = 1;

class GameObjectBase;
class StaticObject;
class StatusTeam {
private:
	StatusTeam();
public:
	StatusTeam(const eTeamType aTeamType);
	~StatusTeam();

	//所属メンバーか調べる
	bool IsMember(const GameObjectBase*aMember);
	//チームのリスポーンエリア
	std::vector<Dijkstra::Node*>mSpawnCharaNodes;

	bool IsCollisionBase(GameObjectBase* aCollider);
	//ゴールインデクスを設定する
	void SetGoalIndex(const int aIndex);
	//ゴールインデクスを取得する
	int GetGoalIndex();
	//拠点オブジェクトの参照を取得する
	StaticObject* GetBase();

	//スコアを初期化
	void InitScore();
	//スコアを取得
	int GetScore();
	//スコアを加算
	void AddScore(const int aScore = 1);

	//メンバーの配列を取得する
	std::vector<GameObjectBase*>*GetMembers();
	//チームタイプを取得する
	eTeamType GetTeamType();

private:
	//拠点(ゴールデータ)
	StaticObject mBase;
	//ゴールの範囲
	float mBaseScale = 1.0f;
	//ゴールインデックス
	int mGoalIndex = -1;
	//チームメンバーリスト
	std::vector<GameObjectBase*> mMembers;
	//チームタイプ
	const eTeamType mTeamType;
	//チームスコア
	int mScore = 0;
};