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
	InitUI();
	
}

void SceneStageSelect::Update()
{
	mSelectTitle.AddRotation(1.0f);
}

void SceneStageSelect::KeyDown(MSKEY pKey)
{
	switch (pKey)
	{
	case MSKEY::UP:
		mSelectList.ActiveBack();
		break;
	case MSKEY::DOWN:
		mSelectList.ActiveNext();
		break;
	case MSKEY::ENTER:
		StageSelectSave();
		MSDirect::SetScene(std::make_unique<MyMSScene>());

		break;
	case MSKEY::BACKSPACE:
		MSDirect::SetScene(std::make_unique<SceneTitle>());
		break;
	default:
		break;
	}
}

void SceneStageSelect::KeyHold(MSKEY pKey)
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
	using namespace ValueSS;
	mLuaDb.Load(cLuaFilePath, cLuaID);
	auto lManager = mLuaDb.GetManager(cLuaFilePath);
	//
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

		mTexManager.RegisterFileList(lFileNameMap);
	}


	std::vector<float>lPosition;
	std::vector<float>lSize;
	std::vector<float>lScale;

	//����p
	auto ClearTemp = [&]() {
		lPosition.clear();
		lSize.clear();
		lScale.clear();
	};

	//�w�i
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


	//�Z���N�g�^�C�g��
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

	//��������e�L�X�g
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

	//�X�e�[�W���X�g�t���[��
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
