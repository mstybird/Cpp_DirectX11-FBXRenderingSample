#include "BarGauge.h"
#include<DX11TextureManager.hpp>
#include"MSSprite2DRender.h"
BarGauge::BarGauge()
{
	mInImage.SetPivot({ 0.5f,0.5f });
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

void BarGauge::SetParam(MSProgress & aParam)
{
	mResource = aParam;
}

void BarGauge::Update()
{
	//�O�g�̃T�C�Y�ƈʒu��ݒ�
	mOutImage.SetSize(mGaugeSize);
	mOutImage.SetPosition(mGlobalPosition);
	
	//���g�̃T�C�Y�Ɗg�嗦�A�I�t�Z�b�g��ݒ�
	auto mInPos = mGlobalPosition;
	mInPos += (mGaugeSize / 2);
	mInImage.SetSize(mGaugeSize);
	mInImage.SetPosition(mInPos);

	mInImage.SetScale(mGaugeScale);

	//�E���獶�Ɍ���Ɖ���
	mInImage.SetSplitSizeX(
	{ 0, mResource.GetNowPer() }
	);
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
	{ 0, mResource.GetNowPer() }
	);
	if (aParent != nullptr) {
		LoadGlobalData();
	}


	aRender.Render(mOutImage);
	aRender.Render(mInImage);
}
