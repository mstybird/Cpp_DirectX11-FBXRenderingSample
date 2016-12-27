#include "Effect.hxx"
#include"DXMath.hpp"
#include<cassert>
#define _CRT_SECURE_NO_WARNINGS

void Comfort::EffectCamera::SetDXCamera(DXCamera * aCamera)
{
	mEye.X = aCamera->mEyePosition.x;
	mEye.Y = aCamera->mEyePosition.y;
	mEye.Z = aCamera->mEyePosition.z;
	mLookAt.X = aCamera->mLookPosition.x;
	mLookAt.Y = aCamera->mLookPosition.y;
	mLookAt.Z = aCamera->mLookPosition.z;
	mUp.X = 0;
	mUp.Y = 1;
	mUp.Z = 0;

	auto m1 = GetLookAtLH();
	auto m2 = *aCamera->GetMatrix();

	//mUp.X = aCamera.mUpVector->x;
	//mUp.Y = aCamera->mUpVector->y;
	//mUp.Z = aCamera->mUpVector->z;
	//printf("%.2f:%.2f:%.2f\n", mUp.X, mUp.Y, mUp.Z);

}

::Effekseer::Matrix44 Comfort::EffectCamera::GetLookAtLH()
{
	return ::Effekseer::Matrix44().LookAtLH(mEye, mLookAt, mUp);
}

void Comfort::EffectProjection::SetDXProjection(DXProjection * aProjection)
{
	mAngle = aProjection->mAngle;
	mAspect = aProjection->mAspect;
	mNear = aProjection->mNear;
	mFar = aProjection->mFar;


	auto m1 = GetPerspectiveFovRH();
	auto m2=aProjection->GetMatrix();

}

::Effekseer::Matrix44 Comfort::EffectProjection::GetPerspectiveFovRH()
{
	return ::Effekseer::Matrix44().PerspectiveFovLH(D3DXToRadian(mAngle), mAspect, mNear, mFar);
}

Comfort::EffectDatabase::~EffectDatabase()
{
	Release();
}

void Comfort::EffectDatabase::Initialize(::Effekseer::Manager *& aManager)
{
	mManagerPtr = aManager;
}

bool Comfort::EffectDatabase::Load(const char * aFileName, const int32_t aRegistID)
{
	//“o˜^Ï‚Ý‚Å‚ ‚ê‚ÎƒGƒ‰[‚Æ‚·‚é
	if (this->IsExist(aRegistID)) {
		assert(0);
	}

	wchar_t lTmpWStr[FILENAME_MAX];
	size_t lReturnSize;
	mbstowcs_s(&lReturnSize, lTmpWStr, aFileName, strlen(aFileName) + 1);
	//mbstowcs(lTmpWStr, aFileName, strlen(aFileName) + 1);
	auto mEffect = ::Effekseer::Effect::Create(mManagerPtr, (const EFK_CHAR*)lTmpWStr);

	//“Ç‚Ýž‚ß‚½‚ç“o˜^
	if (mEffect != nullptr) {
		mDatabase[aRegistID] = mEffect;
	}

	return mEffect != nullptr;
}

::Effekseer::Effect* Comfort::EffectDatabase::Get(const int32_t aID)const
{
	//‘¶Ý‚·‚é‚©’²‚×‚é
	//‘¶Ý‚µ‚È‚¯‚ê‚Înullptr‚ð•Ô‚·
	if (!this->IsExist(aID)) {
		return nullptr;
	}


	return mDatabase.at(aID);
}

bool Comfort::EffectDatabase::IsExist(const int32_t aID)const
{
	return mDatabase.count(aID) != 0;
}

void Comfort::EffectDatabase::CleanAll()
{
}

void Comfort::EffectDatabase::Clean(const int32_t aID)
{
}

void Comfort::EffectDatabase::Release()
{
	for (decltype(auto)lPair : mDatabase) {
		ES_SAFE_RELEASE(lPair.second);
	}
}

