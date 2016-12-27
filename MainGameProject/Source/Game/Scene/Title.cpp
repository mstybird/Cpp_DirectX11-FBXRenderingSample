#include "Title.h"
#include"MyScene.h"
#include"StageSelect.h"
#include"MSDirect.h"
#include"MS3DRender.h"
#include<thread>
void SceneTitle::Initialize()
{
	InitShader();
	printf("OK:Shader\n");
	InitUI();
	printf("OK:UI\n");
	InitSound();
	printf("OK:Sound\n");
	mBGM.Play();
	InitializeEnd();
	printf("OK:All\n");
}
void SceneTitle::Update()
{
	UpdateSceneChange();
}
void SceneTitle::KeyDown(MSKEY pKey)
{
	if (mIsSceneChange)return;
	switch (pKey)
	{
	case MSKEY::UP:
		mButtonList.ActiveBack();
		mSESelect.Stop(true);
		mSESelect.Play();
		break;
	case MSKEY::DOWN:
		mButtonList.ActiveNext();
		mSESelect.Stop(true);
		mSESelect.Play();
		break;
	case MSKEY::ENTER:
		mSEEnter.Stop(true);
		mSEEnter.Play();
		mButtonList.PushButton();
		switch (mButtonList.GetActiveIndex()) 
		{
		case eChangeToStageSelect:
		{
			SceneChange();

		}
			break;
		case eChangeToGameEnd:
			PostQuitMessage(EXIT_SUCCESS);
			break;
		default:
			break;
		}
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

void SceneTitle::UpdateSceneChange()
{
	if (mIsSceneChange == false)return;

	switch (mSequence)
	{
	case SceneTitle::SceneSequence::ChangeFirst:
		mSceneTime = clock();
		mSequence = SceneSequence::ChangeLoop;
		{
			tween::TweenerParam lParam(2000, tween::ELASTIC, tween::EASE_IN);
			lParam.addProperty(&mTitle.mPosition.y, -150);
			mTweener.addTween(lParam);
		}
		//フォールスルー
	case SceneTitle::SceneSequence::ChangeLoop:
	{
		auto lNowTime = clock();
		if (mSceneTime + 2200 > lNowTime) {
			mTweener.step(lNowTime);
		}
		else {
			mSequence = SceneSequence::ChangeEnd;
		}
		


	}

		break;
	case SceneTitle::SceneSequence::ChangeEnd:
		mScneThread.Wait();
		MSDirect::SetScene(std::move(mScene));
		break;
	default:
		break;
	}

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

void SceneTitle::InitSound()
{
	using namespace ValueTitle::Sound;
	mLuaDb.Load(cLuaPath, cLuaID);
	auto lManager = mLuaDb.GetManager(cLuaID);
	std::string lFileName;
	lManager->GetGlobal(cBGMFilePath, lFileName);
	mBGM = mSoundDevice.CreateSoundFromFile(lFileName);

	lManager->GetGlobal(cSESelectPath, lFileName);
	mSESelect = mSoundDevice.CreateSoundFromFile(lFileName);

	lManager->GetGlobal(cSEEnterPath, lFileName);
	mSEEnter = mSoundDevice.CreateSoundFromFile(lFileName);


}

void SceneTitle::SceneChange()
{
	mScene = std::make_unique<SceneStageSelect>();

	mScneThread.SetFunction([&]() {mScene->Initialize(); });
	mScneThread.Start();
	mIsSceneChange = true;

}
