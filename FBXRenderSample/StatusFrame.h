#pragma once
#include"UIBase.h"
#include"MSSprite2DResource.h"
#include<memory>
/*
	HP�Q�[�W�AEP�Q�[�W���i�[���Ă���t���[��
*/
class BarGauge;
class MSSprite2DRender;
class DX11TextureManager;
class StatusFrame :public UIBase {
public:
	StatusFrame();
	BarGauge*GetHPBar();
	BarGauge*GetEPBar();

	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
	void SetTexture(DX11TextureManager&aManager, const int&aTextureID);
	void SetSize(const float & aWidth, const float & aHeight);
private:
	//HP�o�[
	std::shared_ptr<BarGauge> mHpBar;
	//MP�o�[
	std::shared_ptr<BarGauge> mEpBar;
	//�t���[���C���[�W
	MSSprite2DResource mFrame;
};
