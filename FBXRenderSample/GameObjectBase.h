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

class GameObjectBase {
public:
	GameObjectBase();
	virtual ~GameObjectBase();

	virtual void Initialize();
	virtual void Update() = 0;
	virtual void Render() = 0;

	void SetShader(MSBase3DShader*aShader);

	virtual void SetMesh(MSFbxManager&aSetMesh);
	void SetRenderer(MS3DRender*aSetRenderer);
	void GetRenderer(MS3DRender*&aSetRenderer);
	void AddCollisionTarget(GameObjectBase*aCollisionTarget);
	void ClearCollisionTarget();

	void SetActive(bool aActive);
	bool IsActive();

	DXWorld* GetWorld();
	DXCamera* GetView();
	DXProjection* GetProj();
	DX11RenderResource* GetTransform();
	std::vector<GameObjectBase*>*GetCollisionTargets() {
		return &mCollisionTargets;
	};
protected:
	virtual void UpdateMesh();
	virtual GameObjectBase* UpdateCollision();
	//ラムダ用
protected:
	std::unique_ptr<DX11RenderResource>mTransform;
	std::unique_ptr<MSCollisionRayPicking>mRayPick;
	std::vector<GameObjectBase*>mCollisionTargets;
	MS3DRender*mRender;
	//このオブジェクトの描画に使うシェーダ
	MSBase3DShader* mShader;
	//このオブジェクトがアクティブかどうか
	bool mActive;
};
