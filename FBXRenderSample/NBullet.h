#pragma once
#include"GameObjectBase.h"
#include"StatusBulletBase.h"
#include"CharacterBase.h"
#include<DXMath.hpp>
#include<memory>
class CharacterBase;
struct StatusBulletBase;
class MSBase3DShader;


struct NBulletStatus {
	int mCost;
	float mAtk;
	MSProgress mInterval;
	BulletDamageType mType;
};

//�e���I�u�W�F�N�g
class NBullet :public GameObjectBase {
public:
	virtual ~NBullet();
	//��ɒe�̃V�F�[�_�̏�����
	virtual void InitShader();
	//��ɒe���Ƃ̃X�e�[�^�X�̏�����
	virtual void InitStatus() {}
	void Create(std::vector<std::unique_ptr<NBullet>>&aOutBulletList, CharacterBase* aShoter);

	StatusBulletBase* GetStatus() {
		return &mStatus;
	}
	template<typename T>
	T* GetStatus() {
		static_assert(std::is_base_of<StatusBulletBase*, T*>::value == false, "Type Error.");
		return static_cast<T*>(mStatus.get());
	}

	virtual void UpdateStatus() {}

	//�e�̍X�V
	virtual void Update();
	virtual void Render();


	//�˒�
	float mFiringRange;
	//���x
	float mVelocity;
	//�����x�N�g��
	DXVector3 mDirection;


	//�e�̎�ނ��ƂɎ���(�Q�Ƃ̂݃R�s�[����p)
	std::shared_ptr<MSBase3DShader>mBulletShader;
	StatusBulletBase mStatus;
	//���ˎ�
	CharacterBase* mParentPtr;

};