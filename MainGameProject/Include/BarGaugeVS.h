#pragma once
#include"UIBase.h"
#include"MSProgressData.hxx"
#include"MSSprite2DResource.h"
#include<DXMath.hpp>
#include<unordered_map>

/*
	VS�X�R�A�Q�[�W�p�N���X
*/
class DX11TextureManager;

class BarGaugeVS :public UIBase {
public:
	BarGaugeVS();

	//�Q�[�W�̃t���[���A�Q�[�W���E���̐ݒ�
	void SetTextures(
		std::pair<DX11TextureManager, int>aFrame,
		std::pair<DX11TextureManager, int>aLeftImage,
		std::pair<DX11TextureManager, int>aRightImage
	);

	//�Q�[�W���̂��̂̃T�C�Y
	void SetSize(const float&aWidth, const float&aHeight);


	//�Q�[�W���g�̊g�嗦
	void SetScale(const float&aX, const float&aY);

	//�Q�[�W�̃I�t�Z�b�g��ݒ肷��(0.0~1.0
	void SetOffset(const float&aOffsetLeft, const float&aOffsetTop);
	void SetOffset(const DXVector2&aOffset);

	//�Q�[�W�p�̃X�e�[�^�X��ݒ肷��
	void SetParam(const float&aLeftParam, const float&aRightParam,bool aIsAnimation);

	//�ݒ肳�ꂽ�l�Ń��\�[�X���X�V����
	virtual void Update()override;
	//�eUI��n���Ďq�v�f�ɓK�p������
	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
private:
	//�Q�[�W�̍ő�l�̕���
	eGaugeDirection mDirection;

	//�Q�[�W�̊O�g(�t���[��)
	MSSprite2DResource mFrameImg;
	//�Q�[�W(��)
	MSSprite2DResource mGaugeLeftImg;
	//�Q�[�W(�E)
	MSSprite2DResource mGaugeRightImg;

	//�Q�[�W�X�R�A(��)
	float mStatusLeft;
	//�Q�[�W�X�R�A(�E)
	float mStatusRight;

	float mScreenScoreLeft;
	float mScreenScoreRight;


	//�Q�[�W�S�̂̃T�C�Y
	DXVector2 mGaugeSize;
	//�Q�[�W�̒��g�T�C�Y(�O�g�ɑ΂���X�P�[��)
	DXVector2 mGaugeScale;
	//�O�g������W����Ƃ����I�t�Z�b�g
	DXVector2 mGaugeOffset;

};


