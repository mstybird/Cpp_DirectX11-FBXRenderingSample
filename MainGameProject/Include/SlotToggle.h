#pragma once
#include<Toggle.h>

class SlotToggle :public Toggle {
public:
	SlotToggle();
	virtual ~SlotToggle();
	virtual void OnChangeIndex(const int Direction)override;

	//�`�揈��
	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
	int mRenderCount;	//�`���
	bool mIsChangeAnimation;
	std::vector<DXVector2> mTargetPosition;
	UIBase* mParent;
};

