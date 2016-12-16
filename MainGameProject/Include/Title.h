#pragma once
#include"MSSceneBase.h"
#include<DX11TextureManager.hpp>
#include<NcgLua.hpp>
#include"MySprite2DShader.h"
#include"MSSprite2DRender.h"
#include"MSSprite2DResource.h"
#include"Button.h"
#include"Toggle.h"
#include<CppTweener.h>
#include<MSThread.hpp>
#include<DXAL.hpp>
#include<memory>
#include<iostream>
#include<vector>

namespace ValueTitle{
	//0~499
	namespace UI {
		namespace Lua {
			static const int cLuaID = 0;
			static const char* cLuaFilePath = "Resource/Script/TitleUI.lua";

			namespace ValueName {

				static const char* cBackgroundPosition = "BackgroundPosition";
				static const char* cBackgroundSize = "BackgroundSize";
				static const char* cBackgroundScale = "BackgroundScale";
				static const char* cBackgroundTexturePath = "BackgroundTexturePath";

				static const char* cTitlePosition = "TitlePosition";
				static const char* cTitleSize = "TitleSize";
				static const char* cTitleScale = "TitleScale";
				static const char* cTitleTexturePath = "TitleTexturePath";

				static const char* cTogglePosition = "TogglePosition";
				static const char* cTogglePadding = "TogglePadding";


				static const char* cButtonStartSize = "ButtonStartSize";
				static const char* cButtonStartNormalTexturePath = "ButtonStartNormalTexturePath";
				static const char* cButtonStartPushTexturePath = "ButtonStartPushTexturePath";
				static const char* cButtonStartActiveTexturePath = "ButtonStartActiveTexturePath";
				static const char* cButtonStartDisableTexturePath = "ButtonStartDisableTexturePath";

				static const char* cButtonExitSize = "ButtonExitSize";
				static const char* cButtonExitNormalTexturePath = "ButtonExitNormalTexturePath";
				static const char* cButtonExitPushTexturePath = "ButtonExitPushTexturePath";
				static const char* cButtonExitActiveTexturePath = "ButtonExitActiveTexturePath";
				static const char* cButtonExitDisableTexturePath = "ButtonExitDisableTexturePath";


			}
		}
		namespace TextureID {
			static const int cBackgroundID = 100;
			static const int cTitleID = 110;
			static const int cButtonStartNormalID = 120;
			static const int cButtonStartActiveID = 121;
			static const int cButtonStartPushID = 122;
			static const int cButtonStartDisableID = 123;
			static const int cButtonExitNormalID = 130;
			static const int cButtonExitActiveID = 131;
			static const int cButtonExitPushID = 132;
			static const int cButtonExitDisableID = 133;
		}


	}

	namespace Sound{
		static const char* cLuaPath = "Resource/Script/TitleSound.lua";
		static const int cLuaID = 500;

		static const char* cBGMFilePath = "BGMPath";
		static const char* cSESelectPath = "SESelectPath";
		static const char* cSEEnterPath = "SEEnterPath";
	}
}

class SceneTitle :public MSSceneBase{
public:
	void Initialize()override;
	void Update()override;
	void KeyDown(MSKEY pKey)override;
	void KeyHold(MSKEY pKey)override;
	void Render()override;
	void Destroy() override {}

private:

	void UpdateSceneChange();

	//2D�V�F�[�_�̏�����
	void InitShader();
	//�e�N�X�`���̓ǂݍ���
	void InitUI();
	//�T�E���h�̏�����
	void InitSound();

	//�X�e�[�W�Z���N�g�V�[���`�F���W
	void SceneChange();
private:

	enum {
		eChangeToStageSelect = 0,
		eChangeToGameEnd
	};

	enum class SceneSequence {
		ChangeFirst,
		ChangeLoop,
		ChangeEnd
	};

	//Lua�f�[�^�x�[�X
	NcgLuaDatabase mLuaDb;
	//�e�N�X�`���}�l�[�W��
	DX11TextureManager mTexManager;

	//2D�X�v���C�g�֌W
	MySprite2DShader m2DShader;
	MSSprite2DRender m2DRender;
	MSSprite2DResource mBackground;
	MSSprite2DResource mTitle;

	Button mBtnStart;
	Button mBtnExit;
	Toggle mButtonList;
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
};