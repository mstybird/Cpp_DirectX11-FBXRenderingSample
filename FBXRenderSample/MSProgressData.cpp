#include "MSProgressData.h"

MSProgress::MSProgress():
	mMax{},
	mNow{},
	mMin{}
{
}

void MSProgress::Set(float aMax, float aMin, float aNow)
{
	mMax = aMax;
	mMin = aMin;
	mNow = aNow;
	
}

bool MSProgress::SetNowFix(float aNowValue)
{
	if (IsBetweenValue(aNowValue)) {

	}
	else {

	}
	return false;
}

bool MSProgress::SetNowPer(float aNowValue)
{
	return false;
}

bool MSProgress::AddFix(float aValue)
{
	return false;
}

bool MSProgress::SubFix(float aValue)
{
	return false;
}

bool MSProgress::AddPer(float aValue)
{
	return false;
}

bool MSProgress::SubPer(float aValue)
{
	return false;
}

bool MSProgress::IsBetweenValue(float aValue)
{
	return mMax >= aValue&&mMin <= aValue;
}
