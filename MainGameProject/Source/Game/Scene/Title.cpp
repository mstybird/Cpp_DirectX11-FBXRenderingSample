#include "Title.h"
#include"MyScene.h"
#include"MSDirect.h"
#include"MS3DRender.h"
void SceneTitle::Initialize()
{
	InitShader();

	InitUI();

}
void SceneTitle::Update()
{
}
void SceneTitle::KeyDown(MSKEY pKey)
{
	switch (pKey)
	{
	case MSKEY::UP:
		mButtonList.ActiveBack();
		break;
	case MSKEY::DOWN:
		mButtonList.ActiveNext();
		break;
	case MSKEY::CH_Z:
		mButtonList.PushButton();
		break;
	case MSKEY::ENTER:
		MSDirect::SetScene(std::make_unique<MyMSScene>());
		break;
	default:
		break;
	}

}

void SceneTitle::KeyHold(MSKEY pKey)
{
}

void SceneTitle::Render()
{
	MS3DRender::Clear({ 0.2f,0.2f,0.2f,1 });
	m2DRender.Render(mBackground);
	m2DRender.Render(mTitle);
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
	auto lManager = mLuaDb.GetManager(ValueTitle::UI::Lua::cLuaID);
	{
		using namespace ValueTitle::UI::Lua;
		using namespace ValueTitle::UI::Lua::ValueName;
		using namespace ValueTitle::UI::TextureID;
		
		std::unordered_map<int, std::string>lFileNameMap;
		std::string lFileName;

		lManager->GetGlobal(cBackgroundTexturePath, lFileName);
		lFileNameMap[cBackgroundID] = lFileName;

		lManager->GetGlobal(cTitleTexturePath, lFileName);
		lFileNameMap[cTitleID] = lFileName;

		lManager->GetGlobal(cButtonStartNormalTexturePath, lFileName);
		lFileNameMap[cButtonStartNormalID] = lFileName;
		lManager->GetGlobal(cButtonStartActiveTexturePath, lFileName);
		lFileNameMap[cButtonStartActiveID] = lFileName;
		lManager->GetGlobal(cButtonStartPushTexturePath, lFileName);
		lFileNameMap[cButtonStartPushID] = lFileName;
		lManager->GetGlobal(cButtonStartDisableTexturePath, lFileName);
		lFileNameMap[cButtonStartDisableID] = lFileName;

		lManager->GetGlobal(cButtonExitNormalTexturePath, lFileName);
		lFileNameMap[cButtonExitNormalID] = lFileName;
		lManager->GetGlobal(cButtonExitActiveTexturePath, lFileName);
		lFileNameMap[cButtonExitActiveID] = lFileName;
		lManager->GetGlobal(cButtonExitPushTexturePath, lFileName);
		lFileNameMap[cButtonExitPushID] = lFileName;
		lManager->GetGlobal(cButtonExitDisableTexturePath, lFileName);
		lFileNameMap[cButtonExitDisableID] = lFileName;
		mTexManager.RegisterFileList(lFileNameMap);

	}
	//座標などの情報を取得する

	std::vector<float>lPosition;
	std::vector<float>lSize;
	std::vector<float>lScale;
	//解放用
	auto ClearTemp = [&]() {
		lPosition.clear();
		lSize.clear();
		lScale.clear();
	};

	using namespace ValueTitle::UI::Lua::ValueName;
	using namespace ValueTitle::UI::TextureID;
	//背景
	{
		lManager->GetGlobal(cBackgroundPosition, lPosition);
		lManager->GetGlobal(cBackgroundSize, lSize);
		lManager->GetGlobal(cBackgroundScale, lScale);
		mBackground.SetTexture(mTexManager, cBackgroundID);
		mBackground.SetPosition({ lPosition[0],lPosition[1] });
		mBackground.SetSize({ lSize[0], lSize[1] });
		mBackground.SetScale({ lScale[0], lScale[1] });

		ClearTemp();
	}

	//タイトル
	{
		lManager->GetGlobal(cTitlePosition, lPosition);
		lManager->GetGlobal(cTitleSize, lSize);
		lManager->GetGlobal(cTitleScale, lScale);
		mTitle.SetTexture(mTexManager, cTitleID);
		mTitle.SetPosition({ lPosition[0],lPosition[1] });
		mTitle.SetSize({ lSize[0], lSize[1] });
		mTitle.SetScale({ lScale[0], lScale[1] });


		ClearTemp();
	}

	//Toggle
	{
		std::vector<float>lPadding;
		lManager->GetGlobal(cTogglePosition, lPosition);
		lManager->GetGlobal(cTogglePadding, lPadding);
		mButtonList.AddButton(&mBtnStart);
		mButtonList.AddButton(&mBtnExit);
		mButtonList.SetGlobalPosition(lPosition[0], lPosition[1]);
		mButtonList.SetPadding(lPadding[0], lPadding[1]);
		ClearTemp();
	}

	//Button
	{
		mBtnStart.SetTextures(&mTexManager, cButtonStartNormalID, cButtonStartActiveID, cButtonStartPushID, cButtonStartDisableID);
		mBtnExit.SetTextures(&mTexManager, cButtonExitNormalID, cButtonExitActiveID, cButtonExitPushID, cButtonExitDisableID);
		lManager->GetGlobal(cButtonStartSize, lSize);
		mBtnStart.SetSize(lSize[0], lSize[1]);
		ClearTemp();
		lManager->GetGlobal(cButtonExitSize, lSize);
		mBtnExit.SetSize(lSize[0], lSize[1]);

	}
}
