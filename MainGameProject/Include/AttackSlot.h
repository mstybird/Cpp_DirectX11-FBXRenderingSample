#pragma once
#include"UIBase.h"
#include"Toggle.h"
#include"SlotToggle.h"
#include"Button.h"
class AttackSlot :public UIBase {
public:

	AttackSlot();
	~AttackSlot();

	void AddSlot(ButtonDesc aDesc);


	SlotToggle mSlot;					//攻撃スロットデータ
	std::vector<Button>mItemArray;	//スロット用ボタンデータ

	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;

};