Comfort::EfkManager::~EfkManager()
{
	Release();
}

void Comfort::EfkManager::Initialize(::EffekseerRenderer::Renderer *& aRenderer, const int aInstanceMax)
{
	mManager = ::Effekseer::Manager::Create(2000);

	mManager->SetSpriteRenderer(aRenderer->CreateSpriteRenderer());
	mManager->SetRibbonRenderer(aRenderer->CreateRibbonRenderer());
	mManager->SetRingRenderer(aRenderer->CreateRingRenderer());
	mManager->SetTrackRenderer(aRenderer->CreateTrackRenderer());
	mManager->SetModelRenderer(aRenderer->CreateModelRenderer());

	mManager->SetTextureLoader(aRenderer->CreateTextureLoader());
	mManager->SetModelLoader(aRenderer->CreateModelLoader());
	mManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
}

::Effekseer::Manager *& Comfort::EfkManager::GetManager()
{
	return mManager;
}

void Comfort::EfkManager::Update()
{
	mManager->Update();
}

void Comfort::EfkManager::Release()
{
	if (mManager) {
		mManager->Destroy();
		mManager = nullptr;
	}
}

void Comfort::EfkObject::SetEffect(::Effekseer::Effect *& aEffect)
{
	mEffect = aEffect;
}

void Comfort::EfkObject::SetManager(EfkManager * aManager)
{
	mParentManager = aManager->GetManager();
}

void Comfort::EfkObject::SetPosition(const::Effekseer::Vector3D & aPosition)
{
	mPosition = aPosition;
	if (mHandle != -1) {
		mParentManager->SetLocation(mHandle, mPosition);
		
	}
}

void Comfort::EfkObject::AddPosition(const::Effekseer::Vector3D & aPosition)
{
	mPosition += aPosition;
	if (mHandle != -1) {
		mParentManager->SetLocation(mHandle, mPosition);
	}
}

void Comfort::EfkObject::SetRotation(const::Effekseer::Vector3D & aRotate)
{
	if (mHandle != -1) {
		
		mParentManager->SetRotation(mHandle, { 0,1,0 }, D3DXToRadian(aRotate.Y));
		
	}
}

void Comfort::EfkObject::Play()
{
	mHandle = mParentManager->Play(mEffect, mPosition.X, mPosition.Y, mPosition.Z);
}

void Comfort::EfkObject::Stop()
{
	mParentManager->StopEffect(mHandle);
}

Comfort::EfkRenderer::~EfkRenderer()
{
	Release();
}

void Comfort::EfkRenderer::Initialize(ID3D11Device * aDevice, ID3D11DeviceContext * aDeviceContext, const int aDrawMax)
{
	mRenderer = ::EffekseerRendererDX11::Renderer::Create(aDevice, aDeviceContext, 2000);
}

::EffekseerRenderer::Renderer *& Comfort::EfkRenderer::GetRenderer()
{
	// TODO: return ƒXƒe[ƒgƒƒ“ƒg‚ð‚±‚±‚É‘}“ü‚µ‚Ü‚·
	return mRenderer;
}

void Comfort::EfkRenderer::RenderAll(EfkManager*aManager)
{
	mRenderer->BeginRendering();
	aManager->GetManager()->Draw();
	mRenderer->EndRendering();
}

void Comfort::EfkRenderer::SetProjection(EffectProjection * aProjection)
{
	mRenderer->SetProjectionMatrix(
		aProjection->GetPerspectiveFovRH()
	);
}

void Comfort::EfkRenderer::SetCamera(EffectCamera * aCamera)
{

	mRenderer->SetCameraMatrix(
		aCamera->GetLookAtLH()
	);
}

void Comfort::EfkRenderer::SetCamera(DXCamera * aCamera)
{
	aCamera->GetMatrix();
	aCamera->GetMatrix();
}

void Comfort::EfkRenderer::Release()
{
	if (mRenderer) {
		mRenderer->Destory();
		mRenderer = nullptr;
	}
}