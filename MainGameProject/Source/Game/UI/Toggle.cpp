#include "Toggle.h"
#include"Button.h"
Toggle::Toggle():
	mActiveIndex{-1},
	mPivot{0,0}
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

void Toggle::SetActiveIndex(const int aID)
{
	if (0 <= aID && aID < mButtonArray.size()) {
		GetActiveButton()->SetNormal();
		mActiveIndex = aID;
		GetActiveButton()->SetActive();
	}
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
	GetActiveButton()->SetActive();
	return;
}

int Toggle::CollisionPoint(int aX, int aY)
{
	StoreGlobalData();

	DXVector2 lPosition = mGlobalPosition;
	DXVector2 lScale = mGlobalScale;
	DXVector2 lSize;
	if (mLastParent != nullptr) {

		lPosition += *mLastParent->GetGlobalPosition();
		lScale *= *mLastParent->GetGlobalScale();
	}

	int lHitIndex = -1;
	for (int i = 0; i < mButtonArray.size();++i) {
		lSize = mButtonArray[i]->GetSizeGlobal()*lScale;
		if (
			lPosition.x < aX &&
			aX < lPosition.x + lSize.x &&
			lPosition.y < aY &&
			aY < lPosition.y + lSize.y
			) {
			//衝突した場合
			lHitIndex = i;
		}

		auto ButtonSize = mButtonArray[i]->GetScalingSize(this);
		lPosition += mPadding;
		lPosition.y += ButtonSize.y;
	}

	LoadGlobalData();

	return lHitIndex;
}

void Toggle::SetPivot(int aX, int aY)
{
	mPivot.x = aX;
	mPivot.y = aY;
	//for (auto lButton : mButtonArray) {
	//	lButton->
	//}
}

void Toggle::Render(MSSprite2DRender & aRender, UIBase * aParent)
{
	StoreGlobalData();
	mLastParent = aParent;
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
