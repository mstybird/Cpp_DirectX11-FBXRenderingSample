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
#include"ChangeStates.hxx"
#include"MyScene.h"
#include<cassert>

CharacterBase::CharacterBase(const DXVector3 & aCameraLen, const DXVector3 & aCameraOffset)
	:
	cCameraLen{ aCameraLen },
	cCameraOffset{aCameraOffset},
	mIsBulletShotWaiting{ false },
	mBulletShotInterval{ -1 }
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
	mFrameResource.vLightDir = { 1,0,1,0 };
	mField = &pSetStatus;
	
	//�m�[�}���e
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

void CharacterBase::UpdateAlive()
{
	switch (mStatus->mLive)
	{
	case CharaStateFlag::ALIVE:
		mStatus->mDeadFrame = 0;
		break;
	case CharaStateFlag::DEAD:
		LivingIsDeadProc();

		break;
	case CharaStateFlag::RESPAWNWAIT:
	{
		LivingIsRespawnWaitProc();

	}
	break;
	default:
		break;
	}
}

void CharacterBase::UpdateCamera()
{

	for (auto& lCollision : mCollisionTargets) {



		auto lTmpMesh = lCollision->mTransform;
		DXVector3 lTmpS;
		//���胁�b�V�����R���W�����p�ɕύX
		lCollision->GetWorld()->GetMatrix()->GetT(lTmpS);
		lCollision->mCollisionMesh->GetWorld()->SetT(lTmpS);
		lCollision->mCollisionMesh->GetWorld()->SetRC(lCollision->GetWorld()->mRotationCenter);


		//�R���W�����X�P�[�����ʂŐݒ肳��Ă��Ȃ��ꍇ��
		//���b�V���̃R���W�����X�P�[�����g��
		if (lCollision->mIsCollisionScaleDefault == true) {
			lCollision->GetWorld()->GetMatrix()->GetS(lTmpS);
			lCollision->mCollisionMesh->GetWorld()->SetS(lTmpS);
		}

		lCollision->mTransform = lCollision->mCollisionMesh;

		if (lCollision->IsActive() == false) {
			lCollision->mTransform = lCollision->mCollisionMesh;
			continue;
		}
		MSCollisionRayPicking lRayPick;
		lRayPick.SetFramePosition(mTransform->GetCamera()->mEyePosition);
		lRayPick.SetSlipFlag(false);
		DXVector3 lDistance;
		DXVector3 v;
		int lMeshIndex;
		int lSubIndex;
		if (lRayPick.Collision(v, *mTransform, *lCollision->mTransform,&lMeshIndex,&lSubIndex)) {


			lTmpMesh->GetTransVector()->at(lMeshIndex) = 0.2f;
		}


		lCollision->mTransform = lTmpMesh;
	}
	GetView()->SetCamera(*GetWorld(), mCameraLen, mCameraOffset);
}

void CharacterBase::UpdateMotion()
{
	//���[�V���������[�v���� or ���[�V��������~���ĂȂ��Ȃ�Ώ������Ȃ�

	if(UpdateMesh()==false)return;

	auto lNowMotion = this->GetTransform()->GetMesh()->GetAnimation();

	switch (lNowMotion)
	{
	case ValueMyScene::Chara::cAnimAttack:
	case ValueMyScene::Chara::cAnimAttacked:
	case ValueMyScene::Chara::cAnimSkill:
	case ValueMyScene::Chara::cAnimWalk:
		//�A�C�h����Ԃɖ߂�
		this->GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimIdle);
		this->GetTransform()->GetMesh()->SetLoopFlag(true);
		this->GetTransform()->GetMesh()->SetFrontFrame();
		break;
	default:
		break;
	}

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

void CharacterBase::SetBulletShotWait(bool aIsWait)
{
	mIsBulletShotWaiting = aIsWait;
}

void CharacterBase::SetBulletShotInterval(int aIntervalFrame)
{
	mBulletShotInterval = aIntervalFrame;
}

void CharacterBase::LivingIsDeadProc()
{
	if (mStatus->mDeadFrame > 120) {
		mStatus->mLive = CharaStateFlag::RESPAWNWAIT;
	}
	else {
		++mStatus->mDeadFrame;
	}
}

void CharacterBase::UpdateBullets()
{
	if (mIsBulletShotWaiting==true) {
		--mBulletShotInterval;
		if (mBulletShotInterval <= 0) {
			ChangeStates::BulletShot(mBullets, this, mBltManager);
			mIsBulletShotWaiting = false;
		}
	}

	//�A�N�e�B�u�łȂ��e�̍폜
	{
		auto lBegin = mBullets.begin();
		auto lEnd = mBullets.end();
		auto lRemoveFunc = [this](std::unique_ptr<BulletObjectBase>&aBullet) {
			return aBullet->IsActive() == false;
		};
		auto lRemoveIt = std::remove_if(lBegin, lEnd, lRemoveFunc);
		mBullets.erase(lRemoveIt, lEnd);
	}

	//�A�N�e�B�u�Ȓe�̍X�V
	for (auto&lBullet : mBullets) {
		lBullet->Update();
	}
	//�e���̂��̂̃X�e�[�^�X�̍X�V
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

