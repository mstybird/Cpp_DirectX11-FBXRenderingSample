#include "MySceneUI.h"
#include"MSSprite2DRender.h"
#include"StatusFrame.h"
#include"BarGaugeVS.h"
#include"ScoreView.h"
#include"TimeView.h"
#include"AttackSlot.h"
MySceneUI::MySceneUI():
	mStatusFrame{ std::make_unique<StatusFrame>() },
	mScoreView{std::make_unique<ScoreView>()},
	mTimeView{std::make_unique<TimeView>()},
	mAttackSlot{std::make_unique<AttackSlot>()}
{
}

MySceneUI::~MySceneUI()
{
}

void MySceneUI::Release()
{
	mStatusFrame.reset();
	mScoreView.reset();
	mTimeView.reset();
}

void MySceneUI::Render(MSSprite2DRender & aRender, UIBase * aParent)
{
	mScoreView->Render(aRender, this);
	mStatusFrame->Render(aRender, this);
	mTimeView->Render(aRender, this);
	mAttackSlot->Render(aRender, this);
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

AttackSlot * MySceneUI::GetAttackSlot()
{
	return mAttackSlot.get();;
}
