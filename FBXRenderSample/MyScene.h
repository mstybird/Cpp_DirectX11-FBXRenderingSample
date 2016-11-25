#pragma once
#include<memory>
#include<iostream>
#include<vector>
#include"MSwrap.h"
#include"IncludeForward.h"
#include"Enemy.h"
#include"Player.h"
#include"StaticObject.h"
#include"Ball.h"
#include"StatusField.h"
#include<NcgLua.hpp>
#include"Comform\Effekseer\Effect.hxx"
#include"MSFbxDatabase.h"
#include"StatusFrame.h"
#include"BarGaugeVS.h"
#include"MySceneUI.h"
#include"Graphic_Text.h"
#include"BulletManager.h"
namespace ValueMyScene {
	namespace UI {

		static const int cUILuaID=100;

		static const int cStatusFrameID = 100;
		static const int cHPBarOutID = 110;
		static const int cHPBarInID = 111;
		static const int cEPBarOutID = 120;
		static const int cEPBarInID = 121;

		static const char* cStatusFramePositionName = "StatusFramePosition";
		static const char* cStatusFrameSize = "StatusFrameSize";
		static const char* cStatusFrameScale = "StatusFrameScale";
		static const char* cStatusFrameTexturePath = "StatusFrameTexturePath";

		static const char* cHPBarPosition = "HPBarPosition";
		static const char* cHPBarSize = "HPBarSize";
		static const char* cHPBarInScale = "HPBarInScale";
		static const char* cHPBarOutTexturePath = "HPBarOutTexturePath";
		static const char* cHPBarInTexturePath = "HPBarInTexturePath";

		static const char* cEPBarPosition = "EPBarPosition";
		static const char* cEPBarSize = "EPBarSize";
		static const char* cEPBarInScale = "EPBarInScale";
		static const char* cEPBarOutTexturePath = "EPBarOutTexturePath";
		static const char* cEPBarInTexturePath = "EPBarInTexturePath";
	}
}

class MyMSScene :public MSSceneBase {
public:
	MyMSScene();
	~MyMSScene();
private:
	void Initialize();
	void Update()override;
	void KeyDown(MSKEY pKey)override;
	void KeyHold(MSKEY pKey)override;
	void Render()override;
	void Destroy() override {}

private:
	void InitializeFont();
	void InitializeUI();

private:

	MSFbxDatabase mdDB;
	//描画に使うシェーダ
	My3DShader shader;
	//シーンに一つ
	MS3DRender render;

	//MSCollisionRayPicking lRayPick;


	std::vector<unique_ptr<Enemy>> enemy;
	Player mPlayer;
	StaticObject mField;
	Ball mBall;
	StatusField mFieldStatus;

	NcgLuaDatabase mLuaDb;

	DX11TextureManager mTexManager;

	//2Dスプライト関係
	MySprite2DShader m2DShader;
	MSSprite2DRender m2DRender;
	MSSprite2DResource mImage;

	//UI
	MySceneUI ui;

	//テキスト
	TextManager textMan;
	std::shared_ptr<TextGraphic> text;

	BulletManager bltManager;
	std::vector<std::unique_ptr<NBullet>> blts;

	//Effekseer関係
	::Comfort::EfkRenderer mEfkRender;
	::Comfort::EfkManager mEfkManager;
	::Comfort::EffectDatabase mEfkDb;
	::Comfort::EfkObject mEfkObj;

	const int cbox = 0;
	const int cChara = 1;
	const int cFieldD = 2;
	const int cFieldC = 3;
	const int cBall = 4;

};