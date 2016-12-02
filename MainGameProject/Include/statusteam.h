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
struct StatusTeam {
private:
	StatusTeam();
public:
	StatusTeam(const eTeamType aTeamType);
	~StatusTeam();

	//�S�[���C���f�b�N�X
	int mGoalIndex = -1;
	//�`�[�������o�[���X�g
	std::vector<GameObjectBase*> mMembers;
	//�`�[���^�C�v
	const eTeamType mTeamType;
	//���������o�[�����ׂ�
	bool IsMember(const GameObjectBase*aMember);
	//�`�[���̃��X�|�[���G���A
	std::vector<Dijkstra::Node*>mSpawnCharaNodes;
	//���_(�S�[���f�[�^)
	StaticObject mBase;
	//�S�[���͈̔�
	float mBaseScale = 1.0f;
	bool IsCollisionBase(GameObjectBase* aCollider);


};