#include "StageSelect.h"
#include"MSDirect.h"
void SceneStageSelect::Initialize()
{
	InitShader();
	InitUI();
}

void SceneStageSelect::Update()
{
}

void SceneStageSelect::KeyDown(MSKEY pKey)
{
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
