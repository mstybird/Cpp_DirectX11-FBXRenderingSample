#include "NBullet.h"
#include"My3DShader.h"
#include"ChangeStates.hxx"
#include"CharacterBase.h"
#include"StatusBase.h"
#include"StatusField.h"
#include"MS3DRender.h"
NBullet::~NBullet()
{
}


void NBullet::InitStatus()
{
	GameObjectBase::Initialize();
	mStatus.mAtk = 10;
	mStatus.mCost = 2;
	mStatus.mDirection = { 0,0,1 };
	mStatus.mFiringRange = 20.0f;
	mStatus.mInterval.Set(1.0f, 0.0f, 1.0f);
	mStatus.mType = BulletDamageType::NORMAL;
	mStatus.mVelocity = 0.25f;
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
	bullet->InitStatus();
	MS3DRender* lRender;
	aShoter->GetRenderer(lRender);
	bullet->SetRenderer(lRender);

	//�����ʒu�ݒ�
	auto& lShotPos = *aShoter->GetWorld()->GetMatrix().lock();
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

	lScaling = { 0.01f ,0.01f ,0.01f };
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

	//���C�s�b�N�̏�����
	bullet->mRayPick->SetFramePosition(*bullet->mTransform);

	//���ˎ�̓o�^
	bullet->mParentPtr = aShoter;

	aOutBulletList.push_back(std::move(bullet));


}

void NBullet::Update()
{
	if (!mActive)return;

	GetWorld()->AddT(this->mStatus.mDirection*this->mStatus.mVelocity);
	//���������ꍇ
	auto lHitTargets = UpdateCollision(false);
	for (auto&lHitTarget : lHitTargets) {
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
		if (lIsDead == false)break;

		//���񂾃L�����̃X�e�[�^�X���擾
		auto lStatus = lChara->GetStatus();
		lStatus->mLive = CharaStateFlag::DEAD;
		//�{�[���������Ă����ꍇ�A�{�[�����t�B�[���h�ɃZ�b�g
		if (lStatus->mBall != nullptr) {
			DXVector3 lPosition;
			lChara->GetWorld()->GetMatrix().lock()->GetT(lPosition);
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
	mRender->Render(*mTransform);
}
