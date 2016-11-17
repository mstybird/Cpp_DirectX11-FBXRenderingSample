#pragma once
#include"UIBase.h"
#include"MSSprite2DResource.h"
#include<memory>
/*
	HP�Q�[�W�AEP�Q�[�W���i�[���Ă���t���[��
*/
class BarGauge;
class StatusFrame :public UIBase {
	BarGauge*GetHPBar();
	BarGauge*GetEPBar();

private:
	//HP�o�[
	std::shared_ptr<BarGauge> mHpBar;
	//MP�o�[
	std::shared_ptr<BarGauge> mEpBar;
	//�t���[���C���[�W
	MSSprite2DResource mFrame;
};
