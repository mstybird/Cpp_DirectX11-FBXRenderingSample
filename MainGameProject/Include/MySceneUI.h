#pragma once
#include"UIBase.h"
#include<memory>
class BarGauge;
class StatusFrame;
class ScoreView;
class BarGaugeVS;
class TimeView;
class AttackSlot;
class MySceneUI:public UIBase {
public:
	MySceneUI();
	virtual ~MySceneUI();
	void Release();
	void UpdateAll() {}
	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr);
	StatusFrame* GetStatusFrame();
	ScoreView* GetScoreView();
	TimeView* GetTimeView();
	AttackSlot* GetAttackSlot();
	std::unique_ptr<StatusFrame> mStatusFrame;
private:
	std::unique_ptr<AttackSlot>mAttackSlot;
	std::unique_ptr<ScoreView>mScoreView;
	std::unique_ptr<TimeView>mTimeView;
};