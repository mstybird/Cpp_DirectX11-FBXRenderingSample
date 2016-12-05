#pragma once
#include"UIBase.h"
#include"Graphic_Text.h"

/*
	時間を表示するクラス
*/
class TimeView :public UIBase {
public:
	TimeView();
	~TimeView();
	
	void UpdateTime(const int mMinute, const int mSecond);

	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
private:
	//テキストマネージャ
	TextManager mTextMan;
	//スコアテキスト
	std::shared_ptr<TextGraphic>mTimeText;

};
