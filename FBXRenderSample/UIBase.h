#pragma once
#include"DXMath.hpp"
class MSSprite2DRender;
enum class eGaugeDirection {
	Left,
	Right
};

//UI���N���X
class UIBase{
public:
	virtual ~UIBase() {}
	virtual void Update() {}
	virtual void Render(MSSprite2DRender aRender) = 0;

	void SetGlobalPosition(const float&aOffsetX, const float&aOffsetY);
	void SetGlobalPosition(const DXVector2&aOffset);

protected:
	DXVector2 mGlobalPosition;

};