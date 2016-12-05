#pragma once
#include"TimeView.h"
#include<iomanip>
#include <sstream>
TimeView::TimeView()
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

TimeView::~TimeView()
{

}

void TimeView::UpdateTime(const int mMinute, const int mSecond)
{
	std::ostringstream sout;
	sout << std::setw(4) << std::right << mMinute << " : " << std::setw(4) << std::left << mSecond;
	mTimeText = mTextMan.Create(sout.str());
}

void TimeView::Render(MSSprite2DRender & aRender, UIBase * aParent)
{
	if (aParent != nullptr) {
		StoreGlobalData();

		mGlobalPosition += *aParent->GetGlobalPosition();
		mGlobalScale *= *aParent->GetGlobalScale();
	}


	mTimeText->SetPosition(mGlobalPosition);
	mTimeText->Render(aRender);

	if (aParent != nullptr) {
		LoadGlobalData();
	}
}
