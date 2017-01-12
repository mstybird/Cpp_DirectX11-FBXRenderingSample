#pragma once
#include"GameObjectBase.h"
#include"StatusBulletBase.h"
#include"CharacterBase.h"
#include"Effect.hxx"
#include<DXMath.hpp>
#include<memory>
class CharacterBase;
struct StatusBulletBase;
class MSBase3DShader;




//弾基底オブジェクト
class BulletObjectBase :public GameObjectBase {
public:
	virtual ~BulletObjectBase();
	//主に弾ごとのステータスの初期化
	virtual void InitStatus(StatusBulletBase* aBulletStatus);

	//エフェクトの登録
	void SetEffect(::Comfort::EfkManager*aManager, ::Comfort::EffectDatabase*aDb, const int aHitID, const int aShotID,const int aKillID);

	virtual void Create(std::vector<std::unique_ptr<BulletObjectBase>>&aOutBulletList, CharacterBase* aShoter) = 0;

	//発射動作開始時に呼ばれる
	virtual void ShotFirstEffect(CharacterBase* aShoter) = 0;

	void KillChara(CharacterBase* aShoter);

	StatusBulletBase* GetStatus() {
		return &mStatus;
	}
	template<typename T>
	T* GetStatus() {
		static_assert(std::is_base_of<StatusBulletBase*, T*>::value == false, "Type Error.");
		return static_cast<T*>(mStatus.get());
	}


	//衝突しないオブジェクトを遅延追加する
	void AddNoCollisionObject(GameObjectBase* aObject);

	//衝突しないオブジェクトをクリアする
	void ClearNocollisionList();

	virtual void UpdateStatus() {}

	//弾の更新
	virtual void Update()final;

	//移動処理
	virtual void UpdateMove() = 0;
	//ヒットしたが倒せなかったとき
	virtual void UpdateHitNoKill() {};
	//ヒットしてさらに倒せた時
	virtual void UpdateHitAndKill() {};


	virtual void Render()final;

	CharacterBase* GetParentChara();
	void SetParentChara(CharacterBase* aChara);

	//エフェクトの設定、取得関係

	::Comfort::EfkObject* GetEffectFirst();
	::Comfort::EfkObject* GetEffectShot();
	::Comfort::EfkObject* GetEffectHit();
	::Comfort::EfkObject* GetEffectKill();
	void SetEffectFirst(::Comfort::EfkObject& aEffect);
	void SetEffectShot(::Comfort::EfkObject& aEffect);
	void SetEffectHit(::Comfort::EfkObject& aEffect);
	void SetEffectKill(::Comfort::EfkObject& aEffect);



protected:
	//弾のステータス
	StatusBulletBase mStatus;
	//発射主
	CharacterBase* mParentPtr;

	//衝突しないオブジェクトリスト
	std::vector<GameObjectBase*>mNoCollisions;

	::Comfort::EfkObject mFirstEffect;		//発射開始時のエフェクト
	::Comfort::EfkObject mShotEffect;		//発射直後のエフェクト
	::Comfort::EfkObject mHitEffect;		//ヒット時のエフェクト
	::Comfort::EfkObject mKillEffect;		//倒した時のエフェクト



};

