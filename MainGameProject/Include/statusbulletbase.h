#pragma once
/*
	�e�̏��
*/
#include"MSProgressData.hxx"
#include<DXMath.hpp>
//�_���[�W�^�C�v
enum class BulletDamageType {
	NORMAL,	//�ʏ�(�h�䂪���������Ζh��Ɋ֌W����
	PERCENT,//�����_���[�W
	FIXED	//�Œ�_���[�W
};

BulletDamageType ConvertDamageType(int aType);


namespace ValueStatusBullet {
	static const char* cCost = "Cost";
	static const char* cAtk = "Atk";
	static const char* cInterval = "Interval";
	static const char* cIntervalRecovery = "IntervalRecovery";
	static const char* cDamageType = "DamageType";
	static const char* cFiringRange = "FiringRange";
	static const char* cVelocity = "Velocity";
	static const char* cDirection = "Direction";
}


//�o���b�g�ŗL�̔\�͂̊��N���X
struct StatusBulletBase {
	int mCost;				//�g�p�R�X�g
	float mAtk;				//�U����
	MSProgress mInterval;	//���ˊԊu
	float mIntervalRecovery;//���ˊԊu�񕜗�
	BulletDamageType mType;	//�_���[�W�^�C�v
	float mFiringRange;		//�˒�
	float mVelocity;		//���x
	DXVector3 mDirection;	//�����x�N�g��


};