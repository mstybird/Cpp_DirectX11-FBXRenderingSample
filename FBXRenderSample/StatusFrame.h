#pragma once
#include"UIBase.h"
#include"MSSprite2DResource.h"
#include<memory>
/*
	HPゲージ、EPゲージを格納しているフレーム
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
	//HPバー
	std::shared_ptr<BarGauge> mHpBar;
	//MPバー
	std::shared_ptr<BarGauge> mEpBar;
	//フレームイメージ
	MSSprite2DResource mFrame;
};
