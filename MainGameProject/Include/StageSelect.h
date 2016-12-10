#pragma once
#include"MSSceneBase.h"
#include<NcgLua.hpp>
#include<DX11TextureManager.hpp>
#include"MySprite2DShader.h"
#include"MSSprite2DRender.h"
#include"MSSprite2DResource.h"
#include<memory>
#include<iostream>
#include<vector>



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

private:

	//Lua�f�[�^�x�[�X
	NcgLuaDatabase mLuaDb;
	//�e�N�X�`���}�l�[�W��
	DX11TextureManager mTexManager;

	//2D�X�v���C�g�֌W
	MySprite2DShader m2DShader;
	MSSprite2DRender m2DRender;
	MSSprite2DResource mBackground;
	MSSprite2DResource mSelectTitle;

	//�T���l�C��
	std::vector<MSSprite2DResource>mThumnails;
	MSSprite2DResource* mActiveThumbnail;

	//��������e�L�X�g
	MSSprite2DResource mDescText;

};