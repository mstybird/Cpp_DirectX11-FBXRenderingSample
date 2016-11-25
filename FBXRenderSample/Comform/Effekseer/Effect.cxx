#include "Effect.hxx"
#include"DXMath.hpp"
#include<cassert>
#define _CRT_SECURE_NO_WARNINGS

void Comfort::EffectCamera::SetDXCamera(DXCamera * aCamera)
{
	mEye.X = aCamera->mEyePosition->x;
	mEye.Y = aCamera->mEyePosition->y;
	mEye.Z = aCamera->mEyePosition->z;
	mLookAt.X = aCamera->mLookPosition->x;
	mLookAt.Y = aCamera->mLookPosition->y;
	mLookAt.Z = aCamera->mLookPosition->z;
	mUp.X = aCamera->mUpVector->x;
	mUp.Y = aCamera->mUpVector->y;
	mUp.Z = aCamera->mUpVector->z;

}

::Effekseer::Matrix44 Comfort::EffectCamera::GetLookAtRH()
{
	return ::Effekseer::Matrix44().LookAtLH(mEye, mLookAt, mUp);
}

void Comfort::EffectProjection::SetDXProjection(DXProjection * aProjection)
{
	mAngle = D3DXToRadian(aProjection->mAngle);
	mWidth = 1;
	mHeight = aProjection->mAspect;
	mNear = aProjection->mNear;
	mFar = aProjection->mFar;

}

::Effekseer::Matrix44 Comfort::EffectProjection::GetPerspectiveFovRH()
{
	return ::Effekseer::Matrix44().PerspectiveFovLH(mAngle, mWidth / mHeight, mNear, mFar);
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
	//�o�^�ς݂ł���΃G���[�Ƃ���
	if (this->IsExist(aRegistID)) {
		assert(0);
	}

	wchar_t lTmpWStr[FILENAME_MAX];
	mbstowcs(lTmpWStr, aFileName, strlen(aFileName) + 1);
	auto mEffect = ::Effekseer::Effect::Create(mManagerPtr, (const EFK_CHAR*)lTmpWStr);

	//�ǂݍ��߂���o�^
	if (mEffect != nullptr) {
		mDatabase[aRegistID] = mEffect;
	}

	return mEffect != nullptr;
}

::Effekseer::Effect* Comfort::EffectDatabase::Get(const int32_t aID)const
{
	//���݂��邩���ׂ�
	//���݂��Ȃ����nullptr��Ԃ�
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
	mManager= ::Effekseer::Manager::Create(2000);

	mManager->SetSpriteRenderer(aRenderer->CreateSpriteRenderer());
	mManager->SetRibbonRenderer(aRenderer->CreateRibbonRenderer());
	mManager->SetRingRenderer(aRenderer->CreateRingRenderer());
	mManager->SetTrackRenderer(aRenderer->CreateTrackRenderer());
	mManager->SetModelRenderer(aRenderer->CreateModelRenderer());

	mManager->SetTextureLoader(aRenderer->CreateTextureLoader());
	mManager->SetModelLoader(aRenderer->CreateModelLoader());
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
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
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
		aCamera->GetLookAtRH()
	);
}

void Comfort::EfkRenderer::Release()
{
	if (mRenderer) {
		mRenderer->Destory();
		mRenderer = nullptr;
	}
}
