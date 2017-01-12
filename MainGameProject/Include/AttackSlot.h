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


	SlotToggle mSlot;					//�U���X���b�g�f�[�^
	std::vector<Button>mItemArray;	//�X���b�g�p�{�^���f�[�^

	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;

};