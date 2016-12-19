#pragma once
#include"MSSceneBase.h"
#include<NcgLua.hpp>
#include<DX11TextureManager.hpp>
#include"MySprite2DShader.h"
#include"MSSprite2DRender.h"
#include"MSSprite2DResource.h"
#include"StageSelectList.h"
#include<DXAL.hpp>
#include<CppTweener.h>
#include<MSThread.hpp>
#include<memory>
#include<iostream>
#include<vector>
#include<string>

/*
	Memo
	SelectList �̊���
	�Q�[���̐i�s�����P

*/

namespace ValueSS {

	namespace UI {
		static const int cLuaID = 0;
		static const char* cLuaFilePath = "Resource/Script/StageSelect.lua";

		static const char* cBackgroundPosition = "BackgroundPosition";
		static const char* cBackgroundSize = "BackgroundSize";
		static const char* cBackgroundScale = "BackgroundScale";
		static const char* cBackGroundTexturePath = "BackgroundTexturePath";

		static const char* cTitlePosition = "TitlePosition";
		static const char* cTitleSize = "TitleSize";
		static const char* cTitleScale = "TitleScale";
		static const char* cTitleTexturePath = "TitleTexturePath";

		static const char* cThumnailPosition = "ThumbnailPosition";
		static const char* cThumnailSize = "ThumbnailSize";
		static const char* cThumnailScale = "ThumbnailScale";
		static const char* cThumnailTexturePath = "ThumbnailTexturePath";

		static const char* cStageListFramePosition = "StageListFramePosition";
		static const char* cStageListFrameSize = "StageListFrameSize";
		static const char* cStageListFrameScale = "StageListFrameScale";
		static const char* cStageListFrameTexturePath = "StageListFrameTexturePath";

		static const char* cButtonPosition = "ButtonPosition";
		static const char* cButtonSize = "ButtonSize";
		static const char* cButtonScale = "ButtonScale";
		static const char* cButtonOffset = "ButtonOffset";

		static const char* cStagePathList = "StagePathList";

		static const char* cDescTextPosition = "DescTextPosition";
		static const char* cDescTextSize = "DescTextSize";
		static const char* cDescTextScale = "DescTextScale";
		static const char* cDescTextTexturePath = "DescTextTexturePath";

		static const int cBackGroundID = 0;
		static const int cTitleID = 10;
		static const int cThumnailID = 20;
		static const int cStageListFrameID = 30;
		static const int cDescTextID = 30;
		static const int cStagePathOffsetID = 500;

	}

	//ID:5000~
	namespace Sound {
		static const int cLuaID = 5000;
		static const char* cLuaPath = "Resource/Script/StageSelectSound.lua";


		static const char* cBGMFilePath = "BGMPath";
		static const char* cSESelectPath = "SESelectPath";
		static const char* cSEEnterPath = "SEEnterPath";

	}

}

class SceneStageSelect :public MSSceneBase {
public:
	void Initialize()override;
	void Update()override;
	void KeyDown(MSKEY pKey)override;
	void KeyHold(MSKEY pKey)override;
	void Render()override;
	void Destroy() override {}

private:
	//2D�V�F�[�_�̏�����
	void InitShader();
	//�e�N�X�`���̓ǂݍ���
	void InitUI();
	//�T�E���h�̓ǂݍ���
	void InitSound();
	//�I�������X�e�[�W�̈ꎞ�Z�[�u
	void StageSelectSave();

	//�V�[���J�ڂ̏���
	void UpdateSceneChange();
private:
	
	enum class SceneSequence {
		eChangeFirst,
		eChangeLoop,
		eChangeEnd
	};

	//Lua�f�[�^�x�[�X
	NcgLuaDatabase mLuaDb;
	//�e�N�X�`���}�l�[�W��
	DX11TextureManager mTexManager;

	//2D�X�v���C�g�֌W
	MySprite2DShader m2DShader;
	MSSprite2DRender m2DRender;

	//�w�i
	MSSprite2DResource mBackground;
	//�Z���N�g�^�C�g��
	MSSprite2DResource mSelectTitle;
	//��������e�L�X�g
	MSSprite2DResource mDescText;


	//�X�e�[�W���X�g
	StageSelectList mSelectList;
	SoundDevice mSoundDevice;
	SoundPlayer mBGM;
	SoundPlayer mSESelect;
	SoundPlayer mSEEnter;

	bool mIsSceneChange = false;
	//�V�[���J�ڂɎg���x���]���X���b�h
	MSThread mScneThread;
	//�V�[���J�ڂɎg���V�[�P���X�t���O
	SceneSequence mSequence;
	uint32_t mSceneTime;
	//�J�ڃV�[��
	std::unique_ptr<MSSceneBase> mScene;
	//Tween
	tween::Tweener mTweener;
	bool mIsMoveThumbnail = false;
};