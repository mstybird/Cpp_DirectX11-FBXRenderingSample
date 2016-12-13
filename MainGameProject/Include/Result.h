#pragma once
#include"MSSprite2DResource.h"
#include"Graphic_Text.h"
#include"Toggle.h"
#include"Button.h"
#include"StatusField.h"
struct ResultValue {
	IssueFlag mIssue;
	int mWhiteScore;
	int mBlackScore;
};

class DX11TextureManager;
class MSSprite2DRender;
class MySceneResult {
public:
	MySceneResult();
	~MySceneResult();
	void SetFrameTexture(DX11TextureManager * aManager, const int aFrameID);
	void SetLogoTexture(DX11TextureManager * aManager, const int aWinID, const int aLoseID, const int aDrawID);
	void SetButtonRetryTexture(DX11TextureManager * aManager, const int aNormalID, const int aActiveID, const int aPushID, const int aDisableID);
	void SetButtonToTitleTexture(DX11TextureManager * aManager, const int aNormalID, const int aActiveID, const int aPushID, const int aDisableID);

	void SetFramePosition(const float aX, const float aY);
	void SetLogoPosition(const float aX, const float aY);
	void SetButtonPosition(const float aX, const float aY);
	void SetTextPosition(const float aX, const float aY);

	void SetFrameSize(const float aX, const float aY);
	void SetLogoSize(const float aX, const float aY);
	void SetButtonSize(const float aX, const float aY);

	void SetButtonPadding(const float aX, const float aY);

	//���U���g�Ɏg���f�[�^������������
	void SetValues(const ResultValue& aValue);
	//���U���g�`��
	void Render(MSSprite2DRender&aRender);

	//�{�^������(�O��)
	void ButtonBack();
	//�{�^������(����)
	void ButtonNext();
	//�{�^��������
	void ButtonPush();
	//�{�^���𗣂�
	void ButtonUp();
	//�A�N�e�B�u�ȃ{�^���̃C���f�N�X���擾����
	int GetButtonActiveIndex();

private:
	TextManager mTextMan;
	//�t���[���摜
	MSSprite2DResource mFrame;
	//�������S
	MSSprite2DResource mLogoWin;
	//�����������S
	MSSprite2DResource mLogoDraw;
	//�s�k���S
	MSSprite2DResource mLogoLose;
	//�g�p���郍�S
	MSSprite2DResource* mActiveLogo;

	//�X�R�A�e�L�X�g
	std::shared_ptr<TextGraphic>mTextScore;
	DXVector2 mTextPos;

	//���S���
	DXVector2 mLogoPos;
	DXVector2 mLogoSize;

	Toggle mButtonList;
	Button mButtonRetry;
	Button mButtonToTitle;

};