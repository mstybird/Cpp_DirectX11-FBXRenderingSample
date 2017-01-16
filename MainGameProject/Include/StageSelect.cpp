#include "StageSelect.h"
#include"MSDirect.h"
#include"MyScene.h"
#include"Title.h"
#include"MSKeyCodeList.h"
#include<sstream>
#include<fstream>
#include<algorithm>
#include<direct.h>
#include<memory>
void SceneStageSelect::Initialize()
{
	InitShader();
	printf("OK:Shader\n");
	InitUI();
	printf("OK:UI\n");
	InitSound();
	printf("OK:Sound\n");
	InitializeEnd();
	printf("OK:All\n");

}

void SceneStageSelect::Update()
{
	mBGM.Play();
	UpdateSceneChange();
}

void SceneStageSelect::KeyDown(MSKEY pKey)
{
	if (mIsSceneChange)return;
	switch (pKey)
	{
	case MSKEY::UP:
		mSelectList.ActiveBack();
		mSESelect.Stop(true);
		mSESelect.Play();
		break;
	case MSKEY::DOWN:
		mSelectList.ActiveNext();
		mSESelect.Stop(true);
		mSESelect.Play();
		break;
	case MSKEY::ENTER:
		mSEEnter.Stop(true);
		mSEEnter.Play();
		StageSelectSave();

		mScene = std::make_unique<MyMSScene>();

		mScneThread.SetFunction([&]() {mScene->Initialize(); });
		mScneThread.Start();
		mIsSceneChange = true;

		break;
	case MSKEY::BACKSPACE:
		mSEEnter.Stop(true);
		mSEEnter.Play();

		mScene = std::make_unique<SceneTitle>();

		mScneThread.SetFunction([&]() {mScene->Initialize(); });
		mScneThread.Start();
		mIsSceneChange = true;
		break;
	default:
		break;
	}
}

void SceneStageSelect::KeyHold(MSKEY pKey)
{

}
void SceneStageSelect::MouseMove(const POINT & aNowPosition, const POINT & aDiffPosition)
{
	mCursor.AddPosition(aDiffPosition.x, aDiffPosition.y);
}
void SceneStageSelect::MouseDown(const MouseType aType)
{
	if (mIsSceneChange)return;
	int lHitIndex = this->mSelectList.GetList()->CollisionPoint(mCursor.mPosition.x, mCursor.mPosition.y);
	if (lHitIndex != -1) {
		if (lHitIndex == mSelectList.GetList()->GetActiveIndex()) {
			mSelectList.GetList()->PushButton();
			mButtonLastPushed = lHitIndex;
		}
	}
}
void SceneStageSelect::MouseUp(const MouseType aType)
{
}
void SceneStageSelect::Render()
{
	
	m2DRender.Render(mBackground);
	m2DRender.Render(mSelectTitle);

	mSelectList.Render(m2DRender);

	m2DRender.Render(mDescText);
}

void SceneStageSelect::InitShader()
{
	m2DShader.Init();
	m2DShader.InitVertex("Resource/HLSL/Sprite2D.hlsl");
	m2DShader.InitPixel("Resource/HLSL/Sprite2D.hlsl");
	m2DRender.SetViewPort(MSDirect::GetViewPort());
	m2DRender.SetShader(m2DShader);
}

