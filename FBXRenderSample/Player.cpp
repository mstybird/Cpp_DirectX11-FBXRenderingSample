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
}
Player::~Player()
{
}
void Player::Initialize()
{
	CharacterBase::Initialize();
	InitStatus();
}

void Player::Update()
{
	UpdateMesh();
	UpdateBullets();
	
	
	auto lHitTargets = UpdateCollision(true);
	for (auto&lHitTarget : lHitTargets) {
		if (lHitTarget) {
			//ボールに当たった場合、そのボールを回収する
			Ball* lBall = dynamic_cast<Ball*>(lHitTarget);
			if (lBall) {
				mStatus.mBall = lBall;
				mField->SetBallHolder(this);
			}
		}
	}

	GetView()->SetCamera(*GetWorld(), mCameraLen);
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

StatusPlayer * Player::GetStatus()
{
	return &mStatus;
}

void Player::InitStatus()
{
}
