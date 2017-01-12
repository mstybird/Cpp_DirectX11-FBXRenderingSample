#pragma once
#include"GameObjectBase.h"
#include<memory>
#include<vector>
#include<type_traits>
#include<DXMath.hpp>
class MSFbxManager;
class MSBase3DShader;
class MSFbxManager;
class MSFbxObject;

class DX11RenderResource;
class MSCollisionRayPicking;
class DXWorld;
class DXCamera;
class DXProjection;
class MS3DRender;
class BulletObject;
struct StatusField;
class BulletObjectBase;
struct StatusBase;
class BulletManager;
class CharacterBase:public GameObjectBase {
	friend class BulletObject;
public:
	CharacterBase(const DXVector3& cCameraLen, const DXVector3& cCameraOffset);
	CharacterBase();
	virtual ~CharacterBase();
	virtual void Initialize(StatusField&pSetStatus);
	virtual void InitStatus(const StatusBase* aInitStatus);
	//�������S�Ȃǂ̏������s��
	void UpdateAlive();
	//�d�͏���
	virtual void UpdateGravity() {};
	//�J�����̍X�V
	virtual void UpdateCamera()final;
	//���[�V�����̓K�p����
	void UpdateMotion();

	//�t�B�[���h���̎Q�Ƃ�ݒ�
	void SetField(StatusField&pSetStatus);
	//�f�t�H���g�X�e�[�^�X(���X�|�[�����Ɏg�p)
	void SetDefaultStatus(const StatusBase&aStatus);
	const StatusBase* GetDefaultStatus()const;
	//void SetBulletMesh(MSFbxManager&aSetMesh);
	void AddSearchTarget(GameObjectBase*aCollisionTarget);
	void ClearSearchTarget();
	std::vector<GameObjectBase*>*GetSearchTargets() {
		return &mSearchTargets;
	};

	StatusField*GetField();
	StatusBase* GetStatus() {
		return mStatus.get();
	}
	template<typename T>
	T* GetStatus() {
		static_assert(std::is_base_of<StatusBase*,T*>::value == false, "Type Error.");
		return static_cast<T*>(mStatus.get());
	}
	//���X�|�[������
	void Respawn();
	//�e���˂̃E�F�C�g�t���O��ݒ�
	void SetBulletShotWait(bool aIsWait);
	//�e���˂̃E�F�C�ƃt���[������ݒ�
	void SetBulletShotInterval(int aIntervalFrame);

	void LivingIsDeadProc();
	virtual void LivingIsRespawnWaitProc() = 0;
protected:
	void UpdateBullets();
	void RenderBullets();

public:
	BulletManager* mBltManager;

protected:
	//�g�p���̃o���b�g��ID
	int mActiveBulletID = 0;
	//�g�p�\�ȃo���b�g�̈ꗗ
	std::vector<std::unique_ptr<BulletObjectBase>> mBullets;
	//�{���Ώ�
	std::vector<GameObjectBase*>mSearchTargets;
	//�t�B�[���h���
	StatusField*mField;
	//�L�����N�^�X�e�[�^�X
	std::shared_ptr<StatusBase>mStatus;
	const DXVector3 cCameraLen;
	const DXVector3 cCameraOffset;
	DXVector3 mCameraLen;
	DXVector3 mCameraOffset;

	std::unique_ptr<StatusBase>mDefaultStatus;
	//�o���b�g���ˏ����o�^���̏���
	bool mIsBulletShotWaiting;
	int mBulletShotInterval;

	

};


