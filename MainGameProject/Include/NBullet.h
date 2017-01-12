#pragma once
#include"GameObjectBase.h"
#include"StatusBulletBase.h"
#include"CharacterBase.h"
#include"Effect.hxx"
#include<DXMath.hpp>
#include<memory>
class CharacterBase;
struct StatusBulletBase;
class MSBase3DShader;




//�e���I�u�W�F�N�g
class BulletObjectBase :public GameObjectBase {
public:
	virtual ~BulletObjectBase();
	//��ɒe���Ƃ̃X�e�[�^�X�̏�����
	virtual void InitStatus(StatusBulletBase* aBulletStatus);

	//�G�t�F�N�g�̓o�^
	void SetEffect(::Comfort::EfkManager*aManager, ::Comfort::EffectDatabase*aDb, const int aHitID, const int aShotID,const int aKillID);

	virtual void Create(std::vector<std::unique_ptr<BulletObjectBase>>&aOutBulletList, CharacterBase* aShoter) = 0;

	//���˓���J�n���ɌĂ΂��
	virtual void ShotFirstEffect(CharacterBase* aShoter) = 0;

	void KillChara(CharacterBase* aShoter);

	StatusBulletBase* GetStatus() {
		return &mStatus;
	}
	template<typename T>
	T* GetStatus() {
		static_assert(std::is_base_of<StatusBulletBase*, T*>::value == false, "Type Error.");
		return static_cast<T*>(mStatus.get());
	}


	//�Փ˂��Ȃ��I�u�W�F�N�g��x���ǉ�����
	void AddNoCollisionObject(GameObjectBase* aObject);

	//�Փ˂��Ȃ��I�u�W�F�N�g���N���A����
	void ClearNocollisionList();

	virtual void UpdateStatus() {}

	//�e�̍X�V
	virtual void Update()final;

	//�ړ�����
	virtual void UpdateMove() = 0;
	//�q�b�g�������|���Ȃ������Ƃ�
	virtual void UpdateHitNoKill() {};
	//�q�b�g���Ă���ɓ|������
	virtual void UpdateHitAndKill() {};


	virtual void Render()final;

	CharacterBase* GetParentChara();
	void SetParentChara(CharacterBase* aChara);

	//�G�t�F�N�g�̐ݒ�A�擾�֌W

	::Comfort::EfkObject* GetEffectFirst();
	::Comfort::EfkObject* GetEffectShot();
	::Comfort::EfkObject* GetEffectHit();
	::Comfort::EfkObject* GetEffectKill();
	void SetEffectFirst(::Comfort::EfkObject& aEffect);
	void SetEffectShot(::Comfort::EfkObject& aEffect);
	void SetEffectHit(::Comfort::EfkObject& aEffect);
	void SetEffectKill(::Comfort::EfkObject& aEffect);



protected:
	//�e�̃X�e�[�^�X
	StatusBulletBase mStatus;
	//���ˎ�
	CharacterBase* mParentPtr;

	//�Փ˂��Ȃ��I�u�W�F�N�g���X�g
	std::vector<GameObjectBase*>mNoCollisions;

	::Comfort::EfkObject mFirstEffect;		//���ˊJ�n���̃G�t�F�N�g
	::Comfort::EfkObject mShotEffect;		//���˒���̃G�t�F�N�g
	::Comfort::EfkObject mHitEffect;		//�q�b�g���̃G�t�F�N�g
	::Comfort::EfkObject mKillEffect;		//�|�������̃G�t�F�N�g



};

