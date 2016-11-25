#include "CharacterBase.h"
#include"DX11RenderResource.h"
#include"MSCollisionRayPicking.h"
#include"DXMath.hpp"
#include"MSUtility.h"
#include"MS3DRender.h"
#include"My3DShader.h"
#include"MSFbxObject.h"
#include"StatusBase.h"
#include"StaticObject.h"
#include"StatusField.h"
#include"StatusBulletBase.h"
#include<cassert>
#include"BulletManager.h"
CharacterBase::CharacterBase(){

}

CharacterBase::~CharacterBase()
{
}

void CharacterBase::Initialize(StatusField&pSetStatus)
{
	GameObjectBase::Initialize();
	mField = &pSetStatus;
	//ノーマル弾
//	mBulletNormal->Initialize();
}

void CharacterBase::SetField(StatusField&pSetStatus)
{
	mField = &pSetStatus;
}

//void CharacterBase::SetBulletMesh(MSFbxManager & aSetMesh)
//{
////	mBulletNormal->SetBulletMesh(aSetMesh);
//
//}

StatusField * CharacterBase::GetField()
{
	return mField;
}

void CharacterBase::Respawn()
{
	assert(mField != nullptr);
	mField->Respawn(this);
}

void CharacterBase::UpdateBullets()
{
	//アクティブでない弾の削除
	{
		auto lBegin = mBullets.begin();
		auto lEnd = mBullets.end();
		auto lRemoveFunc = [this](std::unique_ptr<NBullet>&aBullet) {
			return aBullet->IsActive() == false;
		};
		auto lRemoveIt = std::remove_if(lBegin, lEnd, lRemoveFunc);
		mBullets.erase(lRemoveIt, lEnd);
	}

	
	for (auto&lBullet : mBullets) {
		lBullet->Update();
	}
	//弾のインターバル更新

//	mBulletNormal->UpdateStatus();
}

void CharacterBase::RenderBullets()
{
	for (auto&lBullet : mBullets) {
		lBullet->Render();
	}
}


void CharacterBase::AddSearchTarget(GameObjectBase * aCollisionTarget)
{
	mSearchTargets.push_back(aCollisionTarget);
}

void CharacterBase::ClearSearchTarget()
{
	mSearchTargets.clear();
}

