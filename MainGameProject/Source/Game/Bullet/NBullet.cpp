#include "NBullet.h"
#include"My3DShader.h"
#include"ChangeStates.hxx"
#include"CharacterBase.h"
#include"StatusBase.h"
#include"StatusField.h"
#include"MS3DRender.h"
#include<DXMath.hpp>
NBullet::~NBullet()
{
}


void NBullet::InitStatus(StatusBulletBase* aBulletStatus)
{
	mStatus = *aBulletStatus;
}

void NBullet::SetEffect(::Comfort::EfkManager * aManager, ::Comfort::EffectDatabase * aDb, const int aHitID, const int aShotID, const int aKillID)
{

	auto lEffect = aDb->Get(aHitID);
	mHitEffect.SetManager(aManager);
	mHitEffect.SetEffect(lEffect);
	lEffect = aDb->Get(aShotID);
	mShotEffect.SetManager(aManager);
	mShotEffect.SetEffect(lEffect);
	lEffect = aDb->Get(aKillID);
	mKillEffect.SetManager(aManager);
	mKillEffect.SetEffect(lEffect);
}

void NBullet::Create(std::vector<std::unique_ptr<NBullet>>& aOutBulletList, CharacterBase * aShoter)
{
	printf("Create %s, From %s\n", typeid(aShoter).name(), typeid(*this).name());
	std::unique_ptr<NBullet>bullet=std::make_unique<NBullet>();
//	bullet->mShader = mShader;
	//シェーダの設定
	bullet->SetShader(mShader);
	//リソースの初期化
	bullet->Initialize();
	bullet->InitStatus(&mStatus);
	MS3DRender* lRender;
	aShoter->GetRenderer(lRender);
	bullet->SetRenderer(lRender);

	//初期位置設定
	auto& lShotPos = *aShoter->GetWorld()->GetMatrix().lock();
	//弾発射方向の確定
	D3DXVec3TransformNormal(&bullet->mStatus.mDirection, &D3DXVECTOR3(0, 0, 1), &lShotPos);

	bullet->mStatus.mDirection.Normalize();
	bullet->SetMesh(*GetMesh());
	bullet->SetCollisionMesh(*GetMesh());

	//発射速度の設定
	bullet->mStatus.mVelocity = mStatus.mVelocity;
	//トランスフォームの設定

	DXVector3 lSpwanPos;
	DXVector3 lScaling;

	lShotPos.GetT(lSpwanPos);
	bullet->GetWorld()->SetT(lSpwanPos);
	lScaling = *GetWorld()->mScale;
	bullet->GetWorld()->SetS(lScaling);

	//衝突対象を登録
	//壁を登録
	for (auto& lCollision : *aShoter->GetCollisionTargets()) {
		bullet->AddCollisionTarget(lCollision);
	}



	//捜索対象(ターゲットを登録)
	for (auto&lTarget : *aShoter->GetSearchTargets()) {
		bullet->AddCollisionTarget(lTarget);
	}

	//レイピックの初期化
	bullet->mRayPick->SetFramePosition(*bullet->mTransform);

	//発射主の登録
	bullet->mParentPtr = aShoter;

	bullet->mHitEffect = this->mHitEffect;
	bullet->mShotEffect = this->mShotEffect;
	bullet->mKillEffect = this->mKillEffect;

	bullet->mShotEffect.SetPosition({ lSpwanPos.x,lSpwanPos.y,lSpwanPos.z });
	DXVector3 lRotate = *aShoter->GetWorld()->mRotationCenter;
	bullet->mShotEffect.Play();
	bullet->mShotEffect.SetRotation({
		lRotate.x,
		lRotate.y,
		lRotate.z
	});
	aOutBulletList.push_back(std::move(bullet));



}

void NBullet::Update()
{
	if (!mActive)return;

	GetWorld()->AddT(this->mStatus.mDirection*this->mStatus.mVelocity);
	GetWorld()->AddRC(4.0f, 6.0f, 10.0f);
	//当たった場合
	auto lHitTargets = UpdateCollision(false);
	for (auto&lHitTarget : lHitTargets) {
		DXVector3 lPosition;
		GetWorld()->GetMatrix().lock()->GetT(lPosition);
		this->mHitEffect.SetPosition({ lPosition.x,lPosition.y,lPosition.z });
		DXVector3 lRotate = *this->GetWorld()->mRotationCenter;
		this->mHitEffect.Play();
		mHitEffect.SetRotation({
			lRotate.x,
			lRotate.y,
			lRotate.z
		});



		//弾を消すために非アクティブにする
		SetActive(false);

		//キャラクターに当たった時の処理
		auto lChara = dynamic_cast<CharacterBase*>(lHitTarget);
		//キャラクターでない場合は次へ
		if (!lChara)continue;
		//ダメージ処理
		auto lIsDead = ChangeStates::Damage(this, lChara);
		//死んだらそのターゲットに死亡フラグをセット
		//死んでなければ処理しない
		if (lIsDead == false)break;

		//死んだキャラのステータスを取得
		auto lStatus = lChara->GetStatus();
		lStatus->mLive = CharaStateFlag::DEAD;

		//キルエフェクト再生
		this->mKillEffect.SetPosition({ lPosition.x,lPosition.y,lPosition.z });
		lRotate = *this->GetWorld()->mRotationCenter;
		this->mKillEffect.Play();
		mKillEffect.SetRotation({
			lRotate.x,
			lRotate.y,
			lRotate.z
		});


		//ボールを持っていた場合、ボールをフィールドにセット
		if (lStatus->mBall != nullptr) {
			DXVector3 lPosition;
			lChara->GetWorld()->GetMatrix().lock()->GetT(lPosition);
			lChara->GetField()->RespawnBall(&lPosition);
			lStatus->mBall = nullptr;
		}

		//発射主の攻撃対象を空にする

		auto parent = this->mParentPtr;
		//this->mParentPtr->GetStatus()->mTargetChara = nullptr;

		break;
	}
}

void NBullet::Render()
{
	if (!mActive)return;
	assert(mRender);
	mRender->SetShader(mShader);
	mRender->Render(mTransform.get());
}
