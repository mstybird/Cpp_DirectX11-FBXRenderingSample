#pragma once
#include"GameObjectBase.h"
#include"StatusBulletBase.h"
#include"CharacterBase.h"
#include<DXMath.hpp>
#include<memory>
class CharacterBase;
struct StatusBulletBase;
class MSBase3DShader;


struct NBulletStatus {
	int mCost;
	float mAtk;
	MSProgress mInterval;
	BulletDamageType mType;
};

//弾基底オブジェクト
class NBullet :public GameObjectBase {
public:
	virtual ~NBullet();
	//主に弾のシェーダの初期化
	virtual void InitShader();
	//主に弾ごとのステータスの初期化
	virtual void InitStatus() {}
	void Create(std::vector<std::unique_ptr<NBullet>>&aOutBulletList, CharacterBase* aShoter);

	StatusBulletBase* GetStatus() {
		return &mStatus;
	}
	template<typename T>
	T* GetStatus() {
		static_assert(std::is_base_of<StatusBulletBase*, T*>::value == false, "Type Error.");
		return static_cast<T*>(mStatus.get());
	}

	virtual void UpdateStatus() {}

	//弾の更新
	virtual void Update();
	virtual void Render();


	//射程
	float mFiringRange;
	//速度
	float mVelocity;
	//方向ベクトル
	DXVector3 mDirection;


	//弾の種類ごとに持つ(参照のみコピーする用)
	std::shared_ptr<MSBase3DShader>mBulletShader;
	StatusBulletBase mStatus;
	//発射主
	CharacterBase* mParentPtr;

};