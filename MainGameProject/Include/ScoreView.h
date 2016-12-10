#pragma once
#include"UIBase.h"
#include"Graphic_Text.h"
#include<memory>
/*
	スコア表示を管理するクラス
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
	//テキストマネージャ
	TextManager mTextMan;

	//スコアゲージ
	std::shared_ptr<BarGaugeVS>mScoreBar;
	//スコアテキスト
	std::shared_ptr<TextGraphic>mScoreText;
	DXVector2 mTextOffset;

};
