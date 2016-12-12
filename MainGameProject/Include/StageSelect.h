#pragma once
#include"MSSceneBase.h"
#include<NcgLua.hpp>
#include<DX11TextureManager.hpp>
#include"MySprite2DShader.h"
#include"MSSprite2DRender.h"
#include"MSSprite2DResource.h"
#include"StageSelectList.h"
#include<memory>
#include<iostream>
#include<vector>
#include<string>

/*
	Memo
	SelectList の完成
	ゲームの進行を改善

*/

namespace ValueSS {
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

	//背景
	MSSprite2DResource mBackground;
	//セレクトタイトル
	MSSprite2DResource mSelectTitle;
	//操作説明テキスト
	MSSprite2DResource mDescText;

	//サムネイル
	std::vector<MSSprite2DResource>mThumnails;
	MSSprite2DResource* mActiveThumbnail;

	//ステージリスト
	StageSelectList mSelectList;

};