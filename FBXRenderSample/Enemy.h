#pragma once
#include"CharacterBase.h"
#include"StatusEnemy.h"
class EnemyAI;
class NcgLuaManager;

/*
	�^�X�N�F
	�G�̕K�v�ȓ����S�Ď���

*/

//�e�G��AI������
class Enemy :public CharacterBase {
public:
	Enemy();
	~Enemy();

	//�f�[�^�̏�����(���̌Ăяo�����K�v)
	void Initialize(StatusField&pSetStatus)override;
	//�S�Ă̏��������I�������Ɉ�x�����Ă�
	void InitFinal();

	//�K�����͌ĂԕK�v������
	void SetAI(NcgLuaManager*aAI);
	//�S�[���n�_��ݒ肷��
	void SetGoalIndex(int aIndex);

	//�X�e�[�^�X�̎Q�Ƃ��擾����
	//EnemyStatus* GetStatus();

	//���t���[���s���G�l�~�[�̍X�V����
	void Update()override;
	void UpdateAlive()override;

	void Render()override;

private:
	//�X�e�[�^�X�̏�����
	void InitStatus()override;
	//AI���X�V����
	void UpdateAI();

	//�T��(���v��)
	void UpdateSearching();
	//�^�[�Q�b�g�̈ʒu�Ɉړ�����
	void UpdateMoveToTarget();
	//�G�l���M�[�`���[�W
	void UpdateChargeEnergy();
	//�B���
	void UpdateMovingHide();
	//�U��
	void UpdateEnergyShot();
	//�S�[���ւƈړ�����
	void UpdateMoveToGoal();
	//�{�[������ɓ���ɂ���
	void UpdateMoveToBall();
	//�{�[���������Ă���G���U�����ɂ���
	void UpdateMoveToBallTarget();

	//�m�[�h�Ԉړ�����
	bool MoveNode();
	//���E����
	GameObjectBase* IsCulling();

private:


	//�s��f�[�^�A���E��������
	
	
	std::unique_ptr<EnemyAI>mAI;


	//EnemyStatus mStatus;
};