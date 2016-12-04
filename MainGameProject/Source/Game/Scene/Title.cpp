#include "Title.h"
#include"MyScene.h"
#include"MSDirect.h"
#include"MS3DRender.h"
void SceneTitle::Initialize()
{
	InitShader();


	mTexManager.RegistFromFile("Resource/Texture/Title/Chips_Cover.jpg", 0);
	mBtnStart.SetTextures(&mTexManager, 0, 0, 0, 0);
	mBtnStart.SetSize( 200,200 );
	mBtnStart.SetGlobalPosition( 200, 100 );
	mBtnExit.SetTextures(&mTexManager, 0, 0, 0, 0);
	mBtnExit.SetSize(200,200 );
	mBtnExit.SetGlobalPosition( 200, 100 );

	mButtonList.AddButton(&mBtnStart);
	mButtonList.AddButton(&mBtnExit);
	mButtonList.SetGlobalPosition({ 600,50 });

	mButtonList.SetPadding(0, 20);

	mImage.SetSize({ 200,200 });
	mImage.SetPosition({200, 100});
}
void SceneTitle::Update()
{
}
void SceneTitle::KeyDown(MSKEY pKey)
{
	if (pKey == MSKEY::CH_Z) {
		MSDirect::SetScene(std::make_unique<MyMSScene>());
	}
}

void SceneTitle::KeyHold(MSKEY pKey)
{
}

void SceneTitle::Render()
{
	MS3DRender::Clear({ 0.2f,0.8f,0.2f,1 });
	//m2DRender.Render(mImage);
	mButtonList.Render(m2DRender);
}

void SceneTitle::InitShader()
{
	m2DShader.Init();
	m2DShader.InitVertex("Resource/HLSL/Sprite2D.hlsl");
	m2DShader.InitPixel("Resource/HLSL/Sprite2D.hlsl");
	m2DRender.SetViewPort(MSDirect::GetViewPort());
	m2DRender.SetShader(m2DShader);

}

void SceneTitle::InitUI()
{
	//設定ファイル読み込み
	{
		using namespace ValueTitle::UI::Lua;
		mLuaDb.Load(cLuaFilePath, cLuaID);
	}
	//テクスチャ一括読み込み
	//テクスチャ一括登録
	{
		using namespace ValueTitle::UI::Lua;
		using namespace ValueTitle::UI::Lua::ValueName;
		using namespace ValueTitle::UI::TextureID;
		
		auto lManager = mLuaDb.GetManager(cLuaID);
		std::unordered_map<int, std::string>lFileNameMap;
		std::string lFileName;

		lManager->GetGlobal(cBackgroundTexturePath, lFileName);
		lFileNameMap[cBackgroundID] = lFileName;

		lManager->GetGlobal(cBackgroundTexturePath, lFileName);
		lFileNameMap[cTitleID] = lFileName;

		lManager->GetGlobal(cBackgroundTexturePath, lFileName);
		lFileNameMap[cButtonStartNormalID] = lFileName;
		lManager->GetGlobal(cBackgroundTexturePath, lFileName);
		lFileNameMap[cButtonStartActiveID] = lFileName;
		lManager->GetGlobal(cBackgroundTexturePath, lFileName);
		lFileNameMap[cButtonStartPushID] = lFileName;
		lManager->GetGlobal(cBackgroundTexturePath, lFileName);
		lFileNameMap[cButtonStartDisableID] = lFileName;

		lManager->GetGlobal(cBackgroundTexturePath, lFileName);
		lFileNameMap[cButtonExitNormalID] = lFileName;
		lManager->GetGlobal(cBackgroundTexturePath, lFileName);
		lFileNameMap[cButtonExitActiveID] = lFileName;
		lManager->GetGlobal(cBackgroundTexturePath, lFileName);
		lFileNameMap[cButtonExitPushID] = lFileName;
		lManager->GetGlobal(cBackgroundTexturePath, lFileName);
		lFileNameMap[cButtonExitDisableID] = lFileName;
		mTexManager.RegisterFileList(lFileNameMap);

		//座標などの情報を取得する
	}
}
