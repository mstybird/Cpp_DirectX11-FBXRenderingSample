#pragma once
#include"GameObjectBase.h"
#include"DXMath.hpp"
#include"Effect.hxx"
//弾の基底クラス
class CharacterBase;
struct StatusBulletBase;
class BulletObject :public GameObjectBase {
public:
	virtual ~BulletObject();
	//主に弾のシェーダの初期化
	virtual void Initialize() = 0;
	//基本的に正面を基準に弾生成
	//生成パターンは弾ごとに異なり、正整数も異なると仮定して、ベクタで管理することを前提にする
	virtual void Create(std::vector<std::unique_ptr<BulletObject>>&aOutBulletList, CharacterBase* aShoter) = 0;
	void SetBulletMesh(MSFbxManager&aSetMesh);
	void SetEffect(::Comfort::EfkManager*aManager, ::Comfort::EffectDatabase*aDb, const int aHitID, const int aShotID);

	StatusBulletBase* GetStatus() {
		return mStatus.get();
	}
	template<typename T>
	T* GetStatus() {
		static_assert(std::is_base_of<StatusBulletBase*, T*>::value == false, "Type Error.");
		return static_cast<T*>(mStatus.get());
	}

	virtual void UpdateStatus();

	//弾の更新
	virtual void Update() = 0;
	virtual void Render() = 0;
protected:
	//射程
	float mFiringRange;
	//速度
	float mVelocity;
	//方向ベクトル
	DXVector3 mDirection;	


	//弾の種類ごとに持つ(参照のみコピーする用)
	std::unique_ptr<MSBase3DShader> mBulletShader;
	std::unique_ptr<StatusBulletBase>mStatus;
	//弾のメッシュ
	MSFbxManager* mBulletMesh;
	CharacterBase* mParentPtr;

};
