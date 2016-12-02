#pragma once
#include"GameObjectBase.h"
#include"EnemyAI.h"
#include"StatusTeam.h"
#include<vector>
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
	void InitRenderAndShader(MS3DRender& aRender, MSBase3DShader&aShader);
	void CreateFieldNodes();
	void CreateSpawnCharaNodes();
	void CreateSpawnBallNodes();
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
	//
	DXVector3 GetNodePosition(const int aIndex);

	void SetBallHolder(CharacterBase*pBallHolder);
	void RespawnBall(DXVector3*pPosition = nullptr);

	StatusTeam mTeamWhite;	//白チーム
	StatusTeam mTeamBlack;	//黒チーム

};