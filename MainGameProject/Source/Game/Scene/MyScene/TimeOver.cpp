#include "TimeOver.h"
#include<DX11TextureManager.hpp>
#include"MSSprite2DRender.h"
MySceneTimeOver::MySceneTimeOver()
{
	mFrameCounter = 0;
	mLogoFinish.SetPivot({ 0.5f,0.5f });
	mLogoWin.SetPivot({ 0.5f,0.5f });
	mLogoLose.SetPivot({ 0.5f,0.5f });
}

MySceneTimeOver::~MySceneTimeOver()
{
}

void MySceneTimeOver::SetTextures(DX11TextureManager * aManager, const int aFinishID, const int aWinID, const int aLoseID)
{
	mLogoFinish.SetTexture(*aManager, aFinishID);
	mLogoWin.SetTexture(*aManager, aWinID);
	mLogoLose.SetTexture(*aManager, aLoseID);
}

void MySceneTimeOver::SetFinishPosition(const DXVector2 & aPosition)
{
	mLogoFinish.SetPosition(aPosition);
}

void MySceneTimeOver::SetFinishSize(const DXVector2 & aSize)
{
	mLogoFinish.SetSize(aSize);
}

void MySceneTimeOver::SetFinishScale(const DXVector2 & aScale)
{
	mLogoFinish.SetScale(aScale);
}

void MySceneTimeOver::SetIssuePosition(const DXVector2 & aPosition)
{
	mIssuePosition = aPosition;
}

void MySceneTimeOver::SetIssueSize(const DXVector2 & aSize)
{
	mIssueSize = aSize;
}

void MySceneTimeOver::SetIssueScale(const DXVector2 & aScale)
{
	mIssueScale = aScale;
}

void MySceneTimeOver::UpdateStart(const bool aIsWin)
{
	mIsWin = aIsWin;
	mActiveLogo = &mLogoFinish;
	mSequence = SceneTimeOverSequence::FirstWait;
	mFrameCounter = 0;
	mIsUpdating = true;
}

void MySceneTimeOver::Update()
{
	switch (mSequence)
	{
	case SceneTimeOverSequence::FirstWait:
		UpdateFirstWait();
		break;
	case SceneTimeOverSequence::Finishing:
		UpdateFinishing();
		break;
	case SceneTimeOverSequence::FinishedWait:
		UpdateFinishedWait();
		break;
	case SceneTimeOverSequence::Issuing:
		UpdateIssuing();
		break;
	case SceneTimeOverSequence::IssuedWait:
		UpdateIssuedWait();
		break;
	default:
		break;
	}
	++mFrameCounter;
}

bool MySceneTimeOver::IsUpdateing()
{
	return mIsUpdating;
}

void MySceneTimeOver::Render(MSSprite2DRender & aRender)
{
	if (mActiveLogo != nullptr) {
		aRender.Render(*mActiveLogo);
	}
}

void MySceneTimeOver::UpdateFirstWait()
{
	if (mFrameCounter == 0) {
		mActiveLogo = nullptr;;
	}
	else if (mFrameCounter > 60) {
		mSequence = SceneTimeOverSequence::Finishing;
		mFrameCounter = 0;
	}
}

void MySceneTimeOver::UpdateFinishing()
{
	if (mFrameCounter == 0) {
		mActiveLogo = &mLogoFinish;
	}
	else if (mFrameCounter > 120) {
		mSequence = SceneTimeOverSequence::FinishedWait;
		mFrameCounter = 0;
	}
}

void MySceneTimeOver::UpdateFinishedWait()
{
	if (mFrameCounter == 0) {
		mActiveLogo = nullptr;
	}
	else if (mFrameCounter > 30) {
		mSequence = SceneTimeOverSequence::Issuing;
		mFrameCounter = 0;
	}
}

void MySceneTimeOver::UpdateIssuing()
{
	if (mFrameCounter == 0) {
		if (mIsWin) {
			mActiveLogo = &mLogoWin;
		}
		else {
			mActiveLogo = &mLogoLose;
		}
	}
	else if (mFrameCounter > 120) {
		mSequence = SceneTimeOverSequence::FinishedWait;
		mFrameCounter = 0;
	}
}

void MySceneTimeOver::UpdateIssuedWait()
{
	if (mFrameCounter == 0) {
		mActiveLogo = nullptr;
	}
	else if (mFrameCounter > 60) {
		mIsUpdating = false;
	}
}
