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
	//2Dシェーダの初期化
	void InitShader();
	//テクスチャの読み込み
	void InitUI();

private:

	//Luaデータベース
	NcgLuaDatabase mLuaDb;
	//テクスチャマネージャ
	DX11TextureManager mTexManager;

	//2Dスプライト関係
	MySprite2DShader m2DShader;
	MSSprite2DRender m2DRender;
	MSSprite2DResource mBackground;
	MSSprite2DResource mSelectTitle;

	//サムネイル
	std::vector<MSSprite2DResource>mThumnails;
	MSSprite2DResource* mActiveThumbnail;

	//操作説明テキスト
	MSSprite2DResource mDescText;

};