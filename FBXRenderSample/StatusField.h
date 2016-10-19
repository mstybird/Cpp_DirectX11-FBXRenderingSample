#pragma once
#include<vector>
#include"GameObjectBase.h"
#include"AI/EnemyAI.h"
class CharacterBase;
namespace NodeControl {
	void AddNodeSafe(std::vector<Dijkstra::Node*>&aNodeList, Dijkstra::Node*aAddNode);
	void LinkNodeSafe(
		std::vector<Dijkstra::Node*>&aNodeList,
		std::pair<int, int>aFromTo,
		int aCost,
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
	//誰がボールを所持しているか
	GameObjectBase* mBallHoldChara;
	//フィールドにボールがあるかどうか
	bool mBallIsField;
	//ボール情報
	GameObjectBase* mBall;

	//フィールドのAI用ノードリスト
	std::vector<Dijkstra::Node*>mFieldNodes;
	//フィールドのスポーン用ノードリスト
	std::vector<Dijkstra::Node*>mSpawnCharaNodes;
	//ボールオブジェクトスポーン用ノードリスト
	std::vector<Dijkstra::Node*>mSpawnBallNodes;

	void CreateFieldNodes();
	void CreateSpawnCharaNodes();
	void CreateSpawnBallNodes();
	std::vector<Dijkstra::Node*>GetFieldNodesClone();
	//キャラクターをリスポーンさせる
	void Respawn(CharacterBase* aSpawnChara);


	StatusField();
	void SetBallHolder(GameObjectBase*pBallHolder);
	void RespawnBall();

};