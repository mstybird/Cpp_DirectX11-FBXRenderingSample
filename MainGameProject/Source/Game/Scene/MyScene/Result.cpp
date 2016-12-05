#include "Result.h"
#include<iomanip>
#include <sstream>
MySceneResult::MySceneResult()
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

	mButtonList.AddButton(&mButtonRetry);
	mButtonList.AddButton(&mButtonToTitle);
}

MySceneResult::~MySceneResult()
{
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
	sout << std::setw(2) << std::setfill('0') << std::right << aValue.mBlackScore << ":" << std::setw(2) << std::setfill('0') << std::left << aValue.mWhiteScore;
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
}

void MySceneResult::ButtonNext()
{
	mButtonList.ActiveNext();
}

void MySceneResult::ButtonPush()
{
	mButtonList.PushButton();
}

void MySceneResult::ButtonUp()
{
	mButtonList.PopButton();
}

int MySceneResult::GetButtonActiveIndex()
{
	return mButtonList.GetActiveIndex();
}
