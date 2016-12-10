#pragma once
#include"UIBase.h"
#include"MSSprite2DResource.h"
#include<memory>
#include"Graphic_Text.h"
/*
	HP�Q�[�W�AEP�Q�[�W���i�[���Ă���t���[��
*/
class BarGauge;
class MSSprite2DRender;
class DX11TextureManager;
struct StatusBase;
class StatusFrame :public UIBase {
public:
	StatusFrame();
	BarGauge*GetHPBar();
	BarGauge*GetEPBar();

	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
	void SetTexture(DX11TextureManager&aManager, const int&aTextureID);
	void SetSize(const float & aWidth, const float & aHeight);
	void SetHPTextOffset(const float aWidth, const float aHeight);
	void SetEPTextOffset(const float aWidth, const float aHeight);
	void UpdateStatus(StatusBase* aStatus);

private:
	//�e�L�X�g�}�l�[�W��
	TextManager mTextMan;


	//HP�o�[
	std::shared_ptr<BarGauge> mHpBar;
	std::shared_ptr<TextGraphic> mHpText;
	DXVector2 mHpTextPos;
	//MP�o�[
	std::shared_ptr<BarGauge> mEpBar;
	std::shared_ptr<TextGraphic> mEpText;
	DXVector2 mEpTextPos;
	//�t���[���C���[�W
	MSSprite2DResource mFrame;
};
