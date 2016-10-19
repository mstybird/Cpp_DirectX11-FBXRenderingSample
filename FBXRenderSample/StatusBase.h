#pragma once
#include"MSProgress\MSProgressData.hxx"

enum class CharaStateFlag {
	ALIVE,		//������
	DEAD,		//���S��
	RESPAWNWAIT	//���X�|�[���ҋ@��
};

class GameObjectBase;
struct StatusBase {

	StatusBase();

	//���ݑ����Ă���G
	GameObjectBase* mTarget;
	//�{�[�����������Ă��邩(�������Ă����炻�̃{�[���̃A�h���X)
	GameObjectBase* mBall;

	//HP
	MSProgress mHp;

	//�G�l���M�[�c��
	MSProgress mEnergy;

	//�����֌W�t���O
	CharaStateFlag mLive;

//	int mEnergy;

};