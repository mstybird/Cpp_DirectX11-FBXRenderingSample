#pragma once
#include"UIBase.h"
#include"MSSprite2DResource.h"

enum class ButtonState {
	Normal,
	Active,
	Push,
	Disable
};

struct ButtonDesc {
	DX11TextureManager* mManager;
	int mNormalID;
	int mActiveID;
	int mPushID;
	int mDisableID;
	DXVector2 mSize;
};


class Button :public UIBase {
public:

	Button();
	~Button();
	//�e�N�X�`�����Z�b�g����
	void SetTextures(
		DX11TextureManager* aManager,
		const int aNormalID,
		const int aActiveID,
		const int aPushID,
		const int aDisableID
	);

	void SetSize(const float&aWidth, const float&aHeight);
	//GlobalScale�Ɗ|�����킹���T�C�Y�̎擾
	DXVector2 GetSizeGlobal();
	//�g�嗦�𖳎������T�C�Y
	DXVector2 GetSizeLocal();
	DXVector2 GetScalingSize(UIBase*aParent = nullptr);
	//�w�肵����Ԃɂ��A���O�̃X�e�[�g��Ԃ�
	ButtonState SetState(const ButtonState aState);
	ButtonState GetState();

	//�I����Ԃɂ��A���O�̃X�e�[�g��Ԃ�
	ButtonState SetActive();
	//������Ԃɂ��A���O�̃X�e�[�g��Ԃ�
	ButtonState SetDisable();
	//�ʏ��Ԃɂ��A���O�̃X�e�[�g��Ԃ�
	ButtonState SetNormal();
	//�����Ă����Ԃɂ��A���O�̃X�e�[�g��Ԃ�
	ButtonState SetPushing();


	//���݃A�N�e�B�u�����ׂ�
	bool IsActive();
	//���ݖ�����Ԃ����ׂ�
	bool IsDisable();
	//���ݒʏ펞(�I��������������Ă��Ȃ�)�����ׂ�
	bool IsNormal();
	//���݂�������Ă��邩���ׂ�
	bool IsPushing();
	//�eUI��n���Ďq�v�f�ɓK�p������
	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
private:
	//���݂̏�Ԃ̃X�v���C�g���\�[�X���擾����
	MSSprite2DResource* GetSelecttingImage();

private:
	//�摜���\�[�X
	//�ʏ펞(��I����)
	MSSprite2DResource mImageNormal;
	//�I����
	MSSprite2DResource mImageActive;
	//�������Ƃ�
	MSSprite2DResource mImagePush;
	//�����Ȏ�
	MSSprite2DResource mImageDisable;

	//�{�^���̏��
	ButtonState mState;

	//�{�^���̃T�C�Y
	DXVector2 mSize;
};