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
class NBullet;
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
	virtual void UpdateAlive() = 0;
	//�d�͏���
	virtual void UpdateGravity() {};
	//�J�����̍X�V
	virtual void UpdateCamera()final;
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

	void Respawn();
	BulletManager* mBltManager;

protected:
	void UpdateBullets();
	void RenderBullets();
	int mActiveBulletID = 0;
	std::vector<std::unique_ptr<NBullet>> mBullets;
	//std::unique_ptr<BulletObject>mBulletNormal;
	std::vector<GameObjectBase*>mSearchTargets;
	StatusField*mField;
	std::shared_ptr<StatusBase>mStatus;
	const DXVector3 cCameraLen;
	const DXVector3 cCameraOffset;
	DXVector3 mCameraLen;
	DXVector3 mCameraOffset;

	std::unique_ptr<StatusBase>mDefaultStatus;

};


