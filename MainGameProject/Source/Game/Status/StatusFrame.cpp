#include "StatusFrame.h"
#include"MSSprite2DRender.h"
#include"BarGauge.h"
#include"MSSprite2DResource.h"
#include"StatusBase.h"
#include<MSDirect.h>
#include<iomanip>
#include <sstream>
StatusFrame::StatusFrame():
	mHpBar{ std::make_shared<BarGauge>() },
	mEpBar{std::make_shared<BarGauge>()}
{
	//フォントの準備
	FontLog	logFont;
	::ZeroMemory(&logFont, sizeof(logFont));
	//	logFont.lfHeight = 40;	//フォントサイズ
	logFont.SetFontSize(18);
	logFont.lfWidth = 0;
	logFont.lfEscapement = 0;
	logFont.lfOrientation = 0;
	logFont.lfWeight = FW_EXTRABOLD;
	logFont.lfItalic = 0;
	logFont.lfUnderline = 0;
	logFont.lfStrikeOut = 0;
	logFont.lfCharSet = SHIFTJIS_CHARSET;
	logFont.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logFont.lfQuality = PROOF_QUALITY;
	logFont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;

#ifdef UNICODE
	//wcscpy_sの第二引数は文字(配列)数(バッファサイズだとデバッガが終了しなかったりする)
	wcscpy_s(logFont.lfFaceName, LF_FACESIZE, _T("ＭＳ 明朝"));
#else
	strcpy_s(logFont.lfFaceName, LF_FACESIZE, ("ＭＳ 明朝"));
#endif	
	mTextMan.RegisterFont(logFont, 0);
	mTextMan.SetDefaultSize(640, 960);
	mTextMan.SetFont(0);
}
StatusFrame::~StatusFrame()
{


	int a = 0;
	mHpBar.reset();
	printf("Hello\n");
	//mHpText.reset();
	//mEpBar.reset();
	//mEpText.reset();
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

	
	
	
	aRender.Render(mFrame);
	mHpBar->Render(aRender,this);
	mEpBar->Render(aRender,this);
	mHpText->Render(aRender);
	mEpText->Render(aRender);


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

void StatusFrame::SetHPTextOffset(const float aWidth, const float aHeight)
{
	mHpTextPos.Set(aWidth, aHeight);
}

void StatusFrame::SetEPTextOffset(const float aWidth, const float aHeight)
{
	mEpTextPos.Set(aWidth, aHeight);
}

void StatusFrame::UpdateStatus(StatusBase * aStatus)
{
	mHpBar->SetParam(aStatus->mHp);
	mEpBar->SetParam(aStatus->mEnergy);
	std::ostringstream sout;
	sout << "HP:" << std::setfill('0') << std::setw(2) << (int)aStatus->mHp.GetNow() << "/" << std::setfill('0') << std::setw(2) << (int)aStatus->mHp.GetMax();
	mHpText = mTextMan.Create(sout.str());
	std::ostringstream sout2;

	sout2 << "EP:" << std::setfill('0') << std::setw(2) << (int)aStatus->mEnergy.GetNow() << "/" << std::setw(2) << std::setfill('0') << (int)aStatus->mEnergy.GetMax();
	mEpText = mTextMan.Create(sout2.str());

	//HpBarPosition
	auto lHPBarScale = (mGlobalScale * *mHpBar->GetGlobalScale());
	auto lEPBarScale = (mGlobalScale * *mEpBar->GetGlobalScale());
	auto lHPTextPos = (mGlobalPosition + *mHpBar->GetGlobalPosition())*lHPBarScale + mHpTextPos;
	auto lEPTextPos = (mGlobalPosition + *mEpBar->GetGlobalPosition())*lEPBarScale + mEpTextPos;
	mHpText->SetPosition(lHPTextPos);
	mEpText->SetPosition(lEPTextPos);

}
