#include "AttackSlot.h"

AttackSlot::AttackSlot()
{

}

AttackSlot::~AttackSlot()
{
}

void AttackSlot::AddSlot(ButtonDesc aDesc)
{
	Button lItem;
	lItem.SetTextures(aDesc.mManager, aDesc.mNormalID, aDesc.mActiveID, aDesc.mPushID, aDesc.mDisableID);
	lItem.SetSize(aDesc.mSize.x, aDesc.mSize.y);
	mItemArray.push_back(std::move(lItem));

	mSlot.ClearButton();
	for (auto&lData : mItemArray) {
		mSlot.AddButton(&lData);
	}

}

void AttackSlot::Render(MSSprite2DRender & aRender, UIBase * aParent)
{
	if (aParent != nullptr) {
		StoreGlobalData();
		mGlobalPosition += *aParent->GetGlobalPosition();
		mGlobalScale *= *aParent->GetGlobalScale();
	}

	mSlot.Render(aRender, this);
	if (aParent != nullptr) {
		LoadGlobalData();
	}
}
