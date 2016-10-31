#include"GameObjectBase.h"
#include"StaticObject.h"
#include"DX11RenderResource.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"MSFbxObject.h"
#include"MSGravity.h"
GameObjectBase::GameObjectBase()
{
	mActive = true;
	mIsCollisionScaleDefault = true;
}
GameObjectBase::~GameObjectBase()
{
}
void GameObjectBase::Initialize()
{
	mTransform = std::make_unique<DX11RenderResource>();
	mTransform->InitRenderMatrix();
	mCollisionMesh = std::make_unique<DX11RenderResource>();
	mRayPick = std::make_unique<MSCollisionRayPicking>();
	mRayPick = std::make_unique<MSCollisionRayPicking>();
	mRayPick->SetSlipFlag(true);
	mGravity = std::make_unique<MSGravity>();
}
void GameObjectBase::SetShader(MSBase3DShader * aShader)
{
	mShader = aShader;
}
void GameObjectBase::SetMesh(MSFbxManager & aSetMesh)
{
	mTransform->mMesh->Initialize(aSetMesh);
}

void GameObjectBase::SetCollisionMesh(MSFbxManager & aSetMesh)
{
	mCollisionMesh->mMesh->Initialize(aSetMesh);
}

void GameObjectBase::SetCollisionScale(float aX, float aY, float aZ)
{
	mCollisionMesh->GetWorld().lock()->SetS(aX, aY, aZ);
	mIsCollisionScaleDefault = false;
}




void GameObjectBase::SetRenderer(MS3DRender * aSetRenderer)
{
	mRender = aSetRenderer;
}

void GameObjectBase::GetRenderer(MS3DRender *& aSetRenderer)
{
	aSetRenderer = mRender;
}

void GameObjectBase::AddCollisionTarget(GameObjectBase * aCollisionTarget)
{
	mCollisionTargets.push_back(aCollisionTarget);
}

void GameObjectBase::ClearCollisionTarget()
{
	mCollisionTargets.clear();
}

void GameObjectBase::SetActive(bool aActive)
{
	mActive = aActive;
}

bool GameObjectBase::IsActive()
{
	return mActive;
}

DXWorld * GameObjectBase::GetWorld()
{
	return mTransform->GetWorld().lock().get();
}

DXCamera * GameObjectBase::GetView()
{
	return mTransform->GetCamera().lock().get();
}

DXProjection * GameObjectBase::GetProj()
{
	return mTransform->GetProjection().lock().get();
}

DX11RenderResource * GameObjectBase::GetTransform()
{
	return mTransform.get();
}

void GameObjectBase::UpdateMesh()
{
	//メッシュの更新
	mTransform->mMesh->NextFrame();
	mTransform->mMesh->Update();
}

std::vector<GameObjectBase*> GameObjectBase::UpdateCollision(bool pIsUpdatePosition)
{
	std::vector<GameObjectBase*>lHitTargets{};
	DXVector3 lResult;
	/*
		lCollision	: 衝突ターゲット
	*/
	for (auto&lCollision : mCollisionTargets) {
		auto lTmpMesh = lCollision->mTransform;
		DXVector3 lTmpS;
		//判定メッシュをコリジョン用に変更
		lCollision->GetWorld()->GetMatrix().lock()->GetT(lTmpS);
		lCollision->mCollisionMesh->GetWorld().lock()->SetT(lTmpS);
		lCollision->mCollisionMesh->GetWorld().lock()->SetRC(*lCollision->GetWorld()->mRotationCenter);


		//コリジョンスケールが別で設定されていない場合は
		//メッシュのコリジョンスケールを使う
		if (lCollision->mIsCollisionScaleDefault == true) {
			lCollision->GetWorld()->GetMatrix().lock()->GetS(lTmpS);
			lCollision->mCollisionMesh->GetWorld().lock()->SetS(lTmpS);
		}

		lCollision->mTransform = lCollision->mCollisionMesh;
		
		if (lCollision->IsActive() == false)continue;

		if (mRayPick->Collision(lResult, *mTransform, *lCollision->GetTransform())) {
			DXVector3 v = *GetWorld()->mPosition;
			if (pIsUpdatePosition == true) {
				GetWorld()->SetT(lResult);
			}
			lHitTargets.push_back(lCollision);
		}
		lCollision->mTransform = lTmpMesh;
		//lCollision->GetWorld()->SetS(lTmpS);

	}

	if (pIsUpdatePosition == true) {
		mRayPick->SetFramePosition(*mTransform);
	}
	return std::move(lHitTargets);
}
