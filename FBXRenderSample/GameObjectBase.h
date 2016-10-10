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
	void AddCollisionTarget(DX11RenderResource*aCollisionTarget);
	void ClearCollisionTarget();

	void SetActive(bool aActive);
	bool IsActive();

	DXWorld* GetWorld();
	DXCamera* GetView();
	DXProjection* GetProj();
	DX11RenderResource* GetTransform();
protected:
	virtual void UpdateMesh();
	virtual void UpdateCollision();
	//ラムダ用
	std::vector<DX11RenderResource*>*GetCollisionTargets() {
		return &mCollisionTargets;
	};
protected:
	std::unique_ptr<DX11RenderResource>mTransform;
	std::vector<DX11RenderResource*>mCollisionTargets;
	std::unique_ptr<MSCollisionRayPicking>mRayPick;
	MS3DRender*mRender;
	//このオブジェクトの描画に使うシェーダ
	MSBase3DShader* mShader;
	//このオブジェクトがアクティブかどうか
	bool mActive;
};
