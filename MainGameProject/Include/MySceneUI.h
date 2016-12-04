#pragma once
#include"UIBase.h"
#include<memory>
class BarGauge;
class StatusFrame;
class ScoreView;
class BarGaugeVS;
class MySceneUI:public UIBase {
public:
	MySceneUI();
	virtual ~MySceneUI();
	void UpdateAll() {}
	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr);
	StatusFrame* GetStatusFrame();
	ScoreView* GetScoreView();
	std::unique_ptr<StatusFrame> mStatusFrame;
private:
	std::unique_ptr<ScoreView>mScoreView;

};