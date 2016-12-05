#include "MySceneUI.h"
#include"MSSprite2DRender.h"
#include"StatusFrame.h"
#include"BarGaugeVS.h"
#include"ScoreView.h"
#include"TimeView.h"
MySceneUI::MySceneUI():
	mStatusFrame{ std::make_unique<StatusFrame>() },
	mScoreView{std::make_unique<ScoreView>()},
	mTimeView{std::make_unique<TimeView>()}
{
}

MySceneUI::~MySceneUI()
{
}

void MySceneUI::Render(MSSprite2DRender & aRender, UIBase * aParent)
{
	mScoreView->Render(aRender, this);
	mStatusFrame->Render(aRender, this);
	mTimeView->Render(aRender, this);
}

StatusFrame * MySceneUI::GetStatusFrame()
{
	return mStatusFrame.get();
}

ScoreView * MySceneUI::GetScoreView()
{
	return mScoreView.get();
}

TimeView * MySceneUI::GetTimeView()
{
	return mTimeView.get();
}
