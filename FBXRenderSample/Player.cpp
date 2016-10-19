#include "Player.h"
#include"MS3DRender.h"
#include"DXCamera.h"
#include"BulletNormal.h"
#include"Ball.h"
#include"StatusField.h"
Player::Player():
	mCameraLen{ 0,25,-10 }
{
	mBulletNormal = std::make_unique<BulletNormal>();
	mStatus = std::make_unique<StatusPlayer>();
}
Player::~Player()
{
}
void Player::Initialize(StatusField&pSetStatus)
{
	CharacterBase::Initialize(pSetStatus);
	InitStatus();
}

void Player::Update()
{
	//死んでたときの処理
	UpdateAlive();

	UpdateMesh();
	UpdateBullets();
	
	
	auto lHitTargets = UpdateCollision(true);
	for (auto&lHitTarget : lHitTargets) {
		if (lHitTarget) {
			//ボールに当たった場合、そのボールを回収する
			Ball* lBall = dynamic_cast<Ball*>(lHitTarget);
			if (lBall) {
				GetStatus<StatusPlayer>()->mBall = lBall;
				mField->SetBallHolder(this);
			}
		}
	}

	GetView()->SetCamera(*GetWorld(), mCameraLen);
}

void Player::UpdateAlive()
{
	switch (mStatus->mLive)
	{
	case CharaStateFlag::ALIVE:
		break;
	case CharaStateFlag::DEAD:
		mStatus->mLive = CharaStateFlag::RESPAWNWAIT;
		break;
	case CharaStateFlag::RESPAWNWAIT:
		//スポーン
		InitStatus();
		Respawn();

		break;
	default:
		break;
	}
}

void Player::Render()
{
	mRender->SetShader(mShader);
	mRender->Render(*mTransform);
	RenderBullets();
}

void Player::AddBullet()
{
	mBulletNormal->Create(mBullets, *this);
}


void Player::InitStatus()
{
	auto lStatus = GetStatus<StatusPlayer>();
	lStatus->mBall = nullptr;
	lStatus->mEnergy.Set(5.0f, 0.0f, 2.0f);
	lStatus->mHp.Set(10.0f, .0f, 10.0f);
	lStatus->mLive = CharaStateFlag::ALIVE;
	lStatus->mTarget = nullptr;
}
