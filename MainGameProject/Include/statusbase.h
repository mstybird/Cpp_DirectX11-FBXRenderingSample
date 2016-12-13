#pragma once
#include<cstdint>
#include"MSProgressData.hxx"

enum class CharaStateFlag {
	ALIVE,		//������
	DEAD,		//���S��
	RESPAWNWAIT	//���X�|�[���ҋ@��
};

namespace ValueStatusBase {
	static const char* cHp = "Hp";
	static const char* cEnergy = "Energy";
}

class GameObjectBase;
struct StatusBase {

	StatusBase();
	virtual ~StatusBase() {};
	//���ݑ����Ă���G
	GameObjectBase* mTargetChara;//--[Lua]mLockonEnemy/mInsightEnemy/mBallHoldTarget
	//�{�[�����������Ă��邩(�������Ă����炻�̃{�[���̃A�h���X)
	GameObjectBase* mBall;	//--[Lua]mBallHoldMe

	//�������߂��ɂ��邩�ǂ���
	bool mAllyNear;
	//�{�[���������Ă��閡�����߂��ɂ��邩
	bool mBallAllyNear;
	//HP
	MSProgress mHp;

	//�G�l���M�[�c��
	MSProgress mEnergy;		//--[Lua]mChargedEnergy

	//�����֌W�t���O
	CharaStateFlag mLive;

	//����ł���̃t���[����
	int mDeadFrame;


//	int mEnergy;

};