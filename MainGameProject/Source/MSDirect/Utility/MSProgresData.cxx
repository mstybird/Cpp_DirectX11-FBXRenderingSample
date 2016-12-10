#include "MSProgressData.hxx"
#include<cassert>
MSProgress::MSProgress() :
	mMax{0.0f},
	mNow{0.0f},
	mMin{0.0f}
{
}

float MSProgress::GetMax()
{
	return mMax;
}

float MSProgress::GetNow()
{
	return mNow;
}

float MSProgress::GetMin()
{
	return mMin;
}

float MSProgress::GetNowPer()
{
	float lNowDiff, lFixDiff;
	lFixDiff = mMax - mMin;
	lNowDiff = mNow - mMin;


	return lNowDiff / lFixDiff;
}

float MSProgress::GetMaxRemainPer()
{
	//現在値の割合の取得
	float lPer = GetNowPer();
	//現在地の割合が1.0f以下であれば、割合を取得
	float lRemainPer = 0.0f;
	if (lPer < 1.0f) {
		lRemainPer = 1.0f - lPer;
	}


	return lRemainPer;
	
}

float MSProgress::GetMaxRemainFix()
{
	//最大値から現在地を引いた値が最大値にするための必要な値

	return mMax - mNow;
}

void MSProgress::Set(float aMax, float aMin, float aNow)
{
	mMax = aMax;
	mMin = aMin;
	mNow = aNow;
	//現在地の修正
	FitValue();
}

void MSProgress::SetNowFix(float aNowValue)
{
	float lNow;
	lNow = aNowValue > mMax ? mMax : aNowValue;
	lNow = aNowValue > mMax ? mMax : lNow;
	mNow = lNow;
	return;
}

void MSProgress::SetNowPer(float aNowValue)
{
	float lNowPer;
	lNowPer = aNowValue <= 0.0f ? 0.0f : aNowValue;
	lNowPer = aNowValue >= 1.0f ? 1.0f : lNowPer;

	float lFixDiff;
	lFixDiff = mMax - mMin;
	lFixDiff *= lNowPer;
	mNow = lFixDiff;

	return;
}

MSPRogressFlag MSProgress::AddFix(float aValue)
{
	mNow += aValue;
	MSPRogressFlag lResult{ IsBetweenValue(mNow) };
	FitValue();
	return lResult;
}

MSPRogressFlag MSProgress::SubFix(float aValue)
{
	mNow -= aValue;
	MSPRogressFlag lResult{ IsBetweenValue(mNow) };
	FitValue();
	return lResult;
}

MSPRogressFlag MSProgress::AddPer(float aValue)
{
	float lFixDiff;
	lFixDiff = mMax - mMin;
	mNow += lFixDiff*aValue;
	MSPRogressFlag lResult{ IsBetweenValue(mNow) };
	FitValue();
	return lResult;
}

MSPRogressFlag MSProgress::SubPer(float aValue)
{
	float lFixDiff;
	lFixDiff = mMax - mMin;
	mNow -= lFixDiff*aValue;
	MSPRogressFlag lResult{ IsBetweenValue(mNow) };
	FitValue();
	return lResult;
}

MSPRogressFlag MSProgress::IsBetweenValue(float aValue)
{
	MSPRogressFlag lResult{ MSPRogressFlag::NOTOVERFLOW };
	if (aValue > mMax) {
		lResult = MSPRogressFlag::GREATEMAX;
	}
	if (aValue <= mMin) {
		lResult = MSPRogressFlag::LESSMIN;
	}
	return lResult;
}

void MSProgress::FitValue()
{
	switch (IsBetweenValue(mNow))
	{
	case MSPRogressFlag::GREATEMAX:
		mNow = mMax;
		break;
	case MSPRogressFlag::LESSMIN:
		mNow = mMin;
		break;
	case MSPRogressFlag::NOTOVERFLOW:
		//処理しない
		break;
	default:
		//処理しない
		break;
	}
	return;
}
