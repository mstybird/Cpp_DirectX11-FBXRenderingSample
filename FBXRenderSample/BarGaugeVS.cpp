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
	mGaugeOffset.x = aOffsetLeft;	//左
	mGaugeOffset.y = aOffsetTop;	//上
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
	//親UIがあれば現在の値を一時退避
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
		やること
		ゲージ部分のリソース設定からの描画処理
	*/

	if (aParent != nullptr) {
		LoadGlobalData();
	}

}
