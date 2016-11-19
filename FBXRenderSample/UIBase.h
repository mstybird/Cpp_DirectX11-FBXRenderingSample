#pragma once
#include"DXMath.hpp"
class MSSprite2DRender;
enum class eGaugeDirection {
	Left,
	Right
};

//UI基底クラス
class UIBase{
public:
	UIBase();
	virtual ~UIBase() {}
	virtual void Update() {}
	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr) = 0;

	void SetGlobalPosition(const float&aOffsetX, const float&aOffsetY);
	void SetGlobalPosition(const DXVector2&aOffset);
	void SetGlobalScale(const float&aOffsetX, const float&aOffsetY);
	void SetGlobalScale(const DXVector2&aOffset);

	DXVector2* GetGlobalPosition();
	DXVector2* GetGlobalScale();

	//現在の値をバッファに退避する
	void StoreGlobalData();
	//バッファの値で現在の値を更新する
	void LoadGlobalData();

protected:
	//このオブジェクトが持つグローバル座標
	DXVector2 mGlobalPosition;
	//このオブジェクトが持つグローバルスケール
	DXVector2 mGlobalScale;

	//このオブジェクトが持つグローバル座標の一時退避用
	DXVector2 mGlobalPositionBuffer;
	//このオブジェクトが持つグローバルスケールの一時退避用
	DXVector2 mGlobalScaleBuffer;


};