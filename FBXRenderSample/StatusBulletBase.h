#pragma once
/*
	�e�̏��
*/
#include"MSProgress\MSProgressData.hxx"
#include<DXMath.hpp>
//�_���[�W�^�C�v
enum class BulletDamageType {
	NORMAL,	//�ʏ�(�h�䂪���������Ζh��Ɋ֌W����
	PERCENT,//�����_���[�W
	FIXED	//�Œ�_���[�W
};

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