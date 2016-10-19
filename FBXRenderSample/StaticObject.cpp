#include"StaticObject.h"
#include"MS3DRender.h"

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
	mRender->Render(*mTransform);
}
