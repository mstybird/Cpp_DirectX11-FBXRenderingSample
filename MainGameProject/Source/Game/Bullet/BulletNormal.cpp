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
	//弾のステータス設定
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
	//弾の描画に使うシェーダの設定
	mBulletShader = std::make_unique<My3DShader>();
	mBulletShader->Init();
	mBulletShader->InitVertex("Simple.hlsl");
	mBulletShader->InitPixel("Simple.hlsl");

}

//弾の生成
void BulletNormal::Create(std::vector<std::unique_ptr<BulletObject>>& aOutBulletList, CharacterBase* aShoter)
{
	printf("Create %s, From %s\n",typeid(aShoter).name(), typeid(*this).name());
	std::unique_ptr<BulletNormal>lBullet = std::make_unique<BulletNormal>();
	lBullet->Initialize();
	//初期位置設定
	auto& lShotPos = *aShoter->GetWorld()->GetMatrix().lock();
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
	aShoter->GetRenderer(lRender);
	lBullet->SetRenderer(lRender);
	DXVector3 lSpwanPos;
	lShotPos.GetT(lSpwanPos);
	lBullet->GetWorld()->SetT(lSpwanPos);
	lShotPos.GetS(lSpwanPos);
	lSpwanPos = { 0.01f ,0.01f ,0.01f };
	lBullet->GetWorld()->SetS(lSpwanPos);
	//衝突対象を登録
	//壁を登録
	for (auto& lCollision : *aShoter->GetCollisionTargets()) {
		lBullet->AddCollisionTarget(lCollision);
	}



	//捜索対象(ターゲットを登録)
	for (auto&lTarget : *aShoter->GetSearchTargets()) {
		lBullet->AddCollisionTarget(lTarget);
	}

	//レイピックの初期化
	lBullet->mRayPick->SetFramePosition(*lBullet->mTransform);

	//発射主の登録
	lBullet->mParentPtr = aShoter;

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
		//弾を消すために非アクティブにする
		SetActive(false);

		//キャラクターに当たった時の処理
		auto lChara = dynamic_cast<CharacterBase*>(lHitTarget);
		//キャラクターでない場合は次へ
		if (!lChara)continue;
		//ダメージ処理
		//auto lIsDead = ChangeStates::Damage(this, lChara);
		//死んだらそのターゲットに死亡フラグをセット
		//死んでなければ処理しない
	//	if (lIsDead == false)break;

		//死んだキャラのステータスを取得
		auto lStatus = lChara->GetStatus();
		lStatus->mLive = CharaStateFlag::DEAD;
		//ボールを持っていた場合、ボールをフィールドにセット
		if (lStatus->mBall != nullptr) {
			DXVector3 lPosition;
			lChara->GetWorld()->GetMatrix().lock()->GetT(lPosition);
			lChara->GetField()->RespawnBall(&lPosition);
			lStatus->mBall = nullptr;
		}

		//発射主の攻撃対象を空にする

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
	mRender->Render(*mTransform);
}
