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
	//弾の描画に使うシェーダの設定
	mBulletShader = std::make_unique<My3DShader>();
	mBulletShader->Init();
	mBulletShader->InitVertex("Simple.hlsl");
	mBulletShader->InitPixel("Simple.hlsl");

}

//弾の生成
void BulletNormal::Create(std::vector<std::unique_ptr<BulletObject>>& aOutBulletList, GameObjectBase & aShoter)
{
	std::unique_ptr<BulletNormal>lBullet = std::make_unique<BulletNormal>();
	lBullet->Initialize();
	//初期位置設定
	auto& lShotPos = *aShoter.GetWorld()->GetMatrix().lock();
	//弾発射方向の確定
	DXVector3 lDir;
	auto v = lBullet->mDirection;
	D3DXVec3TransformNormal(&lBullet->mDirection, &D3DXVECTOR3(0, 0, 1), &lShotPos);
	lBullet->mDirection.Normalize();
	lBullet->SetMesh(*mBulletMesh);
	lBullet->SetShader(mBulletShader.get());
	//発射速度の設定
	lBullet->mVelocity = 0.25f;
	//トランスフォームの設定
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
