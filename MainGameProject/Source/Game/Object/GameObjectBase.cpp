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
}
void GameObjectBase::SetShader(MSBase3DShader * aShader)
{
	mShader = aShader;
}
void GameObjectBase::SetCollisionShader(MSBase3DShader * aShader)
{
	mCollisionShader = aShader;
}
MSBase3DShader * GameObjectBase::GetCollisionShader()
{
	return mCollisionShader;
}
void GameObjectBase::SetMesh(MSFbxManager & aSetMesh)
{
	mTransform->GetMesh()->Initialize(aSetMesh);
	auto lTransArray= mTransform->GetTransVector();

	auto lMeshCount = mTransform->GetMeshCount();
	lTransArray->resize(lMeshCount);
	for (auto& lTrans : *lTransArray) {
		lTrans = 1.0f;
	}

}

void GameObjectBase::SetMeshScale(float aX, float aY, float aZ)
{
	GetWorld()->SetS(aX, aY, aZ);
}

void GameObjectBase::SetCollisionMesh(MSFbxManager & aSetMesh)
{
	mCollisionMesh->GetMesh()->Initialize(aSetMesh);
	auto lTransArray = mCollisionMesh->GetTransVector();

	auto lMeshCount = mCollisionMesh->GetMeshCount();
	lTransArray->resize(lMeshCount);
	for (auto& lTrans : *lTransArray) {
		lTrans = 1.0f;
	}
}

void GameObjectBase::SetCollisionScale(float aX, float aY, float aZ)
{
	mCollisionMesh->GetWorld()->SetS(aX, aY, aZ);
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

MSFbxManager * GameObjectBase::GetMesh()
{
	return mTransform->GetMesh()->GetManager();;
}

DXWorld * GameObjectBase::GetWorld()
{
	return mTransform->GetWorld();
}

DXCamera * GameObjectBase::GetView()
{
	return mTransform->GetCamera();
}

DXProjection * GameObjectBase::GetProj()
{
	return mTransform->GetProjection();
}

DX11RenderResource * GameObjectBase::GetTransform()
{
	return mTransform.get();
}

float GameObjectBase::GetDistance(GameObjectBase * aTarget)
{
	DXVector3 lBallHolderPos;
	DXVector3 lThisPos;
	float lDistance;
	lBallHolderPos = aTarget->GetWorld()->mPosition;
	lThisPos = GetWorld()->mPosition;
	lDistance = (lBallHolderPos - lThisPos).GetDistance();

	return lDistance;
}

bool GameObjectBase::UpdateMesh()
{
	//メッシュの更新
	auto lPlayEnd = mTransform->GetMesh()->NextFrame();
	mTransform->GetMesh()->Update();
	
	return lPlayEnd;
}

std::shared_ptr<DX11RenderResource> GameObjectBase::LoadTransform( GameObjectBase* aTransform)
{
	auto lTmpMesh = aTransform->mTransform;

	DXVector3 lTmpS;
	//判定メッシュをコリジョン用に変更
	aTransform->GetWorld()->GetMatrix()->GetT(lTmpS);
	aTransform->mCollisionMesh->GetWorld()->SetT(lTmpS);
	aTransform->mCollisionMesh->GetWorld()->SetRC(aTransform->GetWorld()->mRotationCenter);

	aTransform->mTransform = aTransform->mCollisionMesh;

	if (aTransform->IsActive() == false) {
		aTransform->mTransform = lTmpMesh;
		return{};
	}

	return lTmpMesh;
}

CBResource0 * GameObjectBase::GetFrameResource()
{
	return &mFrameResource;
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
		lCollision->GetWorld()->GetMatrix()->GetT(lTmpS);
		lCollision->mCollisionMesh->GetWorld()->SetT(lTmpS);
		lCollision->mCollisionMesh->GetWorld()->SetRC(lCollision->GetWorld()->mRotationCenter);


		//コリジョンスケールが別で設定されていない場合は
		//メッシュのコリジョンスケールを使う
		if (lCollision->mIsCollisionScaleDefault == true) {
			lCollision->GetWorld()->GetMatrix()->GetS(lTmpS);
			lCollision->mCollisionMesh->GetWorld()->SetS(lTmpS);
		}

		lCollision->mTransform = lCollision->mCollisionMesh;
		
		if (lCollision->IsActive() == false) {
			lCollision->mTransform = lCollision->mCollisionMesh;
			continue;
		}
		if (mRayPick->Collision(lResult, *mTransform, *lCollision->GetTransform())) {
			if (pIsUpdatePosition == true) {
				GetWorld()->SetT(lResult);
				if (mRayPick->Collision(lResult, *mTransform, *lCollision->GetTransform())) {
					GetWorld()->SetT(lResult);
				}
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
