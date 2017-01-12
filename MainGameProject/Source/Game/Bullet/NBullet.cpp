#include "NBullet.h"
#include"My3DShader.h"
#include"ChangeStates.hxx"
#include"CharacterBase.h"
#include"StatusBase.h"
#include"StatusField.h"
#include"MS3DRender.h"
#include<DX11RenderResource.h>
#include<MyScene.h>
BulletObjectBase::~BulletObjectBase()
{
}


void BulletObjectBase::InitStatus(StatusBulletBase* aBulletStatus)
{
	mStatus = *aBulletStatus;
	mFrameResource.vLightDir = { 1,1,0,0 };
}

void BulletObjectBase::SetEffect(::Comfort::EfkManager * aManager, ::Comfort::EffectDatabase * aDb, const int aHitID, const int aShotID, const int aKillID)
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

void BulletObjectBase::KillChara(CharacterBase* aShoter) {

	//キルエフェクト再生
	DXVector3 lPosition;
	DXVector3 lRotate;
	GetWorld()->GetMatrix()->GetT(lPosition);
	lRotate = GetWorld()->mRotationCenter;
	this->mKillEffect.SetPosition({ lPosition.x,lPosition.y,lPosition.z });
	lRotate = this->GetWorld()->mRotationCenter;
	this->mKillEffect.Play();
	mKillEffect.SetRotation({
		lRotate.x,
		lRotate.y,
		lRotate.z
	});



	//死んだキャラのステータスを取得
	auto lStatus = aShoter->GetStatus();
	lStatus->mLive = CharaStateFlag::DEAD;
	lStatus->mDeadFrame = 0;

	//ボールを持っていた場合、ボールをフィールドにセット
	if (lStatus->mBall != nullptr) {
		DXVector3 lPosition;
		aShoter->GetWorld()->GetMatrix()->GetT(lPosition);
		aShoter->GetField()->RespawnBall(&lPosition);
		lStatus->mBall = nullptr;
	}

	//発射主の攻撃対象を空にする

	auto parent = this->mParentPtr;
	//this->mParentPtr->GetStatus()->mTargetChara = nullptr;

}



void BulletObjectBase::AddNoCollisionObject(GameObjectBase * aObject)
{
	mNoCollisions.push_back(aObject);
}

void BulletObjectBase::ClearNocollisionList()
{
	mNoCollisions.clear();
}

void BulletObjectBase::Update() {
	if (!mActive)return;

	//GetWorld()->AddT(this->mStatus.mDirection*this->mStatus.mVelocity);
	//GetWorld()->AddRC(0.0f, 3.0f, 0.0f);

	UpdateMove();

	//当たった場合
	auto lHitTargets = UpdateCollision(false);

	for (auto&lHitTarget : lHitTargets) {

		bool lIsSkip = false;
		//衝突しないオブジェクトだった場合除外する
		for (auto& lNoCollision : mNoCollisions) {
			if (lNoCollision == lHitTarget) {
				lIsSkip = true;
				break;
			}
		}
		if (lIsSkip)continue;

		//ヒットエフェクト再生
		DXVector3 lPosition;
		GetWorld()->GetMatrix()->GetT(lPosition);
		this->mHitEffect.SetPosition({ lPosition.x,lPosition.y,lPosition.z });
		DXVector3 lRotate = this->GetWorld()->mRotationCenter;
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
		if (lIsDead == false) {
			//当たったキャラのモーションを変更

			//攻撃モーション中の場合はキャンセルしない
			if (lChara->GetTransform()->GetMesh()->GetAnimation() != ValueMyScene::Chara::cAnimSkill) {
				lChara->GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimAttacked);
				lChara->GetTransform()->GetMesh()->SetLoopFlag(false);
				lChara->GetTransform()->GetMesh()->SetFrontFrame();

				//固有処理
				UpdateHitNoKill();
			}
			break;
		}
		else {
			//死んだキャラのモーションを変更
			lChara->GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimDown);
			lChara->GetTransform()->GetMesh()->SetLoopFlag(false);
			lChara->GetTransform()->GetMesh()->SetFrontFrame();
			//固有処理
			UpdateHitAndKill();
		}

		KillChara(lChara);

		break;
	}

}

void BulletObjectBase::Render() {
	if (!mActive)return;
	assert(mRender);
	mRender->SetShader(mShader);
	mRender->Render(this);

}

CharacterBase * BulletObjectBase::GetParentChara()
{
	return mParentPtr;
}

void BulletObjectBase::SetParentChara(CharacterBase * aChara)
{
	mParentPtr = aChara;
}

::Comfort::EfkObject * BulletObjectBase::GetEffectFirst()
{
	return &mFirstEffect;
}

::Comfort::EfkObject * BulletObjectBase::GetEffectShot()
{
	return &mShotEffect;
}

::Comfort::EfkObject * BulletObjectBase::GetEffectHit()
{
	return &mHitEffect;
}

::Comfort::EfkObject * BulletObjectBase::GetEffectKill()
{
	return &mKillEffect;
}

void BulletObjectBase::SetEffectFirst(::Comfort::EfkObject & aEffect)
{
	mFirstEffect = aEffect;
}

void BulletObjectBase::SetEffectShot(::Comfort::EfkObject & aEffect)
{
	mShotEffect = aEffect;
}

void BulletObjectBase::SetEffectHit(::Comfort::EfkObject & aEffect)
{
	mHitEffect = aEffect;
}

void BulletObjectBase::SetEffectKill(::Comfort::EfkObject & aEffect)
{
	mKillEffect = aEffect;
}

