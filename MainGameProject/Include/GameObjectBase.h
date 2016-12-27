#pragma once
#include<vector>
#include<memory>
#include"MSCollisionRayPicking.h"
#include"CBResource0.h"
class DX11RenderResource;
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
	friend class CharacterBase;
public:
	GameObjectBase();
	virtual ~GameObjectBase();

	virtual void Initialize();
	virtual void Update() = 0;
	virtual void Render() = 0;

	void SetShader(MSBase3DShader*aShader);
	void SetCollisionShader(MSBase3DShader*aShader);
	MSBase3DShader* GetCollisionShader();
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

	//自身ともう一つのオブジェクトの距離を計算する
	float GetDistance(GameObjectBase*aTarget);
	virtual bool UpdateMesh();

	std::shared_ptr<DX11RenderResource> LoadTransform( GameObjectBase*aTransform);

	//フレーム毎のリソースを取得
	CBResource0* GetFrameResource();

protected:
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
	//カリング処理に使うシェーダ	
	MSBase3DShader* mCollisionShader;

	//フレーム毎に渡すオブジェクト内共通のリソース
	CBResource0 mFrameResource;

	//このオブジェクトがアクティブかどうか
	bool mActive;
	//コリジョンのサイズ(拡大率)はメッシュのサイズと同じにするかどうか
	//一度でもコリジョン用のサイズを設定した場合、フラグはfalseになる
	bool mIsCollisionScaleDefault;
};
