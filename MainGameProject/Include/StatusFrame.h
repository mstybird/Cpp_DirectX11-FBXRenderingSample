#pragma once
#include"UIBase.h"
#include"MSSprite2DResource.h"
#include<memory>
#include"Graphic_Text.h"
/*
	HPゲージ、EPゲージを格納しているフレーム
*/
class BarGauge;
class MSSprite2DRender;
class DX11TextureManager;
class StatusBase;
class StatusFrame :public UIBase {
public:
	StatusFrame();
	BarGauge*GetHPBar();
	BarGauge*GetEPBar();

	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
	void SetTexture(DX11TextureManager&aManager, const int&aTextureID);
	void SetSize(const float & aWidth, const float & aHeight);

	void UpdateStatus(StatusBase* aStatus);

private:
	//テキストマネージャ
	TextManager mTextMan;


	//HPバー
	std::shared_ptr<BarGauge> mHpBar;
	std::shared_ptr<TextGraphic> mHpText;
	DXVector2 mHpTextPos;
	//MPバー
	std::shared_ptr<BarGauge> mEpBar;
	std::shared_ptr<TextGraphic> mEpText;
	DXVector2 mEpTextPos;
	//フレームイメージ
	MSSprite2DResource mFrame;
};