void SceneStageSelect::InitUI()
{
	using namespace ValueSS::UI;
	mLuaDb.Load(cLuaFilePath, cLuaID);
	auto lManager = mLuaDb.GetManager(cLuaFilePath);
	//
	printf("Ok:Lua[%s]\n", cLuaFilePath);
	std::unordered_map<int, std::string>lFileNameMap;
	std::string lFileName;
	{
		lManager->GetGlobal(cBackGroundTexturePath, lFileName);
		lFileNameMap[cBackGroundID] = lFileName;

		lManager->GetGlobal(cTitleTexturePath, lFileName);
		lFileNameMap[cTitleID] = lFileName;

		lManager->GetGlobal(cThumnailTexturePath, lFileName);
		lFileNameMap[cThumnailID] = lFileName;

		lManager->GetGlobal(cDescTextTexturePath, lFileName);
		lFileNameMap[cDescTextID] = lFileName;
		printf("Ok:Load Value\n");
		mTexManager.RegisterFileList(lFileNameMap);
		printf("Ok:Regist Texture\n");
	}
	printf("OK:Texture\n");

	std::vector<float>lPosition;
	std::vector<float>lSize;
	std::vector<float>lScale;

	//解放用
	auto ClearTemp = [&]() {
		lPosition.clear();
		lSize.clear();
		lScale.clear();
	};

	//背景
	{
		lManager->GetGlobal(cBackgroundPosition, lPosition);
		lManager->GetGlobal(cBackgroundSize, lSize);
		lManager->GetGlobal(cBackgroundScale, lScale);
		mBackground.SetTexture(mTexManager, cBackGroundID);
		mBackground.SetPosition({ lPosition[0],lPosition[1] });
		mBackground.SetSize({ lSize[0], lSize[1] });
		mBackground.SetScale({ lScale[0], lScale[1] });

		ClearTemp();
	}


	//セレクトタイトル
	{
		lManager->GetGlobal(cTitlePosition, lPosition);
		lManager->GetGlobal(cTitleSize, lSize);
		lManager->GetGlobal(cTitleScale, lScale);
		mSelectTitle.SetTexture(mTexManager, cTitleID);
		mSelectTitle.SetPosition({ lPosition[0],lPosition[1] });
		mSelectTitle.SetSize({ lSize[0], lSize[1] });
		mSelectTitle.SetScale({ lScale[0], lScale[1] });
		mSelectTitle.SetPivot({ 0.5f,0.5f });
		ClearTemp();
	}

	//操作説明テキスト
	{
		lManager->GetGlobal(cDescTextPosition, lPosition);
		lManager->GetGlobal(cDescTextSize, lSize);
		lManager->GetGlobal(cDescTextScale, lScale);
		mDescText.SetTexture(mTexManager, cDescTextID);
		mDescText.SetPosition({ lPosition[0],lPosition[1] });
		mDescText.SetSize({ lSize[0], lSize[1] });
		mDescText.SetScale({ lScale[0], lScale[1] });

		ClearTemp();
	}

	//ステージリストフレーム
	{
		lManager->GetGlobal(cStageListFramePosition, lPosition);
		lManager->GetGlobal(cStageListFrameSize, lSize);
		lManager->GetGlobal(cStageListFrameScale, lScale);
		//ListFrame
	/*	mBackground.SetTexture(mTexManager, cBackgroundID);
		mBackground.SetPosition({ lPosition[0],lPosition[1] });
		mBackground.SetSize({ lSize[0], lSize[1] });
		mBackground.SetScale({ lScale[0], lScale[1] });
*/
		ClearTemp();

		std::vector<std::string>lStageList;
		lManager->GetGlobal(cStagePathList, lStageList);

		mSelectList.Initialize(&mLuaDb, lManager, &mTexManager, lStageList, cStagePathOffsetID);

	}


}

void SceneStageSelect::InitSound()
{
	using namespace ValueSS::Sound;
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

void SceneStageSelect::StageSelectSave()
{
	std::stringstream lSaveStr;
	auto lStageData = mSelectList.GetActiveData();
	lSaveStr << lStageData->mAIMapFileName << std::endl;
	lSaveStr << lStageData->mBlackSpawnMapFileName << std::endl;
	lSaveStr << lStageData->mWhiteSpawnMapFileName << std::endl;
	lSaveStr << lStageData->mBallSpawnMapFileName << std::endl;
	lSaveStr << lStageData->mModelScriptName;
	std::ofstream lOutFile;

	lOutFile.open("Temp/Stage.dat");
	if (!lOutFile) {
		_mkdir("Temp");
		lOutFile.open("Temp/Stage.dat");
	}
	std::string lOutStr = lSaveStr.str();
	std::replace(lOutStr.begin(), lOutStr.end(), ' ', (char)-127);

	lOutFile << lOutStr;
	lOutFile.close();
}

void SceneStageSelect::UpdateSceneChange()
{
	if (mIsSceneChange == false)return;

	switch (mSequence)
	{
	case SceneSequence::eChangeFirst:
		mSceneTime = clock();
		mSequence = SceneSequence::eChangeLoop;
		{
			
			tween::TweenerParam lParam(2000, tween::EASE_IN_OUT, tween::EASE_IN);
			lParam.addProperty(&mSelectTitle.mPosition.y, -100);
			mTweener.addTween(lParam);
		}
		//フォールスルー
	case SceneSequence::eChangeLoop:
	{
		auto lNowTime = clock();
		if (mIsMoveThumbnail == false) {
			if (mSceneTime + 700 < lNowTime) {
				tween::TweenerParam lParam(2000, tween::EASE_IN, tween::EASE_IN);
				lParam.addProperty(&mSelectList.GetActiveThumbnail()->mPosition.x, 1050);
				mIsMoveThumbnail = true;
			}
			mTweener.step(lNowTime);

		}
		else if (mSceneTime + 2800 < lNowTime) {
			mSequence = SceneSequence::eChangeEnd;
		}
		mTweener.step(lNowTime);



	}

	break;
	case SceneSequence::eChangeEnd:
		mScneThread.Wait();
		MSDirect::SetScene(std::move(mScene));
		break;
	default:
		break;
	}
}
