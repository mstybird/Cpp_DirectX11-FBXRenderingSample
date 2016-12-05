#pragma once
#include"MSSprite2DResource.h"
#include"StatusField.h"
enum class SceneTimeOverSequence {
	FirstWait,
	Finishing,
	FinishedWait,
	Issuing,
	IssuedWait,

};

class DX11TextureManager;
class MSSprite2DRender;
class MySceneTimeOver
{

public:
	MySceneTimeOver();
	~MySceneTimeOver();
	//�e�N�X�`���̐ݒ�
	void SetTextures(DX11TextureManager * aManager, const int aFinishID, const int aWinID, const int aLoseID,const int aDrawID);
	//�^�C�����~�b�g���S�̈ʒu
	void SetFinishPosition(const DXVector2&aPosition);
	//�^�C�����~�b�g���S�̃T�C�Y
	void SetFinishSize(const DXVector2&aSize);
	//�^�C�����~�b�g���S�̃X�P�[��
	void SetFinishScale(const DXVector2&aScale);
	//���s���S�̈ʒu
	void SetIssuePosition(const DXVector2&aPosition);
	//���s���S�̃T�C�Y
	void SetIssueSize(const DXVector2&aSize);
	//���s���S�̃X�P�[��
	void SetIssueScale(const DXVector2&aScale);
	//Update���J�n����(�����������ǂ��������肳����)
	void UpdateStart(const IssueFlag aIsWin);

	//�^�C���I�[�o�[�����̍X�V
	void Update();

	//�܂��X�V����(Update���I�����Ă��������ǂ���)
	bool IsUpdateing();

	//�`��
	void Render(MSSprite2DRender& aRender);
private:
	//�t�B�j�b�V�����S�\���O�̑ҋ@���
	void UpdateFirstWait();
	//�t�B�j�b�V�����S�\����
	void UpdateFinishing();
	//�t�B�j�b�V�����S�\����̑ҋ@
	void UpdateFinishedWait();
	//���s�\��
	void UpdateIssuing();
	//���s�\����
	void UpdateIssuedWait();

private:
	MSSprite2DResource mLogoFinish;
	MSSprite2DResource mLogoWin;
	MSSprite2DResource mLogoDraw;
	MSSprite2DResource mLogoLose;

	MSSprite2DResource* mActiveLogo;

	DXVector2 mIssuePosition;
	DXVector2 mIssueSize;
	DXVector2 mIssueScale;

	SceneTimeOverSequence mSequence;

	//�J�E���^
	int mFrameCounter;
	//�A�b�v�f�[�g���̃t���O
	bool mIsUpdating;
	//�����t���O
	IssueFlag mIsWin;
};
