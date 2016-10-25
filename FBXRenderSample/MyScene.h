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
#include"NcgLua\NcgLua.hpp"
#include"Comform\Effekseer\Effect.hxx"
#include"MSFbxDatabase.h"
class MyMSScene :public MSSceneBase {
public:
	MyMSScene();
	~MyMSScene() {
	}
private:
	void Initialize();
	void Update()override;
	void KeyDown(MSKEY pKey)override;
	void KeyHold(MSKEY pKey)override;
	void Render();
	void Destroy() override {}

private:

	MSFbxDatabase mdDB;

	//描画に使うシェーダ
	My3DShader shader;
	//シーンに一つ
	MS3DRender render;

	MSCollisionRayPicking lRayPick;


	std::vector<unique_ptr<Enemy>> enemy;
	Player mPlayer;
	StaticObject mField;
	Ball mBall;
	StatusField mFieldStatus;

	NcgLuaDatabase mLuaDb;

	DX11TextureManager mTexManager;

	MySprite2DShader m2DShader;
	MSSprite2DRender m2DRender;
	MSSprite2DResource mImage;

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