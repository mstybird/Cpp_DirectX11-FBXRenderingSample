#include "CharacterBase.h"
#include"DX11RenderResource.h"
#include"MSCollisionRayPicking.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"MSUtility.h"
#include"MS3DRender.h"
#include"My3DShader.h"
#include"MSFbxObject.h"

#include"StaticObject.h"
#include"BulletNormal.h"
#include"BulletObject.h"


#include<cassert>


CharacterBase::CharacterBase(){

}

CharacterBase::~CharacterBase()
{
}

void CharacterBase::Initialize()
{
	GameObjectBase::Initialize();
	//ƒm[ƒ}ƒ‹’e
	mBulletNormal->Initialize();
}

void CharacterBase::SetBulletMesh(MSFbxManager & aSetMesh)
{
	mBulletNormal->SetBulletMesh(aSetMesh);
}

void CharacterBase::UpdateBullets()
{
	for (auto&lBullet : mBullets) {
		lBullet->Update();
	}
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

