#include "Toggle.h"
#include"Button.h"
Toggle::Toggle():
	mActiveIndex{-1}
{
}

Toggle::~Toggle()
{
}

void Toggle::AddButton(Button * aButton)
{
	mButtonArray.push_back(aButton);
	//初めて追加したボタンの場合、そのボタンはアクティブにする
	if (mButtonArray.size() == 1) {
		mButtonArray.front()->SetActive();
		mActiveIndex = 0;
	}
}

void Toggle::ClearButton()
{
	mButtonArray.clear();
}

bool Toggle::SetActive(const int aIndex)
{
	if (mButtonArray.size() <= aIndex)return false;
	if (aIndex < 0)return false;

	GetActiveButton()->SetNormal();
	mButtonArray[aIndex]->SetActive();
	mActiveIndex = aIndex;
	return true;
}

void Toggle::SetPadding(DXVector2 aPadding)
{
	mPadding = aPadding;
}

void Toggle::SetPadding(float aX, float aY)
{
	mPadding.Set(aX, aY);
}

int Toggle::GetCount()
{
	return mButtonArray.size();
}

int Toggle::GetActiveIndex()
{
	return mActiveIndex;
}

Button * Toggle::GetActiveButton()
{
	return mButtonArray[mActiveIndex];
}

int Toggle::ActiveNext()
{
	GetActiveButton()->SetNormal();
	++mActiveIndex;
	mActiveIndex %= mButtonArray.size();
	GetActiveButton()->SetActive();
	OnChangeIndex(1);
	return mActiveIndex;
}

int Toggle::ActiveBack()
{
	GetActiveButton()->SetNormal();
	--mActiveIndex;
	if (mActiveIndex < 0) {
		mActiveIndex = mButtonArray.size() - 1;
	}
	GetActiveButton()->SetActive();
	OnChangeIndex(-1);
	return mActiveIndex;
}

void Toggle::PushButton()
{
	GetActiveButton()->SetPushing();
	return;
}

void Toggle::PopButton()
{
	GetActiveButton()->SetActive();;
	return;
}

void Toggle::Render(MSSprite2DRender & aRender, UIBase * aParent)
{
		StoreGlobalData();
	if (aParent != nullptr) {

		mGlobalPosition += *aParent->GetGlobalPosition();
		mGlobalScale *= *aParent->GetGlobalScale();
	}

	for (auto&lButton : mButtonArray) {

		lButton->SetGlobalPosition(mGlobalPosition);
		lButton->SetGlobalScale(mGlobalScale);

		lButton->Render(aRender);

		auto ButtonSize = lButton->GetScalingSize(this);
		mGlobalPosition += mPadding;
		mGlobalPosition.y += ButtonSize.y;
	}

		LoadGlobalData();
	if (aParent != nullptr) {
	}
}
