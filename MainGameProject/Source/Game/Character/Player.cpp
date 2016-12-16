#include "Player.h"
#include"MS3DRender.h"
#include"DXCamera.h"
#include"BulletNormal.h"
#include"Ball.h"
#include"StatusField.h"
#include"MSGravity.h"
#include"DXWorld.h"
#include"NBullet.h"
#include"ChangeStates.hxx"
Player::Player():
	CharacterBase(
{ 0.0f,6.6f,-10.0f },
{ 0.0f,0.0f,0.0f }
)
{
	mCameraOffset = { 0,0,0 };
	mCameraLen = { 0.0f,6.6f,-10.0f };
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
	UpdateMotion();
	UpdateBullets();
	//UpdateGravity();

	auto lHitTargets = UpdateCollision(true);
	for (auto&lHitTarget : lHitTargets) {
		if (lHitTarget) {
			//ボールに当たった場合、そのボールを回収する
			Ball* lBall = dynamic_cast<Ball*>(lHitTarget);
			if (lBall) {
				mField->GetBall(this);
			}
		}
	}
	UpdateCamera();
//	GetView()->SetCamera(*GetWorld(), mCameraLen);
	//mCameraLen.y += 0.01f;

	if (mField->GetTeamAlly(this)->IsCollisionBase(this) == true) {
		if (this->GetStatus()->mBall != nullptr) {
			mField->GoalProccess(this);
		}
	}

}

void Player::LivingIsRespawnWaitProc()
{

	Respawn();

}


void Player::Render()
{
	mRender->SetShader(mShader);
	mRender->Render(mTransform.get());
	RenderBullets();
}

void Player::AddBullet()
{
	if(mIsBulletShotWaiting==false){
		if (ChangeStates::IsAttackDo(this, mBltManager)) {
			if (!ChangeStates::IsBulletWaiting(this, mBltManager)) {
				ChangeStates::FirstBulletProc(this, mBltManager);
			}
		}
	}


}


void Player::InitStatus(const StatusBase* aInitStatus)
{
	CharacterBase::InitStatus(aInitStatus);
}
