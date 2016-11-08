#pragma once
#include<vector>

enum class eTeamType {
	White,
	Black
};

class GameObjectBase;
struct StatusTeam {
	//ゴールインデックス
	int mGoalIndex = -1;
	//チームメンバーリスト
	std::vector<GameObjectBase*> mMembers;
	//所属メンバーか調べる
	bool IsMember(const GameObjectBase*aMember);

};