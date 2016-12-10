#include"ScoreView.h"
#include"BarGaugeVS.h"
#include<iomanip>
#include <sstream>
ScoreView::ScoreView():
	mScoreBar{std::make_shared<BarGaugeVS>()}
{
	//フォントの準備
	FontLog	logFont;
	::ZeroMemory(&logFont, sizeof(logFont));
	//	logFont.lfHeight = 40;	//フォントサイズ
	logFont.SetFontSize(20);
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

ScoreView::~ScoreView()
{
}

void ScoreView::SetSize(const DXVector2 & aSize)
{
	mScoreBar->SetSize(aSize.x, aSize.y);
}

void ScoreView::SetSize(float aWidth, float aHeight)
{
	mScoreBar->SetSize(aWidth, aHeight);
}

void ScoreView::SetTextOffset(const DXVector2 & aOffset)
{
	mTextOffset = aOffset;
}

void ScoreView::SetTextOffset(float aWidth, float aHeight)
{
	mTextOffset.Set(aWidth, aHeight);
}

void ScoreView::SetGaugeScale(float aX, float aY)
{
	mScoreBar->SetScale(aX, aY);
//	mScoreBar->SetGlobalScale(aX, aY);
}

void ScoreView::SetTextures(DX11TextureManager * aTexManager, const int aFrameID, const int aLeftID, const int aRightID)
{
	mScoreBar->SetTextures(
	{ *aTexManager,aFrameID },
	{ *aTexManager,aLeftID },
	{ *aTexManager,aRightID }
	);
}

void ScoreView::UpdateScore(float aLeft, float aRight)
{
	std::ostringstream sout;
	sout << std::setw(4) << std::right << (int)(aLeft+0.5f) << " / " << std::setw(4) << std::left << (int)(aRight + 0.5f);
	mScoreText = mTextMan.Create(sout.str());

	if (aLeft == 0 && aRight == 0) {
		aLeft = 1;
		aRight = 1;
	}
	mScoreBar->SetParam(aLeft, aRight);


}

void ScoreView::Render(MSSprite2DRender & aRender, UIBase * aParent)
{
	if (aParent != nullptr) {
		StoreGlobalData();

		mGlobalPosition += *aParent->GetGlobalPosition();
		mGlobalScale *= *aParent->GetGlobalScale();
	}

	
	mScoreBar->Render(aRender, this);
	mScoreText->SetPosition(mGlobalPosition+mTextOffset);
	mScoreText->Render(aRender);

	if (aParent != nullptr) {
		LoadGlobalData();
	}
}
