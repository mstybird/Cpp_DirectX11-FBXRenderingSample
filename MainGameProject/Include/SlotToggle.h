#pragma once
#include<Toggle.h>

class SlotToggle :public Toggle {
public:
	SlotToggle();
	virtual ~SlotToggle();
	virtual void OnChangeIndex(const int Direction)override;

	//•`‰æˆ—
	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
	int mRenderCount;	//•`‰æŒÂ”
	bool mIsChangeAnimation;
	std::vector<DXVector2> mTargetPosition;
	UIBase* mParent;
};

