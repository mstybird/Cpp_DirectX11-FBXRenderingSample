#pragma once
#include<vector>

enum class eTeamType {
	White,
	Black
};

class GameObjectBase;
struct StatusTeam {
	//�S�[���C���f�b�N�X
	int mGoalIndex = -1;
	//�`�[�������o�[���X�g
	std::vector<GameObjectBase*> mMembers;
};