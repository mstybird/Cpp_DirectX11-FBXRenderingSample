#pragma once
#include"CharacterBase.h"
#include"DXVector3.h"
class EnemyAI;
class NcgLuaManager;
struct EnemyStatus {
	//�G�𑨂��Ă�����true
	bool mTargetting;
	//���ݑ����Ă���G
	DX11RenderResource* mTarget;
	//�Ō�Ƀ^�[�Q�b�g�������ʒu
	DXVector3 mLastLookPosiion;
	//�G�l���M�[�c��
	int mEnergy;

	EnemyStatus() :
		mTargetting{ false },
		mTarget{ nullptr },
		mEnergy{ 0 } {};

};

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
	void AddSearchTarget(DX11RenderResource*aCollisionTarget);
	void ClearSearchTarget();
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
	DX11RenderResource* IsCulling();

private:


	//�s��f�[�^�A���E��������
	
	
	std::vector<DX11RenderResource*>mSearchTargets;
	std::unique_ptr<EnemyAI>mAI;


	EnemyStatus mStatus;
};