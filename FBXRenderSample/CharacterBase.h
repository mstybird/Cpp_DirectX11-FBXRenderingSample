#pragma once
#include<memory>
#include<vector>
#include"GameObjectBase.h"
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
	virtual void Initialize()override;
	virtual void InitStatus() = 0;
	void SetField(StatusField&pSetStatus);
	void SetBulletMesh(MSFbxManager&aSetMesh);
	void AddSearchTarget(GameObjectBase*aCollisionTarget);
	void ClearSearchTarget();
	std::vector<GameObjectBase*>*GetSearchTargets() {
		return &mSearchTargets;
	};

	StatusField*GetField();

protected:
	void UpdateBullets();
	void RenderBullets();

	std::vector<std::unique_ptr<BulletObject>> mBullets;
	std::unique_ptr<BulletObject>mBulletNormal;
	std::vector<GameObjectBase*>mSearchTargets;
	StatusField*mField;
};


