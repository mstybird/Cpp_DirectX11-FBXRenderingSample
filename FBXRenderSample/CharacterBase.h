#pragma once
#include<memory>
#include<vector>
#include<type_traits>
#include"GameObjectBase.h"
#include"StatusBase.h"
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
class CharacterBase:public GameObjectBase {
	friend class BulletObject;
public:
	CharacterBase();
	virtual ~CharacterBase();
	virtual void Initialize(StatusField&pSetStatus);
	virtual void InitStatus() = 0;
	//¶‘¶€–S‚È‚Ç‚Ìˆ—‚ğs‚¤
	virtual void UpdateAlive() = 0;
	//d—Íˆ—
	virtual void UpdateGravity() {};
	void SetField(StatusField&pSetStatus);
	void SetBulletMesh(MSFbxManager&aSetMesh);
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

protected:
	void UpdateBullets();
	void RenderBullets();

	std::vector<std::unique_ptr<BulletObject>> mBullets;
	std::unique_ptr<BulletObject>mBulletNormal;
	std::vector<GameObjectBase*>mSearchTargets;
	StatusField*mField;
	std::unique_ptr<StatusBase>mStatus;
};


