#pragma once
#include<memory>
#include<iostream>
#include<vector>
#include"MSwrap.h"
#include"IncludeForward.h"
#include"Enemy.h"
#include"Player.h"
#include"StaticObject.h"
#include"NcgLua\NcgLua.hpp"
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

	MSFbxManager mdBox;
	MSFbxManager mdField;		
	//描画に使うシェーダ
	My3DShader shader;
	//シーンに一つ
	MS3DRender render;

	MSCollisionRayPicking lRayPick;


	std::vector<unique_ptr<Enemy>> enemy;
	Player mPlayer;
	StaticObject mField;

	NcgLuaDatabase mLuaDb;

	DX11TextureManager mTexManager;

	MySprite2DShader m2DShader;
	MSSprite2DRender m2DRender;
	MSSprite2DResource mImage;

};