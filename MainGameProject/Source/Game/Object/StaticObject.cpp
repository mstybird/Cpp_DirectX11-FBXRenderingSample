#include"StaticObject.h"
#include"MS3DRender.h"
#include"DX11RenderResource.h"
void StaticObject::Initialize()
{
	GameObjectBase::Initialize();
}
void StaticObject::Update()
{
}

void StaticObject::Render()
{
	if (!mActive)return;
	mRender->SetShader(mShader);
	mRender->Render(this);
}

void StaticObject::ClearTrans()
{
	for (auto& lTrans : *mTransform->GetTransVector()) {
		lTrans = 1.0f;
	}
}
