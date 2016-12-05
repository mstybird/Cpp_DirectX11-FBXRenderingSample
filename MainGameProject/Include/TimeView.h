#pragma once
#include"UIBase.h"
#include"Graphic_Text.h"

/*
	���Ԃ�\������N���X
*/
class TimeView :public UIBase {
public:
	TimeView();
	~TimeView();
	
	void UpdateTime(const int mMinute, const int mSecond);

	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
private:
	//�e�L�X�g�}�l�[�W��
	TextManager mTextMan;
	//�X�R�A�e�L�X�g
	std::shared_ptr<TextGraphic>mTimeText;

};
