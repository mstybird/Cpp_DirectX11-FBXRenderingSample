#include "MySceneUI.h"
#include"MSSprite2DRender.h"
#include"StatusFrame.h"
#include"BarGaugeVS.h"
MySceneUI::MySceneUI():
	mStatusFrame{ std::make_unique<StatusFrame>() },
	mScoreView{std::make_unique<ScoreView>()}
{
}

MySceneUI::~MySceneUI()
{
}

void MySceneUI::Render(MSSprite2DRender & aRender, UIBase * aParent)
{
//	bar->Render(aRender, this);
	mStatusFrame->Render(aRender, this);
}

StatusFrame * MySceneUI::GetStatusFrame()
{
	return mStatusFrame.get();
}

ScoreView * MySceneUI::GetScoreView()
{
	return mScoreView.get();
}
