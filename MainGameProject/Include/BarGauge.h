#pragma once
#include"UIBase.h"
#include"MSProgressData.hxx"
#include"MSSprite2DResource.h"
#include<DXMath.hpp>
#include<unordered_map>
/*
	UI�̃Q�[�W�Ǘ��p�N���X
*/
class DX11TextureManager;
class BarGauge :public UIBase{
public:

	BarGauge();

	void SetTextures(
		std::pair<DX11TextureManager, int>aOutImage,
		std::pair<DX11TextureManager, int>aInImage
	);

	//�Q�[�W�̃T�C�Y
	void SetSize(const float&aWidth, const float&aHeight);

	//�Q�[�W���g�̊g�嗦
	void SetGaugeScale(const float&aX, const float&aY);

	//�Q�[�W�̃I�t�Z�b�g��ݒ肷��(0.0~1.0
	void SetOffset(const float&aOffsetLeft,const float&aOffsetTop);
	void SetOffset(const DXVector2&aOffset);
	
	//�Q�[�W�p�̃X�e�[�^�X��ݒ肷��
	void SetParam(MSProgress&aParam);

	//�ݒ肳�ꂽ�l�Ń��\�[�X���X�V����
	virtual void Update()override;
	//�eUI��n���Ďq�v�f�ɓK�p������
	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
	


private:
	//�Q�[�W�̍ő�l�̕���
	eGaugeDirection mDirection;

	//�摜���\�[�X
	//�Q�[�W�O�g
	MSSprite2DResource mOutImage;
	//�Q�[�W����
	MSSprite2DResource mInImage;


	//�Q�[�W�X�e�[�^�X�p
	MSProgress mResource;
	//�Q�[�W�S�̂̃T�C�Y
	DXVector2 mGaugeSize;
	//�Q�[�W�̒��g�T�C�Y(�O�g�ɑ΂���X�P�[��)
	DXVector2 mGaugeScale;
	//�O�g������W����Ƃ����I�t�Z�b�g
	DXVector2 mGaugeOffset;
};

