#include "CharacterBase.h"
#include"DX11RenderResource.h"
#include"MSCollisionRayPicking.h"
#include"DXMath.hpp"
#include"MSUtility.h"
#include"MS3DRender.h"
#include"My3DShader.h"
#include"MSFbxObject.h"
#include"StatusBase.h"
#include"StaticObject.h"
#include"StatusField.h"
#include"StatusBulletBase.h"
#include"BulletManager.h"
#include"MSCollisionRay.h"
#include<cassert>

CharacterBase::CharacterBase(const DXVector3 & aCameraLen, const DXVector3 & aCameraOffset)
	:
	cCameraLen{ aCameraLen },
	cCameraOffset{aCameraOffset}
{
	mCameraOffset = cCameraOffset;
	mCameraLen = cCameraLen;
}

CharacterBase::CharacterBase(){

}

CharacterBase::~CharacterBase()
{
}

void CharacterBase::Initialize(StatusField&pSetStatus)
{
	GameObjectBase::Initialize();
	mField = &pSetStatus;
	//ノーマル弾
//	mBulletNormal->Initialize();
}

void CharacterBase::InitStatus(const StatusBase * aInitStatus)
{
	auto lStatus = GetStatus();
	lStatus->mBall = nullptr;
	lStatus->mEnergy = aInitStatus->mEnergy;
	lStatus->mHp = aInitStatus->mHp;
	lStatus->mLive = CharaStateFlag::ALIVE;
	lStatus->mAllyNear = false;
	lStatus->mBallAllyNear= false;
	lStatus->mTargetChara = nullptr;
}

void CharacterBase::UpdateCamera()
{
	//座標を取得
	auto lLookPosition = *GetWorld()->mPosition;
	//差分計算
	auto lEyePosition = lLookPosition + cCameraLen;

	for (auto& lCollision : mCollisionTargets) {
		auto lTmpMesh = lCollision->mTransform;
		DXVector3 lTmpS;
		//判定メッシュをコリジョン用に変更
		lCollision->GetWorld()->GetMatrix().lock()->GetT(lTmpS);
		lCollision->mCollisionMesh->GetWorld().lock()->SetT(lTmpS);
		lCollision->mCollisionMesh->GetWorld().lock()->SetRC(*lCollision->GetWorld()->mRotationCenter);


		//コリジョンスケールが別で設定されていない場合は
		//メッシュのコリジョンスケールを使う
		if (lCollision->mIsCollisionScaleDefault == true) {
			lCollision->GetWorld()->GetMatrix().lock()->GetS(lTmpS);
			lCollision->mCollisionMesh->GetWorld().lock()->SetS(lTmpS);
		}

		lCollision->mTransform = lCollision->mCollisionMesh;

		if (lCollision->IsActive() == false) {
			lCollision->mTransform = lCollision->mCollisionMesh;
			continue;
		}
		MSCollisionRayPicking lRayPick;
		lRayPick.SetFramePosition(*mTransform->GetCamera().lock()->mEyePosition);
		lRayPick.SetSlipFlag(false);
		DXVector3 lDistance;
		DXVector3 v;
		int lMeshIndex;
		int lSubIndex;
		if (lRayPick.Collision(v, *mTransform, *lCollision->mTransform,&lMeshIndex,&lSubIndex)) {
			mTransform->GetMesh()->GetCurrentMeshData();
		}


		lCollision->mTransform = lTmpMesh;
	}

	//カメラ位置をずらす
	lLookPosition += mCameraOffset;
	lEyePosition += mCameraOffset;
	GetView()->SetCamera(*GetWorld(), mCameraLen, mCameraOffset);
}

void CharacterBase::SetField(StatusField&pSetStatus)
{
	mField = &pSetStatus;
}

//void CharacterBase::SetBulletMesh(MSFbxManager & aSetMesh)
//{
////	mBulletNormal->SetBulletMesh(aSetMesh);
//
//}

StatusField * CharacterBase::GetField()
{
	return mField;
}

void CharacterBase::Respawn()
{
	assert(mField != nullptr);
	mField->Respawn(this);
}

void CharacterBase::UpdateBullets()
{
	//アクティブでない弾の削除
	{
		auto lBegin = mBullets.begin();
		auto lEnd = mBullets.end();
		auto lRemoveFunc = [this](std::unique_ptr<NBullet>&aBullet) {
			return aBullet->IsActive() == false;
		};
		auto lRemoveIt = std::remove_if(lBegin, lEnd, lRemoveFunc);
		mBullets.erase(lRemoveIt, lEnd);
	}

	//アクティブな弾の更新
	for (auto&lBullet : mBullets) {
		lBullet->Update();
	}
	//弾そのもののステータスの更新
	for (auto& lBullet : mBltManager->GetStatusArray(this)) {
		lBullet->mInterval.AddFix(lBullet->mIntervalRecovery);
	}

//	mBulletNormal->UpdateStatus();
}

void CharacterBase::RenderBullets()
{
	for (auto&lBullet : mBullets) {
		lBullet->Render();
	}
}


void CharacterBase::SetDefaultStatus(const StatusBase & aStatus)
{
	if (!mDefaultStatus) {
		mDefaultStatus = std::make_unique<StatusBase>();
	}
	*mDefaultStatus = aStatus;
}

const StatusBase * CharacterBase::GetDefaultStatus() const
{
	return mDefaultStatus.get();
}

void CharacterBase::AddSearchTarget(GameObjectBase * aCollisionTarget)
{
	mSearchTargets.push_back(aCollisionTarget);
}

void CharacterBase::ClearSearchTarget()
{
	mSearchTargets.clear();
}

