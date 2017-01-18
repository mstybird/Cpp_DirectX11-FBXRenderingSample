#include "BarGauge.h"
#include<DX11TextureManager.hpp>
#include"MSSprite2DRender.h"
BarGauge::BarGauge()
{
	mInImage.SetPivot({ 0.5f,0.5f });
	
}
BarGauge::~BarGauge()
{
	printf("GG");
}
void BarGauge::SetTextures(std::pair<DX11TextureManager, int> aOutImage, std::pair<DX11TextureManager, int> aInImage)
{
	mOutImage.SetTexture(aOutImage.first, aOutImage.second);
	mInImage.SetTexture(aInImage.first, aInImage.second);
}

void BarGauge::SetSize(const float & aWidth, const float & aHeight)
{
	mGaugeSize = { aWidth,aHeight };
}

void BarGauge::SetGaugeScale(const float & aX, const float & aY)
{
	mGaugeScale.x = aX;
	mGaugeScale.y = aY;
}

void BarGauge::SetOffset(const float&aOffsetLeft, const float&aOffsetTop)
{
	mGaugeOffset.x = aOffsetLeft;	//��
	mGaugeOffset.y = aOffsetTop;	//��
}

void BarGauge::SetOffset(const DXVector2&aOffset)
{
	mGaugeOffset = aOffset;
}

void BarGauge::SetParam(MSProgress & aParam, bool aIsAnimation)
{
	auto v = mScreenResource.GetNowPer();
	if (isnan(v)) {
		mResource = aParam;
		mScreenResource = aParam;
	}
	else {
		mResource = aParam;
		if (!aIsAnimation) {
			mScreenResource = aParam;
		}
	}

}

void BarGauge::Update()
{

	DXVector2 lNow = { mScreenResource.GetNowPer() ,0 };
	DXVector2 lTarget = { mResource.GetNowPer() ,0 };
	DXVector2 lResult;
	D3DXVec2Lerp(&lResult, &lNow, &lTarget, 0.1f);

	mScreenResource.SetNowPer(lResult.x);
}

void BarGauge::Render(MSSprite2DRender& aRender, UIBase*aParent)
{
	//�eUI������Ό��݂̒l���ꎞ�ޔ�
	DXVector2 mGaugePos;
	if (aParent != nullptr) {
		StoreGlobalData();

		mGlobalPosition += *aParent->GetGlobalPosition();
		mGlobalScale *= *aParent->GetGlobalScale();
		mGaugePos = mGlobalPosition + (mGaugeSize * *aParent->GetGlobalScale() / 2);
	}
	//�Q�[�W�t���[���̐ݒ�
	mOutImage.SetSize(mGaugeSize);
	mOutImage.SetPosition(mGlobalPosition);
	mOutImage.SetScale(mGlobalScale);
	//�Q�[�W���g�̃X�P�[��
	auto lGaugeScale = mGaugeScale*mGlobalScale;
	mInImage.SetSize(mGaugeSize);
	mInImage.SetPosition(mGaugePos);
	mInImage.SetScale(lGaugeScale);
	//�E���獶�Ɍ���Ɖ���
	mInImage.SetSplitSizeX(
	{ 0, mScreenResource.GetNowPer() }
	);
	if (aParent != nullptr) {
		LoadGlobalData();
	}


	aRender.Render(mOutImage);
	aRender.Render(mInImage);
}
