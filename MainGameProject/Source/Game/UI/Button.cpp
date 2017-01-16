#include "Button.h"
#include<MSSprite2DRender.h>
Button::Button() :
	mSize{ 100,100 },
	mState{ButtonState::Normal}
{
}
Button::~Button()
{
}
void Button::SetTextures(DX11TextureManager * aManager, const int aNormalID, const int aActiveID, const int aPushID, const int aDisableID)
{
	mImageNormal.SetTexture(*aManager, aNormalID);
	mImageActive.SetTexture(*aManager, aActiveID);
	mImagePush.SetTexture(*aManager, aPushID);
	mImageDisable.SetTexture(*aManager, aDisableID);
}

void Button::SetSize(const float & aWidth, const float & aHeight)
{
	mSize.Set(aWidth, aHeight);
}

DXVector2 Button::GetSizeGlobal()
{
	return mSize*mGlobalScale;
}

DXVector2 Button::GetSizeLocal()
{
	return mSize;
}

DXVector2 Button::GetScalingSize(UIBase * aParent)
{
	if (aParent != nullptr) {
		StoreGlobalData();
		mGlobalScale *= *aParent->GetGlobalScale();
	}

	DXVector2 lResult = mSize * mGlobalScale;

	if (aParent != nullptr) {
		LoadGlobalData();
	}

	return lResult;
}

ButtonState Button::SetState(const ButtonState aState)
{
	return mState=aState;
}

ButtonState Button::GetState()
{
	return mState;
}

ButtonState Button::SetActive()
{
	ButtonState lBefore = mState;
	mState = ButtonState::Active;
	return lBefore;
}

ButtonState Button::SetDisable()
{
	ButtonState lBefore = mState;
	mState = ButtonState::Disable;
	return lBefore;
}

ButtonState Button::SetNormal()
{
	ButtonState lBefore = mState;
	mState = ButtonState::Normal;
	return lBefore;
}

ButtonState Button::SetPushing()
{
	ButtonState lBefore = mState;
	mState = ButtonState::Push;
	return lBefore;
}

bool Button::IsActive()
{
	return 	mState == ButtonState::Active;
}

bool Button::IsDisable()
{
	return mState == ButtonState::Disable;
}

bool Button::IsNormal()
{
	return mState == ButtonState::Normal;
}

bool Button::IsPushing()
{
	return mState == ButtonState::Push;
}

void Button::Render(MSSprite2DRender & aRender, UIBase * aParent)
{
	if (aParent != nullptr) {
		StoreGlobalData();

		mGlobalPosition += *aParent->GetGlobalPosition();
		mGlobalScale *= *aParent->GetGlobalScale();
	}

	//Œ»Ý‚Ìƒ{ƒ^ƒ“‚Ì‚Ý•ÏX
	auto lButton = GetSelecttingImage();
	lButton->SetSize(mSize);
	lButton->SetPosition(mGlobalPosition);
	lButton->SetScale(mGlobalScale);

	if (aParent != nullptr) {
		LoadGlobalData();
	}

	aRender.Render(*lButton);

}

MSSprite2DResource * Button::GetSelecttingImage()
{
	MSSprite2DResource* lSelectResource{ nullptr };

	switch (mState)
	{
	case ButtonState::Normal:
		lSelectResource = &mImageNormal;
		break;
	case ButtonState::Active:
		lSelectResource = &mImageActive;
		break;
	case ButtonState::Push:
		lSelectResource = &mImagePush;
		break;
	case ButtonState::Disable:
		lSelectResource = &mImageDisable;
		break;
	default:
		break;
	}
	return lSelectResource;
}
