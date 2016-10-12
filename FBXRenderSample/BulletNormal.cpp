#include"BulletNormal.h"
#include"My3DShader.h"
#include"CharacterBase.h"
#include"DXWorld.h"
#include"DXMatrix.h"
#include"MS3DRender.h"
#include"DX11RenderResource.h"
#include"Player.h"
#include"Enemy.h"
#include"StatusField.h"
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
void BulletNormal::Create(std::vector<std::unique_ptr<BulletObject>>& aOutBulletList, CharacterBase & aShoter)
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
	lBullet->SetCollisionMesh(*mBulletMesh);
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
	lSpwanPos = { 0.01f ,0.01f ,0.01f };
	lBullet->GetWorld()->SetS(lSpwanPos);
	//衝突対象を登録
	//壁を登録
	for (auto& lCollision : *aShoter.GetCollisionTargets()) {
		lBullet->AddCollisionTarget(lCollision);
	}



	//捜索対象(ターゲットを登録)
	for (auto&lTarget : *aShoter.GetSearchTargets()) {
		lBullet->AddCollisionTarget(lTarget);
	}

	//レイピックの初期化
	lBullet->mRayPick->SetFramePosition(*lBullet->mTransform);

	aOutBulletList.push_back(std::move(lBullet));
	return;
}

void BulletNormal::Update()
{
	if (!mActive)return;

	GetWorld()->AddT(mDirection*mVelocity);
	//当たった場合
	auto lHitTargets = UpdateCollision(false);
	for (auto&lHitTarget : lHitTargets) {
		if (lHitTarget) {
			//弾を消すために非アクティブにする
			SetActive(false);
			//当たったのが(ターゲット)プレイヤーキャラクターだった場合
			Player* lPlayer = dynamic_cast<Player*>(lHitTarget);
			/*
				ヒットしたキャラクターがボールを所持していた場合、
				ボールをリスポーンさせる
			*/
			//プレイヤーヒット時の処理
			if (lPlayer) {
				if (lPlayer->GetStatus()->mBall != nullptr) {
					lPlayer->GetField()->RespawnBall();
					lPlayer->GetStatus()->mBall = nullptr;
				}
				break;
			}
			Enemy* lEnemy = dynamic_cast<Enemy*>(lHitTarget);
			//自分以外の敵にヒットした場合
			if (lEnemy) {
				if (lEnemy->GetStatus()->mBall != nullptr) {
					lEnemy->GetField()->RespawnBall();
					lEnemy->GetStatus()->mBall = nullptr;
				}
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
