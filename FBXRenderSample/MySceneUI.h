#pragma once
#include<memory>
class BarGauge;
class StatusFrame;
class ScoreView;

class MySceneUI {
	void UpdateAll();
	void RenderAll();
private:
	std::unique_ptr<StatusFrame> mStatusFrame;
	//std::unique_ptr<ScoreView>mScoreView;

};