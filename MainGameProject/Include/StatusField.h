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

//�t�B�[���h���
struct StatusField {

	StatusField();
	virtual ~StatusField();

	//�N���{�[�����������Ă��邩
	CharacterBase* mBallHoldChara;
	//�t�B�[���h�Ƀ{�[�������邩�ǂ���
	bool mBallIsField;
	//�{�[�����
	GameObjectBase* mBall;

	//�t�B�[���h��AI�p�m�[�h���X�g
	std::vector<Dijkstra::Node*>mFieldNodes;
	//�t�B�[���h�̃X�|�[���p�m�[�h���X�g
	//std::vector<Dijkstra::Node*>mSpawnCharaNodes;
	//�{�[���I�u�W�F�N�g�X�|�[���p�m�[�h���X�g
	std::vector<Dijkstra::Node*>mSpawnBallNodes;

	void CreateFieldNodes();
	void CreateSpawnCharaNodes();
	void CreateSpawnBallNodes();
	void InitGoalIndex();
	std::vector<Dijkstra::Node*>GetFieldNodesClone();
	//�L�����N�^�[�����X�|�[��������
	void Respawn(CharacterBase* aSpawnChara);

	//�`�[���Ƀ����o�[��o�^����
	void RegisterTeamMember(CharacterBase*aRegistMember,eTeamType aType);

	//�������Ă���`�[�����擾
	StatusTeam* GetTeamAlly(CharacterBase*aMember);
	//����̃`�[�����擾
	StatusTeam* GetTeamEnemy(CharacterBase*aMember);
	//���`�[���̃^�C�v���擾
	eTeamType GetTeamType(CharacterBase*aChara);

	void SetBallHolder(CharacterBase*pBallHolder);
	void RespawnBall(DXVector3*pPosition = nullptr);

	StatusTeam mTeamWhite;	//���`�[��
	StatusTeam mTeamBlack;	//���`�[��

};