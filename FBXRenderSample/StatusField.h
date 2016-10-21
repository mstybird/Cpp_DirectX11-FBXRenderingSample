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

//�t�B�[���h���
struct StatusField {
	//�N���{�[�����������Ă��邩
	GameObjectBase* mBallHoldChara;
	//�t�B�[���h�Ƀ{�[�������邩�ǂ���
	bool mBallIsField;
	//�{�[�����
	GameObjectBase* mBall;

	//�t�B�[���h��AI�p�m�[�h���X�g
	std::vector<Dijkstra::Node*>mFieldNodes;
	//�t�B�[���h�̃X�|�[���p�m�[�h���X�g
	std::vector<Dijkstra::Node*>mSpawnCharaNodes;
	//�{�[���I�u�W�F�N�g�X�|�[���p�m�[�h���X�g
	std::vector<Dijkstra::Node*>mSpawnBallNodes;

	void CreateFieldNodes();
	void CreateSpawnCharaNodes();
	void CreateSpawnBallNodes();
	std::vector<Dijkstra::Node*>GetFieldNodesClone();
	//�L�����N�^�[�����X�|�[��������
	void Respawn(CharacterBase* aSpawnChara);


	StatusField();
	void SetBallHolder(GameObjectBase*pBallHolder);
	void RespawnBall(DXVector3*pPosition = nullptr);

};