#include"ScoreView.h"
#include"BarGaugeVS.h"
#include<iomanip>
#include <sstream>
ScoreView::ScoreView():
	mScoreBar{std::make_shared<BarGaugeVS>()}
{
	//�t�H���g�̏���
	FontLog	logFont;
	::ZeroMemory(&logFont, sizeof(logFont));
	//	logFont.lfHeight = 40;	//�t�H���g�T�C�Y
	logFont.SetFontSize(40);
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
	//wcscpy_s�̑������͕���(�z��)��(�o�b�t�@�T�C�Y���ƃf�o�b�K���I�����Ȃ������肷��)
	wcscpy_s(logFont.lfFaceName, LF_FACESIZE, _T("�l�r ����"));
#else
	strcpy_s(logFont.lfFaceName, LF_FACESIZE, ("�l�r ����"));
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

void ScoreView::UpdateScore(float aLeft, float aRight)
{
	mScoreBar->SetParam(aLeft, aRight);
	std::ostringstream sout;
	sout << std::setw(4) << std::right << (int)(aLeft+0.5f) << " / " << std::setw(4) << std::left << (int)(aRight + 0.5f);
	mScoreText = mTextMan.Create(sout.str());
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
