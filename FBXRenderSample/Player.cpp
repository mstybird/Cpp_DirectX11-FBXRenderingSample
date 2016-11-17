#include "Player.h"
#include"MS3DRender.h"
#include"DXCamera.h"
#include"BulletNormal.h"
#include"Ball.h"
#include"StatusField.h"
#include"MSGravity.h"
#include"DXWorld.h"
Player::Player():
	//mCameraLen{ 0.0f,0.3f,-2.0f },
	//mCameraOffset{ -1.45f,1.5f,0.0f }
	mCameraLen{ 0.0f,50.0f,-0.1f },
	mCameraOffset{ 0.0f,0.0f,0.0f }
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
	//����ł��Ƃ��̏���
	UpdateAlive();

	//UpdateMesh();
	UpdateBullets();
	//UpdateGravity();

	auto lHitTargets = UpdateCollision(true);
	for (auto&lHitTarget : lHitTargets) {
		if (lHitTarget) {
			//�{�[���ɓ��������ꍇ�A���̃{�[�����������
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
		//�X�|�[��
		InitStatus();
		Respawn();

		break;
	default:
		break;
	}
}

void Player::UpdateGravity()
{
	mGravity->UpdateGravity();
	float f = mGravity->GetGravity();
	this->GetWorld()->AddT(
		DXWorld::TYPE_PARALLEL,
		f,
		{0,-1,0}
	);
}

void Player::UpdateCamera()
{
	//���W���擾
	auto lLookPosition = *GetWorld()->mPosition;
	//�����v�Z
	auto lEyePosition = lLookPosition + mCameraLen;
	//�J�����ʒu�����炷
	//lLookPosition += mCameraOffset;
	//lEyePosition += mCameraOffset;
	GetView()->SetCamera(*GetWorld(),mCameraLen,mCameraOffset);
}

void Player::Render()
{
	mRender->SetShader(mShader);
	mRender->Render(*mTransform);
	RenderBullets();
}

void Player::AddBullet()
{
	mBulletNormal->Create(mBullets, this);
}


void Player::InitStatus()
{
	auto lStatus = GetStatus<StatusPlayer>();
	lStatus->mBall = nullptr;
	lStatus->mEnergy.Set(5.0f, 0.0f, 2.0f);
	lStatus->mHp.Set(10.0f, .0f, 10.0f);
	lStatus->mLive = CharaStateFlag::ALIVE;
	lStatus->mTargetChara = nullptr;
	mGravity->Initialize(
		true,
		0.05f,
		-0.05f,
		0.0f,
		0.1f,
		0.01f
	);
}
