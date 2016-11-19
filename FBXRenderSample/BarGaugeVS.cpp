#include "BarGaugeVS.h"
#include<DX11TextureManager.hpp>
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

	mFrameImg.SetSize(mGaugeSize);
	mFrameImg.SetPosition(mGlobalPosition);
	mFrameImg.SetScale(mGlobalScale);

	/*
		��邱��
		�Q�[�W�����̃��\�[�X�ݒ肩��̕`�揈��
	*/

	if (aParent != nullptr) {
		LoadGlobalData();
	}

}
