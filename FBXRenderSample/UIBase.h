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

	//���݂̒l���o�b�t�@�ɑޔ�����
	void StoreGlobalData();
	//�o�b�t�@�̒l�Ō��݂̒l���X�V����
	void LoadGlobalData();

protected:
	//���̃I�u�W�F�N�g�����O���[�o�����W
	DXVector2 mGlobalPosition;
	//���̃I�u�W�F�N�g�����O���[�o���X�P�[��
	DXVector2 mGlobalScale;

	//���̃I�u�W�F�N�g�����O���[�o�����W�̈ꎞ�ޔ�p
	DXVector2 mGlobalPositionBuffer;
	//���̃I�u�W�F�N�g�����O���[�o���X�P�[���̈ꎞ�ޔ�p
	DXVector2 mGlobalScaleBuffer;


};