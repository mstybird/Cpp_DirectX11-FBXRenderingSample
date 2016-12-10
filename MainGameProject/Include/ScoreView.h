#pragma once
#include"UIBase.h"
#include"Graphic_Text.h"
#include<memory>
/*
	�X�R�A�\�����Ǘ�����N���X
*/
class BarGaugeVS;
class DXVector2;
class ScoreView :public UIBase {
public:
	ScoreView();
	~ScoreView();
	void SetSize(const DXVector2&aSize);
	void SetSize(float aWidth, float aHeight);
	void SetTextOffset(const DXVector2&aOffset);
	void SetTextOffset(float aWidth, float aHeight);
	void SetGaugeScale(float aX, float aY);
	void SetTextures(DX11TextureManager*aTexManager,const int aFrameID, const int aLeftID, const int aRightID);

	void UpdateScore(float aLeft, float aRight);

	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
private:
	//�e�L�X�g�}�l�[�W��
	TextManager mTextMan;

	//�X�R�A�Q�[�W
	std::shared_ptr<BarGaugeVS>mScoreBar;
	//�X�R�A�e�L�X�g
	std::shared_ptr<TextGraphic>mScoreText;
	DXVector2 mTextOffset;

};
