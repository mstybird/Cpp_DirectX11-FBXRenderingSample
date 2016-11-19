#include "StatusFrame.h"
#include"MSSprite2DRender.h"
#include"BarGauge.h"
#include"MSSprite2DResource.h"
StatusFrame::StatusFrame():
	mHpBar{ std::make_shared<BarGauge>() },
	mEpBar{std::make_shared<BarGauge>()}
{
}
BarGauge * StatusFrame::GetHPBar()
{
	return mHpBar.get();
}

BarGauge * StatusFrame::GetEPBar()
{
	return mEpBar.get();
}

void StatusFrame::Render(MSSprite2DRender& aRender, UIBase*aParent)
{


	
	//バッファに値を退避
	mGlobalPositionBuffer = mGlobalPosition;
	mGlobalScaleBuffer = mGlobalScale;

	mGlobalPosition += *aParent->GetGlobalPosition();
	mGlobalScale *= *aParent->GetGlobalScale();

	mFrame.SetPosition(mGlobalPosition);
	mFrame.SetScale(mGlobalScale);

	
	
	
	mHpBar->Render(aRender,this);
	mEpBar->Render(aRender,this);

	aRender.Render(mFrame);
	//バッファから値を復帰
	mGlobalPosition = mGlobalPositionBuffer;
	mGlobalScale = mGlobalScaleBuffer;


}

void StatusFrame::SetTexture(DX11TextureManager & aManager, const int & aTextureID)
{
	mFrame.SetTexture(aManager, aTextureID);
}

void StatusFrame::SetSize(const float & aWidth, const float & aHeight)
{
	mFrame.SetSize({ aWidth,aHeight });
}
