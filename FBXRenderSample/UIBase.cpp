#include "UIBase.h"

void UIBase::SetGlobalPosition(const float & aOffsetX, const float & aOffsetY)
{
	mGlobalPosition.Set(aOffsetX, aOffsetY);
}

void UIBase::SetGlobalPosition(const DXVector2 & aOffset)
{
	mGlobalPosition = aOffset;
}
