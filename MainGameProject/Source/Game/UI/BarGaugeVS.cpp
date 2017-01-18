#include "BarGaugeVS.h"
#include<DX11TextureManager.hpp>
#include"MSSprite2DRender.h"
BarGaugeVS::BarGaugeVS()
{
	mGaugeLeftImg.SetPivot({ 0.5f,0.5f });
	mGaugeScale.Set(1.0f);
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

void BarGaugeVS::SetParam(const float & aLeftParam, const float & aRightParam, bool aIsAnimation)
{
	mStatusLeft = aLeftParam;
	mStatusRight = aRightParam;
	if (!aIsAnimation) {
		mScreenScoreLeft = aLeftParam;
		mScreenScoreRight = aRightParam;
	}
}

void BarGaugeVS::Update()
{
	DXVector2 lNow, lTarget;
	DXVector2 lResult;
	lNow = { mScreenScoreLeft,mScreenScoreRight };
	lTarget = { mStatusLeft,mStatusRight };
	D3DXVec2Lerp(&lResult, &lNow, &lTarget, 0.1f);
	mScreenScoreLeft = lResult.x;
	mScreenScoreRight = lResult.y;
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
	//ゲージフレームの設定
	mFrameImg.SetSize(mGaugeSize);
	mFrameImg.SetPosition(mGlobalPosition);
	mFrameImg.SetScale(mGlobalScale);

	//ゲージ中身のスケール
	auto lGaugeScale = mGaugeScale*mGlobalScale;

	mGaugeLeftImg.SetSize(mGaugeSize);
	mGaugeLeftImg.SetPosition(mGaugePos);
	mGaugeLeftImg.SetScale(lGaugeScale);

	mGaugeRightImg.SetSize(mGaugeSize);
	mGaugeRightImg.SetPosition(mGaugePos);
	mGaugeRightImg.SetScale(lGaugeScale);

	//左右合計スコア
	auto totalScore = mScreenScoreLeft + mScreenScoreRight;
	float left{ 0 };
	float right{ 0 };

	if (totalScore != 0) {
		//左ゲージ幅
		left = mScreenScoreLeft / totalScore;
		right = mScreenScoreRight / totalScore;
	}

	//右から左へ
	mGaugeLeftImg.SetSplitSizeX(
	{0,left}
	);
	mGaugeRightImg.SetSplitSizeX(
	{ 1.0f-right,1.0f }
	);

	/*
		やること
		ゲージ部分のリソース設定からの描画処理
	*/

	aRender.Render(mFrameImg);
	aRender.Render(mGaugeLeftImg);
	aRender.Render(mGaugeRightImg);

	if (aParent != nullptr) {
		LoadGlobalData();
	}

}
