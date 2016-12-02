#pragma once
#include"MSSceneBase.h"
#include<DX11TextureManager.hpp>
#include<NcgLua.hpp>
#include"MySprite2DShader.h"
#include"MSSprite2DRender.h"
#include<memory>
#include<iostream>
#include<vector>

namespace ValueTitle{
	namespace UI {
		static const char* cBackgroundPosition = "";
		static const char* cBackgroundSize = "";
		static const char* cBackgroundScale = "";
		static const char* cBackgroundTexturePath = "";

		static const char* cButtonPosition = "";
		static const char* cButtonSize = "";
		static const char* cButtonScale = "";
		static const char* cButtonNormalTexturePath = "";
		static const char* cButtonPushTexturePath = "";
		static const char* cButtonActiveTexturePath = "";
		static const char* cButtonNoActiveTexturePath = "";

		static const char* cButtonPosition = "";
		static const char* cButtonSize = "";
		static const char* cButtonScale = "";
		static const char* cButtonNormalTexturePath = "";
		static const char* cButtonPushTexturePath = "";
		static const char* cButtonActiveTexturePath = "";
		static const char* cButtonNoActiveTexturePath = "";

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


	NcgLuaDatabase mLuaDb;

	DX11TextureManager mTexManager;

	//2Dスプライト関係
	MySprite2DShader m2DShader;
	MSSprite2DRender m2DRender;

};