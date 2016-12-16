#pragma once
#include"GameObjectBase.h"
#include"EnemyAI.h"
#include"StatusTeam.h"
#include"Effect.hxx"
#include<vector>
#include<chrono>
class CharacterBase;
namespace NodeControl {
	void AddNodeSafe(std::vector<Dijkstra::Node*>&aNodeList, Dijkstra::Node*aAddNode);
	void LinkNodeSafe(
		std::vector<Dijkstra::Node*>&aNodeList,
		std::pair<int, int>aFromTo,
		float aCost,
		bool aBothFlag
	);
	template<typename T>
	bool IsBetweenIndex(std::vector<T>aVector,unsigned int aIndex) {
		auto lMin{ 0 };
		auto lMax{ aVector.size() };
		return (lMin <= aIndex) && (aIndex <= lMax);
	}
}

enum class IssueFlag {
	Win,
	Lose,
	Draw
};

//フィールド情報
struct StatusField {

	StatusField();
	virtual ~StatusField();

	//誰がボールを所持しているか
	CharacterBase* mBallHoldChara;
	//フィールドにボールがあるかどうか
	bool mBallIsField;
	//ボール情報
	GameObjectBase* mBall;

	//フィールドのAI用ノードリスト
	std::vector<Dijkstra::Node*>mFieldNodes;
	//フィールドのスポーン用ノードリスト
	//std::vector<Dijkstra::Node*>mSpawnCharaNodes;
	//ボールオブジェクトスポーン用ノードリスト
	std::vector<Dijkstra::Node*>mSpawnBallNodes;

	void Initialize();
	//制限時間を設定する
	void InitializeTime(const int mLimitSecond);
	void InitEffect(::Comfort::EfkManager*aManager, ::Comfort::EffectDatabase*aDb, const int aBallGetID, const int aKillID);
	void InitRenderAndShader(MS3DRender& aRender, MSBase3DShader&aShader);
	void CreateFieldNodes(const std::string& aAIMap);
	void CreateSpawnCharaNodes(const std::string& aSpawnMapBlack, const std::string& aSpawnMapWhite);
	void CreateSpawnBallNodes(const std::string& aBallMap);
	void InitGoalIndex(const int aWhiteGoalIndex, const int aBlackGoalIndex);
	std::vector<Dijkstra::Node*>GetFieldNodesClone();
	//キャラクターをリスポーンさせる
	void Respawn(CharacterBase* aSpawnChara);

	//チームにメンバーを登録する
	void RegisterTeamMember(CharacterBase*aRegistMember,eTeamType aType);
	//チームのゴールを取得する
	StaticObject* GetTeamBase(eTeamType aTeamType);
	//所属しているチームを取得
	StatusTeam* GetTeamAlly(CharacterBase*aMember);
	//相手のチームを取得
	StatusTeam* GetTeamEnemy(CharacterBase*aMember);
	//自チームのタイプを取得
	eTeamType GetTeamType(CharacterBase*aChara);
	//指定キャラがボールをゲットする
	void GetBall(CharacterBase*aChara);
	//指定したキャラが生老チームに含まれているかどうか
	IssueFlag IsWin(CharacterBase*aChara);

	//指定したノードの座標を取得する
	DXVector3 GetNodePosition(const int aIndex);
	//ボール所持者を設定する
	void SetBallHolder(CharacterBase*pBallHolder);
	void RespawnBall(DXVector3*pPosition = nullptr);

	//ゴールした際の処理
	void GoalProccess(CharacterBase* aGoaler);

	//Team Whiteのスコアを取得する
	int GetScoreWhite();
	//Team Blackのスコアを取得する
	int GetScoreBlack();
	//時間を更新する
	void UpdateTime();
	//ゲームを開始する
	void GameStart();
	//制限時間チェックをする
	bool IsTimeOver();
	//残り時間を分と秒で取得する
	void GetRemainTime(int&aMinutes, int&aSeconds);

	//毎フレーム行う処理
	void Update();

	StatusTeam mTeamWhite;	//白チーム
	StatusTeam mTeamBlack;	//黒チーム
private:
	//開始時間
	std::chrono::system_clock::time_point mStartTime;
	//制限時間
	int mLimitTime;
	//残り時間
	int mRemainTime;
	//ボールゲットエフェクト
	::Comfort::EfkObject mEfkBallGet;
	//ゲーム終了エフェクト
	::Comfort::EfkObject mEfkGameSet;

};