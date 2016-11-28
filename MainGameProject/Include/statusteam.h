#pragma once
#include<vector>
#include"Dijkstra.h"
enum class eTeamType {
	White,
	Black
};

const int TeamIdWhite = 0;
const int TeamIdBlack = 1;

class GameObjectBase;
struct StatusTeam {
private:
	StatusTeam();
public:
	StatusTeam(const eTeamType aTeamType);
	~StatusTeam();

	//ゴールインデックス
	int mGoalIndex = -1;
	//チームメンバーリスト
	std::vector<GameObjectBase*> mMembers;
	//チームタイプ
	const eTeamType mTeamType;
	//所属メンバーか調べる
	bool IsMember(const GameObjectBase*aMember);
	//チームのリスポーンエリア
	std::vector<Dijkstra::Node*>mSpawnCharaNodes;
};