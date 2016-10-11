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
	void Initialize()override;
	//�S�Ă̏��������I�������Ɉ�x�����Ă�
	void InitFinal();

	//�K�����͌ĂԕK�v������
	void SetAI(NcgLuaManager*aAI);

	//���t���[���s���G�l�~�[�̍X�V����
	void Update()override;

	void Render()override;

private:
	//�X�e�[�^�X�̏�����
	void InitStatus()override;
	//AI���X�V����
	void UpdateAI();

	//�{�����̏���
	void UpdateSearching();
	//�^�[�Q�b�g�Ɍ������Ĉړ����鏈��
	void UpdateMoveToTarget();
	//�G�l���M�[�`���[�W
	void UpdateChargeEnergy();
	//�B���
	void UpdateMovingHide();
	//�U��
	void UpdateEnergyShot();

	//�m�[�h�Ԉړ�����
	bool MoveNode();
	//���E����
	GameObjectBase* IsCulling();

private:


	//�s��f�[�^�A���E��������
	
	
	std::unique_ptr<EnemyAI>mAI;


	EnemyStatus mStatus;
};