#include "SlotToggle.h"
#include"Button.h"
SlotToggle::SlotToggle()
{
	mRenderCount = -1;
	mIsChangeAnimation = false;
}
SlotToggle::~SlotToggle()
{
}
void SlotToggle::OnChangeIndex(const int Direction)
{
	mTargetPosition.clear();
	mTargetPosition.resize(mButtonArray.size());

	StoreGlobalData();

	if (mParent != nullptr) {
		mGlobalPosition += *mParent->GetGlobalPosition();
		mGlobalScale *= *mParent->GetGlobalScale();
	}

	int lRenderCount = mRenderCount == -1 ? mButtonArray.size() : mRenderCount;

	int startIndex = GetActiveIndex() - lRenderCount / 2;
	if (startIndex < 0)startIndex = mButtonArray.size() + startIndex;

	mGlobalPosition += mPadding*(lRenderCount / 2);

	int lButtonIndex = startIndex;
	for (int i = 0; i < lRenderCount; ++i) {
		auto lButton = mButtonArray[lButtonIndex];

		mTargetPosition[lButtonIndex] = mGlobalPosition;


		auto ButtonSize = lButton->GetScalingSize(this);
		mGlobalPosition -= mPadding;
		mGlobalPosition.x -= ButtonSize.x;

		++lButtonIndex;
		lButtonIndex %= mButtonArray.size();

	}

	mIsChangeAnimation = true;


	LoadGlobalData();


}
void SlotToggle::Render(MSSprite2DRender & aRender, UIBase * aParent)
{
	
	StoreGlobalData();
	if (aParent != nullptr) {
		if (mParent != aParent) {
			mParent = aParent;
		}
		mGlobalPosition += *mParent->GetGlobalPosition();
		mGlobalScale *= *mParent->GetGlobalScale();
	}
	int lRenderCount = mRenderCount == -1 ? mButtonArray.size() : mRenderCount;

	int startIndex = GetActiveIndex() - lRenderCount / 2;
	if (startIndex < 0)startIndex = mButtonArray.size() + startIndex;
	const int lCenterIndex = lRenderCount / 2;
	mGlobalPosition += mPadding*(lRenderCount / 2);

	int lButtonIndex = startIndex;
	for (int i = 0; i < lRenderCount; ++i) {
		auto lButton = mButtonArray[lButtonIndex];
		if (mIsChangeAnimation) {
			DXVector2 lUpdatePosition;
			const DXVector2* lNowPosition = lButton->GetGlobalPosition();
			D3DXVec2Lerp(&lUpdatePosition, lNowPosition, &mTargetPosition[lButtonIndex], 0.1f);


			DXVector2 lDiffPosition = lUpdatePosition - *lNowPosition;
			float lMinMove = 2.0f;

			if (lDiffPosition.x > 0 && lDiffPosition.x < lMinMove) {
				if (mTargetPosition[lButtonIndex].GetDistance(lUpdatePosition) > lMinMove) {
					lUpdatePosition.x += lMinMove - lDiffPosition.x;
				}
				else {
					lUpdatePosition.x = mTargetPosition[lButtonIndex].x;
				}
			}
			if (lDiffPosition.x < 0 && lDiffPosition.x > -lMinMove) {
				if (mTargetPosition[lButtonIndex].GetDistance(lUpdatePosition) > lMinMove) {
					lUpdatePosition.x -= lMinMove + lDiffPosition.x;
				}
				else {
					lUpdatePosition.x = mTargetPosition[lButtonIndex].x;
				}

			}
			if (lDiffPosition.y > 0 && lDiffPosition.y < lMinMove) {
				if (mTargetPosition[lButtonIndex].GetDistance(lUpdatePosition) > lMinMove) {
					lUpdatePosition.y += lMinMove - lDiffPosition.y;
				}
				else {
					lUpdatePosition.y = mTargetPosition[lButtonIndex].y;
				}
			}
			if (lDiffPosition.y < 0 && lDiffPosition.y > -lMinMove) {
				if (mTargetPosition[lButtonIndex].GetDistance(lUpdatePosition) > lMinMove) {
					lUpdatePosition.y -= lMinMove + lDiffPosition.y;
				}
				else {
					lUpdatePosition.y = mTargetPosition[lButtonIndex].y;
				}
			}

			lButton->SetGlobalPosition(lUpdatePosition);
		}
		else {
			lButton->SetGlobalPosition(mGlobalPosition);

		}

		//中央のスロット以外は縮小
		if (i == lCenterIndex) {		
			lButton->SetGlobalScale(mGlobalScale);
		}
		else {
			lButton->SetGlobalScale(mGlobalScale);
		}

		lButton->Render(aRender);

		auto ButtonSize = lButton->GetScalingSize(this);
		mGlobalPosition -= mPadding;
		mGlobalPosition.x -= ButtonSize.x;

		++lButtonIndex;
		lButtonIndex %= mButtonArray.size();

	}



	LoadGlobalData();
	if (aParent != nullptr) {
	}
}
