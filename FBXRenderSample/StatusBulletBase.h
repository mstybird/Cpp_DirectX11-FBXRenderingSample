#pragma once
/*
	�e�̏��
*/
#include"MSProgress\MSProgressData.hxx"

//�_���[�W�^�C�v
enum class BulletDamageType {
	NORMAL,	//�ʏ�(�h�䂪���������Ζh��Ɋ֌W����
	PERCENT,//�����_���[�W
	FIXED	//�Œ�_���[�W
};

struct StatusBulletBase {
	int mCost;
	float mAtk;
	MSProgress mInterval;
	BulletDamageType mType;
};