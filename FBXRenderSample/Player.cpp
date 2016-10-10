#include "Player.h"
#include"MS3DRender.h"
#include"DXCamera.h"
#include"BulletNormal.h"
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
	UpdateCollision();
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

void Player::InitStatus()
{
}
