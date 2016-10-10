#include"BulletNormal.h"
#include"My3DShader.h"
#include"GameObjectBase.h"
#include"DXWorld.h"
#include"DXMatrix.h"
#include"MS3DRender.h"
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
void BulletNormal::Create(std::vector<std::unique_ptr<BulletObject>>& aOutBulletList, GameObjectBase & aShoter)
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
	aOutBulletList.push_back(std::move(lBullet));
	return;
}

void BulletNormal::Update()
{
	GetWorld()->AddT(mDirection*mVelocity);
}

void BulletNormal::Render()
{
	assert(mRender);
	mRender->SetShader(mShader);
	mRender->Render(*mTransform);
}
