#include "Result.h"
#include<NcgLua.hpp>
#include<iomanip>
#include <sstream>
MySceneResult::MySceneResult()
{
	
	//フォントの準備
	FontLog	logFont;
	::ZeroMemory(&logFont, sizeof(logFont));
	//	logFont.lfHeight = 40;	//フォントサイズ
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
	//wcscpy_sの第二引数は文字(配列)数(バッファサイズだとデバッガが終了しなかったりする)
	wcscpy_s(logFont.lfFaceName, LF_FACESIZE, _T("ＭＳ 明朝"));
#else
	strcpy_s(logFont.lfFaceName, LF_FACESIZE, ("ＭＳ 明朝"));
#endif	
	mTextMan.RegisterFont(logFont, 0);
	mTextMan.SetDefaultSize(640, 960);
	mTextMan.SetFont(0);

	mButtonList.AddButton(&mButtonRetry);
	mButtonList.AddButton(&mButtonToTitle);
}

MySceneResult::~MySceneResult()
{
}

void MySceneResult::InitializeSound(NcgLuaDatabase& aDb)
{
	using namespace ValueResult::Sound;
	aDb.Load(cLuaPath, cLuaID);
	std::string lFileName;
	auto lManager = aDb.GetManager(cLuaID);
	lManager->GetGlobal(cBGMPath, lFileName);
	mBGM = mSoundDevice.CreateSoundFromFile(lFileName);

	lManager->GetGlobal(cSelectPath, lFileName);
	mSESelect = mSoundDevice.CreateSoundFromFile(lFileName);

	lManager->GetGlobal(cEnterPath, lFileName);
	mSEEnter = mSoundDevice.CreateSoundFromFile(lFileName);

	lManager->GetGlobal(cTimeUpPath, lFileName);
	mSETimeUp = mSoundDevice.CreateSoundFromFile(lFileName);


	mBGM.SetLoop(true);

}

void MySceneResult::SetFrameTexture(DX11TextureManager * aManager, const int aFrameID)
{
	mFrame.SetTexture(*aManager, aFrameID);
}

void MySceneResult::SetLogoTexture(DX11TextureManager * aManager, const int aWinID, const int aLoseID, const int aDrawID)
{
	mLogoWin.SetTexture(*aManager, aWinID);
	mLogoLose.SetTexture(*aManager, aLoseID);
	mLogoDraw.SetTexture(*aManager, aDrawID);
}

void MySceneResult::SetButtonRetryTexture(DX11TextureManager * aManager, const int aNormalID, const int aActiveID, const int aPushID, const int aDisableID)
{
	mButtonRetry.SetTextures(aManager, aNormalID, aActiveID, aPushID, aDisableID);
}

void MySceneResult::SetButtonToTitleTexture(DX11TextureManager * aManager, const int aNormalID, const int aActiveID, const int aPushID, const int aDisableID)
{
	mButtonToTitle.SetTextures(aManager, aNormalID, aActiveID, aPushID, aDisableID);
}

void MySceneResult::SetFramePosition(const float aX, const float aY)
{
	mFrame.SetPosition({aX, aY});
}

void MySceneResult::SetLogoPosition(const float aX, const float aY)
{
	mLogoPos.Set(aX, aY);
}

void MySceneResult::SetButtonPosition(const float aX, const float aY)
{
	mButtonList.SetGlobalPosition(aX, aY);
}

void MySceneResult::SetTextPosition(const float aX, const float aY)
{
	mTextPos.Set(aX, aY);
}

void MySceneResult::SetFrameSize(const float aX, const float aY)
{
	mFrame.SetSize({ aX, aY });
}

void MySceneResult::SetLogoSize(const float aX, const float aY)
{
	mLogoSize.Set(aX, aY);
}

void MySceneResult::SetButtonSize(const float aX, const float aY)
{
	mButtonRetry.SetSize(aX, aY);
	mButtonToTitle.SetSize(aX, aY);
}

void MySceneResult::SetButtonPadding(const float aX, const float aY)
{
	mButtonList.SetPadding(aX, aY);
}

void MySceneResult::SetValues(const ResultValue & aValue)
{
	std::ostringstream sout;
	sout << "Score:" << std::setw(2) << std::setfill('0') << aValue.mBlackScore << ":" << std::setw(2) << std::setfill('0') << aValue.mWhiteScore;
	mTextScore = mTextMan.Create(sout.str());
	mTextScore->SetPosition(mTextPos);
	switch (aValue.mIssue)
	{
	case IssueFlag::Win:
		mActiveLogo = &mLogoWin;
		break;
	case IssueFlag::Lose:
		mActiveLogo = &mLogoLose;
		break;
	case IssueFlag::Draw:
		mActiveLogo = &mLogoDraw;
		break;
	default:
		break;
	}
	mActiveLogo->SetPosition(mLogoPos);
	mActiveLogo->SetSize(mLogoSize);
}

void MySceneResult::PlaySETimeUp()
{
	mSETimeUp.Play();
}

void MySceneResult::PlayBGM()
{
	mBGM.Play();
}



void MySceneResult::Render(MSSprite2DRender & aRender)
{
	aRender.Render(mFrame);
	aRender.Render(*mActiveLogo);
	mButtonList.Render(aRender);
	if (mTextScore) {
		mTextScore->Render(aRender);
	}


}

void MySceneResult::ButtonBack()
{
	mButtonList.ActiveBack();
	mSESelect.Play();
}

void MySceneResult::ButtonNext()
{
	mButtonList.ActiveNext();
	mSESelect.Play();
}

void MySceneResult::ButtonPush()
{
	mButtonList.PushButton();
	mSEEnter.Play();
}

void MySceneResult::ButtonUp()
{
	mButtonList.PopButton();
}

int MySceneResult::GetButtonActiveIndex()
{
	return mButtonList.GetActiveIndex();
}
