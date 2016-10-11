#include"BulletNormal.h"
#include"My3DShader.h"
#include"CharacterBase.h"
#include"DXWorld.h"
#include"DXMatrix.h"
#include"MS3DRender.h"
#include"DX11RenderResource.h"
#include"Player.h"
#include<cassert>

BulletNormal::BulletNormal()
{
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
void BulletNormal::Create(std::vector<std::unique_ptr<BulletObject>>& aOutBulletList, CharacterBase & aShoter)
{
	std::unique_ptr<BulletNormal>lBullet = std::make_unique<BulletNormal>();
	lBullet->Initialize();
	//�����ʒu�ݒ�
	auto& lShotPos = *aShoter.GetWorld()->GetMatrix().lock();
	//�e���˕����̊m��
	DXVector3 lDir;
	auto v = lBullet->mDirection;
	D3DXVec3TransformNormal(&lBullet->mDirection, &D3DXVECTOR3(0, 0, 1), &lShotPos);
	lBullet->mDirection.Normalize();
	lBullet->SetMesh(*mBulletMesh);
	lBullet->SetShader(mBulletShader.get());
	//���ˑ��x�̐ݒ�
	lBullet->mVelocity = 0.25f;
	//�g�����X�t�H�[���̐ݒ�
	MS3DRender*lRender;
	aShoter.GetRenderer(lRender);
	lBullet->SetRenderer(lRender);
	DXVector3 lSpwanPos;
	lShotPos.GetT(lSpwanPos);
	lBullet->GetWorld()->SetT(lSpwanPos);
	lShotPos.GetS(lSpwanPos);
	lSpwanPos *= 0.5f;
	lBullet->GetWorld()->SetS(lSpwanPos);

	//�ՓˑΏۂ�o�^
	//�ǂ�o�^
	for (auto& lCollision : *aShoter.GetCollisionTargets()) {
		lBullet->AddCollisionTarget(lCollision);
	}



	//�{���Ώ�(�^�[�Q�b�g��o�^)
	for (auto&lTarget : *aShoter.GetSearchTargets()) {
		lBullet->AddCollisionTarget(lTarget);
	}

	//���C�s�b�N�̏�����
	lBullet->mRayPick->SetFramePosition(*lBullet->mTransform);

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
		if (lHitTarget) {
			//�e���������߂ɔ�A�N�e�B�u�ɂ���
			SetActive(false);
			//���������̂�(�^�[�Q�b�g)�L�����N�^�[�������ꍇ
			Player* lPlayer = dynamic_cast<Player*>(lHitTarget);

			//�v���C���[�q�b�g���̏���
			if (lPlayer) {
				int a = 10;
				int b = 10 * a;

			}

		}
	}


	//UpdateCollision();
}

void BulletNormal::Render()
{
	if (!mActive)return;
	assert(mRender);
	mRender->SetShader(mShader);
	mRender->Render(*mTransform);
}
