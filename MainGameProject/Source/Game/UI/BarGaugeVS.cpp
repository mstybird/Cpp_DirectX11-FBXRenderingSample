#include "BarGaugeVS.h"
#include<DX11TextureManager.hpp>
#include"MSSprite2DRender.h"
BarGaugeVS::BarGaugeVS()
{
	mGaugeLeftImg.SetPivot({ 0.5f,0.5f });
	mGaugeRightImg.SetPivot({ 0.5f,0.5f });
}

void BarGaugeVS::SetTextures(std::pair<DX11TextureManager, int> aFrame, std::pair<DX11TextureManager, int> aLeftImage, std::pair<DX11TextureManager, int> aRightImage)
{
	mFrameImg.SetTexture(aFrame.first, aFrame.second);
	mGaugeLeftImg.SetTexture(aLeftImage.first, aLeftImage.second);
	mGaugeRightImg.SetTexture(aRightImage.first, aRightImage.second);
}

void BarGaugeVS::SetSize(const float & aWidth, const float & aHeight)
{
	mGaugeSize = { aWidth,aHeight };
}

void BarGaugeVS::SetScale(const float & aX, const float & aY)
{
	mGaugeScale.x = aX;
	mGaugeScale.y = aY;
}

void BarGaugeVS::SetOffset(const float & aOffsetLeft, const float & aOffsetTop)
{
	mGaugeOffset.x = aOffsetLeft;	//��
	mGaugeOffset.y = aOffsetTop;	//��
}

void BarGaugeVS::SetOffset(const DXVector2 & aOffset)
{
	mGaugeOffset = aOffset;
}

void BarGaugeVS::SetParam(MSProgress & aLeftParam, MSProgress & aRightParam)
{
	mStatusLeft = aLeftParam;
	mStatusRight = aRightParam;
}

void BarGaugeVS::Update()
{

}

void BarGaugeVS::Render(MSSprite2DRender & aRender, UIBase * aParent)
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
	mFrameImg.SetSize(mGaugeSize);
	mFrameImg.SetPosition(mGlobalPosition);
	mFrameImg.SetScale(mGlobalScale);

	//�Q�[�W���g�̃X�P�[��
	auto lGaugeScale = mGaugeScale*mGlobalScale;

	mGaugeLeftImg.SetSize(mGaugeSize);
	mGaugeLeftImg.SetPosition(mGaugePos);
	mGaugeLeftImg.SetScale(lGaugeScale);

	mGaugeRightImg.SetSize(mGaugeSize);
	mGaugeRightImg.SetPosition(mGaugePos);
	mGaugeRightImg.SetScale(lGaugeScale);

	//���E���v�X�R�A
	auto totalScore = mStatusLeft.GetNow() + mStatusRight.GetNow();
	float left{ 0 };
	float right{ 0 };

	if (totalScore != 0) {
		//���Q�[�W��
		left = mStatusRight.GetNow() / totalScore;
		right = mStatusLeft.GetNow() / totalScore;
	}

	//�E���獶��
	mGaugeLeftImg.SetSplitSizeX(
	{0,left}
	);
	mGaugeRightImg.SetSplitSizeX(
	{ 1.0f-right,1.0f }
	);

	/*
		��邱��
		�Q�[�W�����̃��\�[�X�ݒ肩��̕`�揈��
	*/

	aRender.Render(mGaugeLeftImg);
	aRender.Render(mGaugeRightImg);
	aRender.Render(mFrameImg);

	if (aParent != nullptr) {
		LoadGlobalData();
	}

}
