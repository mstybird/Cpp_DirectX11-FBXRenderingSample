#include "NBullet.h"
#include"My3DShader.h"
#include"ChangeStates.hxx"
#include"CharacterBase.h"
#include"StatusBase.h"
#include"StatusField.h"
#include"MS3DRender.h"
#include<DX11RenderResource.h>
#include<MyScene.h>
NBullet::~NBullet()
{
}


void NBullet::InitStatus(StatusBulletBase* aBulletStatus)
{
	mStatus = *aBulletStatus;
	mFrameResource.vLightDir = { 1,1,0,0 };
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
	//�V�F�[�_�̐ݒ�
	bullet->SetShader(mShader);
	//���\�[�X�̏�����
	bullet->Initialize();
	bullet->InitStatus(&mStatus);

	MS3DRender* lRender;
	aShoter->GetRenderer(lRender);
	bullet->SetRenderer(lRender);

	//�����ʒu�ݒ�
	auto& lShotPos = *aShoter->GetWorld()->GetMatrix();
	//�e���˕����̊m��
	D3DXVec3TransformNormal(&bullet->mStatus.mDirection, &D3DXVECTOR3(0, 0, 1), &lShotPos);

	bullet->mStatus.mDirection.Normalize();
	bullet->SetMesh(*GetMesh());
	bullet->SetCollisionMesh(*GetMesh());

	//���ˑ��x�̐ݒ�
	bullet->mStatus.mVelocity = mStatus.mVelocity;
	//�g�����X�t�H�[���̐ݒ�

	DXVector3 lSpwanPos;
	DXVector3 lScaling;

	lShotPos.GetT(lSpwanPos);
	bullet->GetWorld()->SetT(lSpwanPos);
	lScaling = GetWorld()->mScale;
	bullet->GetWorld()->SetS(lScaling);

	//�ՓˑΏۂ�o�^
	//�ǂ�o�^
	for (auto& lCollision : *aShoter->GetCollisionTargets()) {
		bullet->AddCollisionTarget(lCollision);
	}



	//�{���Ώ�(�^�[�Q�b�g��o�^)
	for (auto&lTarget : *aShoter->GetSearchTargets()) {
		bullet->AddCollisionTarget(lTarget);
	}
	//��Փ˃I�u�W�F�N�g�̒ǉ�
	bullet->AddNoCollisionObject(aShoter->GetField()->mBall);

	//���C�s�b�N�̏�����
	bullet->mRayPick->SetFramePosition(*bullet->mTransform);

	//���ˎ�̓o�^
	bullet->mParentPtr = aShoter;

	bullet->mHitEffect = this->mHitEffect;
	bullet->mShotEffect = this->mShotEffect;
	bullet->mKillEffect = this->mKillEffect;

	bullet->mShotEffect.SetPosition({ lSpwanPos.x,lSpwanPos.y,lSpwanPos.z });
	DXVector3 lRotate = aShoter->GetWorld()->mRotationCenter;
	bullet->mShotEffect.Play();
	bullet->mShotEffect.SetRotation({
		lRotate.x,
		lRotate.y,
		lRotate.z
	});
	aOutBulletList.push_back(std::move(bullet));



}

void NBullet::ShotFirstEffect(CharacterBase * aShoter)
{
	mFirstEffect = this->mHitEffect;

	DXMatrix mMatrix = *aShoter->GetWorld()->GetMatrix();
	DXVector3 lPosition;
	DXVector3 lOffset{ 0.0f,0.0f,1.0f };
	D3DXVec3TransformCoord(&lPosition, &lOffset, &mMatrix);

	mFirstEffect.SetPosition({ lPosition.x,lPosition.y,lPosition.z });
	DXVector3 lRotate = aShoter->GetWorld()->mRotationCenter;
	mFirstEffect.Play();
	mFirstEffect.SetRotation({
		lRotate.x,
		lRotate.y,
		lRotate.z
	});
}

void NBullet::AddNoCollisionObject(GameObjectBase * aObject)
{
	mNoCollisions.push_back(aObject);
}

void NBullet::ClearNocollisionList()
{
	mNoCollisions.clear();
}

void NBullet::Update()
{

	if (!mActive)return;

	GetWorld()->AddT(this->mStatus.mDirection*this->mStatus.mVelocity);
	GetWorld()->AddRC(0.0f, 3.0f, 0.0f);
	//���������ꍇ
	auto lHitTargets = UpdateCollision(false);

	for (auto&lHitTarget : lHitTargets) {

		bool lIsSkip = false;
		//�Փ˂��Ȃ��I�u�W�F�N�g�������ꍇ���O����
		for (auto& lNoCollision : mNoCollisions) {
			if (lNoCollision == lHitTarget) {
				lIsSkip = true;
				break;
			}
		}
		if (lIsSkip)continue;


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



		//�e���������߂ɔ�A�N�e�B�u�ɂ���
		SetActive(false);

		//�L�����N�^�[�ɓ����������̏���
		auto lChara = dynamic_cast<CharacterBase*>(lHitTarget);
		//�L�����N�^�[�łȂ��ꍇ�͎���
		if (!lChara)continue;
		//�_���[�W����
		auto lIsDead = ChangeStates::Damage(this, lChara);
		//���񂾂炻�̃^�[�Q�b�g�Ɏ��S�t���O���Z�b�g
		//����łȂ���Ώ������Ȃ�
		if (lIsDead == false) {
			//���������L�����̃��[�V������ύX

			//�U�����[�V�������̏ꍇ�̓L�����Z�����Ȃ�
			if (lChara->GetTransform()->GetMesh()->GetAnimation() != ValueMyScene::Chara::cAnimSkill) {
				lChara->GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimAttacked);
				lChara->GetTransform()->GetMesh()->SetLoopFlag(false);
				lChara->GetTransform()->GetMesh()->SetFrontFrame();
			}
			break;
		}
		else {
			//���񂾃L�����̃��[�V������ύX
			lChara->GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimDown);
			lChara->GetTransform()->GetMesh()->SetLoopFlag(false);
			lChara->GetTransform()->GetMesh()->SetFrontFrame();
		}

		//�L���G�t�F�N�g�Đ�
		this->mKillEffect.SetPosition({ lPosition.x,lPosition.y,lPosition.z });
		lRotate = this->GetWorld()->mRotationCenter;
		this->mKillEffect.Play();
		mKillEffect.SetRotation({
			lRotate.x,
			lRotate.y,
			lRotate.z
		});



		//���񂾃L�����̃X�e�[�^�X���擾
		auto lStatus = lChara->GetStatus();
		lStatus->mLive = CharaStateFlag::DEAD;
		lStatus->mDeadFrame = 0;

		//�{�[���������Ă����ꍇ�A�{�[�����t�B�[���h�ɃZ�b�g
		if (lStatus->mBall != nullptr) {
			DXVector3 lPosition;
			lChara->GetWorld()->GetMatrix()->GetT(lPosition);
			lChara->GetField()->RespawnBall(&lPosition);
			lStatus->mBall = nullptr;
		}

		//���ˎ�̍U���Ώۂ���ɂ���

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
	mRender->Render(this);
	//mRender->Render(this,true);
}
