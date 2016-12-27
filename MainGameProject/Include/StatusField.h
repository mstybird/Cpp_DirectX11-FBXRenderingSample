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
		auto lMin{ 0U };
		auto lMax{ aVector.size() };
		return (lMin <= aIndex) && (aIndex <= lMax);
	}
}

enum class IssueFlag {
	Win,
	Lose,
	Draw
};

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

	void Initialize();
	//�������Ԃ�ݒ肷��
	void InitializeTime(const int mLimitSecond);
	void InitEffect(::Comfort::EfkManager*aManager, ::Comfort::EffectDatabase*aDb, const int aBallGetID, const int aKillID);
	void InitRenderAndShader(MS3DRender& aRender, MSBase3DShader&aShader);
	void CreateFieldNodes(const std::string& aAIMap);
	void CreateSpawnCharaNodes(const std::string& aSpawnMapBlack, const std::string& aSpawnMapWhite);
	void CreateSpawnBallNodes(const std::string& aBallMap);
	void InitGoalIndex(const int aWhiteGoalIndex, const int aBlackGoalIndex);
	std::vector<Dijkstra::Node*>GetFieldNodesClone();
	//�L�����N�^�[�����X�|�[��������
	void Respawn(CharacterBase* aSpawnChara);

	//�`�[���Ƀ����o�[��o�^����
	void RegisterTeamMember(CharacterBase*aRegistMember,eTeamType aType);
	//�`�[���̃S�[�����擾����
	StaticObject* GetTeamBase(eTeamType aTeamType);
	//�������Ă���`�[�����擾
	StatusTeam* GetTeamAlly(CharacterBase*aMember);
	//����̃`�[�����擾
	StatusTeam* GetTeamEnemy(CharacterBase*aMember);
	//���`�[���̃^�C�v���擾
	eTeamType GetTeamType(CharacterBase*aChara);
	//�w��L�������{�[�����Q�b�g����
	void GetBall(CharacterBase*aChara);
	//�w�肵���L���������V�`�[���Ɋ܂܂�Ă��邩�ǂ���
	IssueFlag IsWin(CharacterBase*aChara);

	//�w�肵���m�[�h�̍��W���擾����
	DXVector3 GetNodePosition(const int aIndex);
	//�{�[�������҂�ݒ肷��
	void SetBallHolder(CharacterBase*pBallHolder);
	void RespawnBall(DXVector3*pPosition = nullptr);

	//�S�[�������ۂ̏���
	void GoalProccess(CharacterBase* aGoaler);

	//Team White�̃X�R�A���擾����
	int GetScoreWhite();
	//Team Black�̃X�R�A���擾����
	int GetScoreBlack();
	//���Ԃ��X�V����
	void UpdateTime();
	//�Q�[�����J�n����
	void GameStart();
	//�������ԃ`�F�b�N������
	bool IsTimeOver();
	//�c�莞�Ԃ𕪂ƕb�Ŏ擾����
	void GetRemainTime(int&aMinutes, int&aSeconds);

	//���t���[���s������
	void Update();

	StatusTeam mTeamWhite;	//���`�[��
	StatusTeam mTeamBlack;	//���`�[��
private:
	//�J�n����
	std::chrono::system_clock::time_point mStartTime;
	//��������
	int mLimitTime;
	//�c�莞��
	int mRemainTime;
	//�{�[���Q�b�g�G�t�F�N�g
	::Comfort::EfkObject mEfkBallGet;
	//�Q�[���I���G�t�F�N�g
	::Comfort::EfkObject mEfkGameSet;

};