#pragma once
#include"UIBase.h"
#include"MSSprite2DResource.h"
#include<memory>
/*
	HPゲージ、EPゲージを格納しているフレーム
*/
class BarGauge;
class StatusFrame :public UIBase {
	BarGauge*GetHPBar();
	BarGauge*GetEPBar();

private:
	//HPバー
	std::shared_ptr<BarGauge> mHpBar;
	//MPバー
	std::shared_ptr<BarGauge> mEpBar;
	//フレームイメージ
	MSSprite2DResource mFrame;
};
