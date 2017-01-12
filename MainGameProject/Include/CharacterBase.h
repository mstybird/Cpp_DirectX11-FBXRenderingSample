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
	//生存死亡などの処理を行う
	void UpdateAlive();
	//重力処理
	virtual void UpdateGravity() {};
	//カメラの更新
	virtual void UpdateCamera()final;
	//モーションの適用処理
	void UpdateMotion();

	//フィールド情報の参照を設定
	void SetField(StatusField&pSetStatus);
	//デフォルトステータス(リスポーン時に使用)
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
	//リスポーン処理
	void Respawn();
	//弾発射のウェイトフラグを設定
	void SetBulletShotWait(bool aIsWait);
	//弾発射のウェイとフレーム数を設定
	void SetBulletShotInterval(int aIntervalFrame);

	void LivingIsDeadProc();
	virtual void LivingIsRespawnWaitProc() = 0;
protected:
	void UpdateBullets();
	void RenderBullets();

public:
	BulletManager* mBltManager;

protected:
	//使用中のバレットのID
	int mActiveBulletID = 0;
	//使用可能なバレットの一覧
	std::vector<std::unique_ptr<BulletObjectBase>> mBullets;
	//捜索対象
	std::vector<GameObjectBase*>mSearchTargets;
	//フィールド情報
	StatusField*mField;
	//キャラクタステータス
	std::shared_ptr<StatusBase>mStatus;
	const DXVector3 cCameraLen;
	const DXVector3 cCameraOffset;
	DXVector3 mCameraLen;
	DXVector3 mCameraOffset;

	std::unique_ptr<StatusBase>mDefaultStatus;
	//バレット発射処理登録時の処理
	bool mIsBulletShotWaiting;
	int mBulletShotInterval;

	

};


