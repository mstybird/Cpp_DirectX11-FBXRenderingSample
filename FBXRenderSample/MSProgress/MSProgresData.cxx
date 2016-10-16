#include "MSProgressData.hxx"
#include<cassert>
MSProgress::MSProgress() :
	mMax{},
	mNow{},
	mMin{}
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
	//���ݒl�̊����̎擾
	float lPer = GetNowPer();
	//���ݒn�̊�����1.0f�ȉ��ł���΁A�������擾
	float lRemainPer = 0.0f;
	if (lPer < 1.0f) {
		lRemainPer = 1.0f - lPer;
	}


	return lRemainPer;
	
}

float MSProgress::GetMaxRemainFix()
{
	//�ő�l���猻�ݒn���������l���ő�l�ɂ��邽�߂̕K�v�Ȓl

	return mMax - mNow;
}

void MSProgress::Set(float aMax, float aMin, float aNow)
{
	mMax = aMax;
	mMin = aMin;
	mNow = aNow;
	//���ݒn�̏C��
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
	if (aValue < mMin) {
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
		//�������Ȃ�
		break;
	default:
		//�������Ȃ�
		break;
	}
	return;
}
