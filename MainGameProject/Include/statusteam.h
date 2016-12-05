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

	//���������o�[�����ׂ�
	bool IsMember(const GameObjectBase*aMember);
	//�`�[���̃��X�|�[���G���A
	std::vector<Dijkstra::Node*>mSpawnCharaNodes;

	bool IsCollisionBase(GameObjectBase* aCollider);
	//�S�[���C���f�N�X��ݒ肷��
	void SetGoalIndex(const int aIndex);
	//�S�[���C���f�N�X���擾����
	int GetGoalIndex();
	//���_�I�u�W�F�N�g�̎Q�Ƃ��擾����
	StaticObject* GetBase();

	//�X�R�A��������
	void InitScore();
	//�X�R�A���擾
	int GetScore();
	//�X�R�A�����Z
	void AddScore(const int aScore = 1);

	//�����o�[�̔z����擾����
	std::vector<GameObjectBase*>*GetMembers();
	//�`�[���^�C�v���擾����
	eTeamType GetTeamType();

private:
	//���_(�S�[���f�[�^)
	StaticObject mBase;
	//�S�[���͈̔�
	float mBaseScale = 1.0f;
	//�S�[���C���f�b�N�X
	int mGoalIndex = -1;
	//�`�[�������o�[���X�g
	std::vector<GameObjectBase*> mMembers;
	//�`�[���^�C�v
	const eTeamType mTeamType;
	//�`�[���X�R�A
	int mScore = 0;
};