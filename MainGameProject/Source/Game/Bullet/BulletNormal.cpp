#include"BulletNormal.h"
#include"My3DShader.h"
#include"CharacterBase.h"
#include"DXMath.hpp"
#include"MS3DRender.h"
#include"DX11RenderResource.h"
#include"Player.h"
#include"Enemy.h"
#include"StatusField.h"
#include"StatusBulletNormal.h"
#include"ChangeStates.hxx"
#include<DX11FBXLoader.hpp>

#include<cassert>

BulletNormal::BulletNormal()
{
	mStatus = std::make_unique<StatusBulletNormal>();
	//�e�̃X�e�[�^�X�ݒ�
	mStatus->mInterval.Set(100.0f, .0f, .0f);
	mStatus->mAtk = 10.0f;
	mStatus->mCost = 2;
	mStatus->mType = BulletDamageType::NORMAL;
}
BulletNormal::~BulletNormal()
{
}
void BulletNormal::Initialize()
{
	GameObjectBase::Initialize();
	//�e�̕`��Ɏg���V�F�[�_�̐ݒ�
	mBulletShader = std::make_unique<My3DShader>();
	mBulletShader->Init();
	mBulletShader->InitVertex("Simple.hlsl");
	mBulletShader->InitPixel("Simple.hlsl");

}

//�e�̐���
void BulletNormal::Create(std::vector<std::unique_ptr<BulletObject>>& aOutBulletList, CharacterBase* aShoter)
{
	printf("Create %s, From %s\n",typeid(aShoter).name(), typeid(*this).name());
	std::unique_ptr<BulletNormal>lBullet = std::make_unique<BulletNormal>();
	lBullet->Initialize();
	//�����ʒu�ݒ�
	auto& lShotPos = *aShoter->GetWorld()->GetMatrix();
	//�e���˕����̊m��
	DXVector3 lDir;
	auto v = lBullet->mDirection;
	D3DXVec3TransformNormal(&lBullet->mDirection, &D3DXVECTOR3(0, 0, 1), &lShotPos);
	lBullet->mDirection.Normalize();
	lBullet->SetMesh(*mBulletMesh);
	lBullet->SetCollisionMesh(*mBulletMesh);
	lBullet->SetShader(mBulletShader.get());
	//���ˑ��x�̐ݒ�
	lBullet->mVelocity = 0.25f;
	//�g�����X�t�H�[���̐ݒ�
	MS3DRender*lRender;
	aShoter->GetRenderer(lRender);
	lBullet->SetRenderer(lRender);
	DXVector3 lSpwanPos;
	lShotPos.GetT(lSpwanPos);
	lBullet->GetWorld()->SetT(lSpwanPos);
	lShotPos.GetS(lSpwanPos);
	lSpwanPos = { 0.01f ,0.01f ,0.01f };
	lBullet->GetWorld()->SetS(lSpwanPos);
	//�ՓˑΏۂ�o�^
	//�ǂ�o�^
	for (auto& lCollision : *aShoter->GetCollisionTargets()) {
		lBullet->AddCollisionTarget(lCollision);
	}



	//�{���Ώ�(�^�[�Q�b�g��o�^)
	for (auto&lTarget : *aShoter->GetSearchTargets()) {
		lBullet->AddCollisionTarget(lTarget);
	}

	//���C�s�b�N�̏�����
	lBullet->mRayPick->SetFramePosition(*lBullet->mTransform);

	//���ˎ�̓o�^
	lBullet->mParentPtr = aShoter;

	aOutBulletList.push_back(std::move(lBullet));
	return;
}

void BulletNormal::Update()
{
	if (!mActive)return;

	GetWorld()->AddT(mDirection*mVelocity);
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
		//auto lIsDead = ChangeStates::Damage(this, lChara);
		//���񂾂炻�̃^�[�Q�b�g�Ɏ��S�t���O���Z�b�g
		//����łȂ���Ώ������Ȃ�
	//	if (lIsDead == false)break;

		//���񂾃L�����̃X�e�[�^�X���擾
		auto lStatus = lChara->GetStatus();
		lStatus->mLive = CharaStateFlag::DEAD;
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

void BulletNormal::Render()
{
	if (!mActive)return;
	assert(mRender);
	mRender->SetShader(mShader);
	mRender->Render(this);
}
