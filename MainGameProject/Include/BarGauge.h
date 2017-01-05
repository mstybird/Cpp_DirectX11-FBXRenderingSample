#pragma once
#include"UIBase.h"
#include"MSProgressData.hxx"
#include"MSSprite2DResource.h"
#include<DXMath.hpp>
#include<unordered_map>
/*
	UIのゲージ管理用クラス
*/
class DX11TextureManager;
class BarGauge :public UIBase{
public:

	BarGauge();
	~BarGauge();
	void SetTextures(
		std::pair<DX11TextureManager, int>aOutImage,
		std::pair<DX11TextureManager, int>aInImage
	);

	//ゲージのサイズ
	void SetSize(const float&aWidth, const float&aHeight);

	//ゲージ中身の拡大率
	void SetGaugeScale(const float&aX, const float&aY);

	//ゲージのオフセットを設定する(0.0~1.0
	void SetOffset(const float&aOffsetLeft,const float&aOffsetTop);
	void SetOffset(const DXVector2&aOffset);
	
	//ゲージ用のステータスを設定する
	void SetParam(MSProgress&aParam);

	//設定された値でリソースを更新する
	virtual void Update()override;
	//親UIを渡して子要素に適用させる
	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
	


private:
	//ゲージの最大値の方向
	eGaugeDirection mDirection;

	//画像リソース
	//ゲージ外枠
	MSSprite2DResource mOutImage;
	//ゲージ内部
	MSSprite2DResource mInImage;


	//ゲージステータス用
	MSProgress mResource;
	//ゲージ全体のサイズ
	DXVector2 mGaugeSize;
	//ゲージの中身サイズ(外枠に対するスケール)
	DXVector2 mGaugeScale;
	//外枠左上座標を基準としたオフセット
	DXVector2 mGaugeOffset;
};

