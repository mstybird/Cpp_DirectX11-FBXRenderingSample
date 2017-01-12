#pragma once
#include"UIBase.h"
#include<DXMath.hpp>
#include<vector>
class Button;
class DXVector2;
class Toggle :public UIBase {
public:
	Toggle();
	virtual ~Toggle();
	//�{�^����ǉ�����
	void AddButton(Button* aButton);
	//�{�^���̎Q�Ƃ��N���A����
	void ClearButton();
	//�A�N�e�B�u�ɂ���{�^�����C���f�N�X�Ŏw�肷��
	bool SetActive(const int aIndex);
	//�{�^���̊Ԋu��ݒ肷��
	void SetPadding(DXVector2 aPadding);
	//�{�^���̊Ԋu��ݒ肷��
	void SetPadding(float aX,float aY);

	//�g�O���ɓo�^�ς݂̃{�^�������擾����
	int GetCount();
	//���݃A�N�e�B�u�ȃ{�^���̃C���f�b�N�X���擾����
	int GetActiveIndex();
	//���݃A�N�e�B�u�ȃ{�^�����擾����
	Button* GetActiveButton();
	//���̃{�^�����A�N�e�B�u�ɂ���
	int ActiveNext();
	//�O�̃{�^�����A�N�e�B�u�ɂ���
	int ActiveBack();
	//�A�N�e�B�u�ȃ{�^��������
	void PushButton();
	//�A�N�e�B�u�ȃ{�^���𗣂�
	void PopButton();
	//�`�揈��
	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;

	virtual void OnChangeIndex(const int Direction) {};

protected:

	//�p�f�B���O
	DXVector2 mPadding;

	//�{�^���̎Q�ƃ��X�g
	std::vector<Button*>mButtonArray;
	//�A�N�e�B�u�C���f�N�X
	int mActiveIndex;
};