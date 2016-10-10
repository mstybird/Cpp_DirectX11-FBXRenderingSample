#pragma once
#include<memory>
#include<vector>
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

#include"GameObjectBase.h"
class BulletObject;

class CharacterBase:public GameObjectBase {
public:
	CharacterBase();
	virtual ~CharacterBase();
	virtual void Initialize()override;
	virtual void InitStatus() = 0;
	void SetBulletMesh(MSFbxManager&aSetMesh);
protected:
	void UpdateBullets();
	void RenderBullets();

	std::vector<std::unique_ptr<BulletObject>> mBullets;
	std::unique_ptr<BulletObject>mBulletNormal;
};


