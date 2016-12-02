#include "Player.h"
#include"MS3DRender.h"
#include"DXCamera.h"
#include"BulletNormal.h"
#include"Ball.h"
#include"StatusField.h"
#include"MSGravity.h"
#include"DXWorld.h"
#include"NBullet.h"
Player::Player()
{
	mCameraLen = { 0.0f,50.0f,-0.1f };
	mCameraOffset = { 0.0f,0.0f,0.0f };
	//mBulletNormal = std::make_unique<BulletNormal>();
	mStatus = std::make_unique<StatusPlayer>();
}
Player::~Player()
{
}
void Player::Initialize(StatusField&pSetStatus)
{
	CharacterBase::Initialize(pSetStatus);
}

void Player::Update()
{
	//死んでたときの処理
	UpdateAlive();

	//UpdateMesh();
	UpdateBullets();
	//UpdateGravity();

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
	UpdateCamera();
//	GetView()->SetCamera(*GetWorld(), mCameraLen);
	//mCameraLen.y += 0.01f;
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
		Respawn();

		break;
	default:
		break;
	}
}


void Player::Render()
{
	mRender->SetShader(mShader);
	mRender->Render(mTransform.get());
	RenderBullets();
}

void Player::AddBullet()
{
	mBltManager->Create(mBullets, this);
	//mBulletNormal->Create(mBullets, this);
}


void Player::InitStatus(const StatusBase* aInitStatus)
{
	CharacterBase::InitStatus(aInitStatus);
}
