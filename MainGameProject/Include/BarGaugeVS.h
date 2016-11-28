#pragma once
#include"UIBase.h"
#include"MSProgressData.hxx"
#include"MSSprite2DResource.h"
#include<DXMath.hpp>
#include<unordered_map>

/*
	VSスコアゲージ用クラス
*/
class DX11TextureManager;

class BarGaugeVS :public UIBase {
public:
	BarGaugeVS();

	//ゲージのフレーム、ゲージ左右二種の設定
	void SetTextures(
		std::pair<DX11TextureManager, int>aFrame,
		std::pair<DX11TextureManager, int>aLeftImage,
		std::pair<DX11TextureManager, int>aRightImage
	);

	//ゲージそのもののサイズ
	void SetSize(const float&aWidth, const float&aHeight);


	//ゲージ中身の拡大率
	void SetScale(const float&aX, const float&aY);

	//ゲージのオフセットを設定する(0.0~1.0
	void SetOffset(const float&aOffsetLeft, const float&aOffsetTop);
	void SetOffset(const DXVector2&aOffset);

	//ゲージ用のステータスを設定する
	void SetParam(MSProgress&aLeftParam, MSProgress&aRightParam);

	//設定された値でリソースを更新する
	virtual void Update()override;
	//親UIを渡して子要素に適用させる
	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
private:
	//ゲージの最大値の方向
	eGaugeDirection mDirection;

	//ゲージの外枠(フレーム)
	MSSprite2DResource mFrameImg;
	//ゲージ(左)
	MSSprite2DResource mGaugeLeftImg;
	//ゲージ(右)
	MSSprite2DResource mGaugeRightImg;

	//ゲージステータス(左)
	MSProgress mStatusLeft;
	//ゲージステータス(右)
	MSProgress mStatusRight;

	//ゲージ全体のサイズ
	DXVector2 mGaugeSize;
	//ゲージの中身サイズ(外枠に対するスケール)
	DXVector2 mGaugeScale;
	//外枠左上座標を基準としたオフセット
	DXVector2 mGaugeOffset;

};


