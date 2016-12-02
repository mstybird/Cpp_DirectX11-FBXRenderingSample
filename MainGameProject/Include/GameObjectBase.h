#pragma once
#include<vector>
#include<memory>
class DX11RenderResource;
#include"MSCollisionRayPicking.h"
class DXWorld;
class DXCamera;
class DXProjection;
class MS3DRender;
class MSBase3DShader;
class MSFbxManager;
class MSFbxObject;
class MSProgress;
class MSGravity;
class GameObjectBase {
	friend class Enemy;
public:
	GameObjectBase();
	virtual ~GameObjectBase();

	virtual void Initialize();
	virtual void Update() = 0;
	virtual void Render() = 0;

	void SetShader(MSBase3DShader*aShader);

	virtual void SetMesh(MSFbxManager&aSetMesh);
	void SetMeshScale(float aX, float aY, float aZ);
	void SetCollisionMesh(MSFbxManager&aSetMesh);
	void SetCollisionScale(float aX, float aY, float aZ);
	//DX11RenderResource* GetCollision();
	void SetRenderer(MS3DRender*aSetRenderer);
	void GetRenderer(MS3DRender*&aSetRenderer);
	void AddCollisionTarget(GameObjectBase*aCollisionTarget);
	void ClearCollisionTarget();

	void SetActive(bool aActive);
	bool IsActive();

	MSFbxManager* GetMesh();
	DXWorld* GetWorld();
	DXCamera* GetView();
	DXProjection* GetProj();
	DX11RenderResource* GetTransform();
	std::vector<GameObjectBase*>*GetCollisionTargets() {
		return &mCollisionTargets;
	};

	//���g�Ƃ�����̃I�u�W�F�N�g�̋������v�Z����
	float GetDistance(GameObjectBase*aTarget);

protected:
	virtual void UpdateMesh();
	virtual std::vector<GameObjectBase*> UpdateCollision(bool pIsUpdatePosition);
	//�����_�p
protected:
	std::shared_ptr<DX11RenderResource>mTransform;
	std::shared_ptr<DX11RenderResource>mCollisionMesh;
	std::shared_ptr<MSCollisionRayPicking>mRayPick;
	std::vector<GameObjectBase*>mCollisionTargets;
	MS3DRender*mRender;
	//���̃I�u�W�F�N�g�̕`��Ɏg���V�F�[�_
	MSBase3DShader* mShader;
	//���̃I�u�W�F�N�g���A�N�e�B�u���ǂ���
	bool mActive;
	//�R���W�����̃T�C�Y(�g�嗦)�̓��b�V���̃T�C�Y�Ɠ����ɂ��邩�ǂ���
	//��x�ł��R���W�����p�̃T�C�Y��ݒ肵���ꍇ�A�t���O��false�ɂȂ�
	bool mIsCollisionScaleDefault;
};
