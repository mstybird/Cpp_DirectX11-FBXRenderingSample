#include "UIBase.h"

UIBase::UIBase():
	mGlobalScale{1.0f,1.0f}
{
}

void UIBase::SetGlobalPosition(const float & aOffsetX, const float & aOffsetY)
{
	mGlobalPosition.Set(aOffsetX, aOffsetY);
}

void UIBase::SetGlobalPosition(const DXVector2 & aOffset)
{
	mGlobalPosition = aOffset;
}

void UIBase::SetGlobalScale(const float & aOffsetX, const float & aOffsetY)
{
	mGlobalScale.Set(aOffsetX, aOffsetY);
}

void UIBase::SetGlobalScale(const DXVector2 & aOffset)
{
	mGlobalScale = aOffset;
}

DXVector2 * UIBase::GetGlobalPosition()
{
	return &mGlobalPosition;
}

DXVector2 * UIBase::GetGlobalScale()
{
	return &mGlobalScale;
}

void UIBase::StoreGlobalData()
{
	mGlobalPositionBuffer = mGlobalPosition;
	mGlobalScaleBuffer = mGlobalScale;
}

void UIBase::LoadGlobalData()
{
	mGlobalPosition = mGlobalPositionBuffer;
	mGlobalScale = mGlobalScaleBuffer;
}
