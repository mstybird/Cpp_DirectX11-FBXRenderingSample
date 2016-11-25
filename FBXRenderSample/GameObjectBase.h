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

	//自身ともう一つのオブジェクトの距離を計算する
	float GetDistance(GameObjectBase*aTarget);

protected:
	virtual void UpdateMesh();
	virtual std::vector<GameObjectBase*> UpdateCollision(bool pIsUpdatePosition);
	//ラムダ用
protected:
	std::shared_ptr<DX11RenderResource>mTransform;
	std::shared_ptr<DX11RenderResource>mCollisionMesh;
	std::shared_ptr<MSCollisionRayPicking>mRayPick;
	std::vector<GameObjectBase*>mCollisionTargets;
	MS3DRender*mRender;
	//このオブジェクトの描画に使うシェーダ
	MSBase3DShader* mShader;
	//このオブジェクトがアクティブかどうか
	bool mActive;
	std::shared_ptr<MSGravity>mGravity;
	//コリジョンのサイズ(拡大率)はメッシュのサイズと同じにするかどうか
	//一度でもコリジョン用のサイズを設定した場合、フラグはfalseになる
	bool mIsCollisionScaleDefault;
};
