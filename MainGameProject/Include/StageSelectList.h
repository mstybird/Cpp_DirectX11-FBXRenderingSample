#pragma once
#include"UIBase.h"
#include"Toggle.h"
#include"MSSprite2DResource.h"
#include<string>
struct StatusBase;
class NcgLuaDatabase;
class NcgLuaManager;
class BarGauge;
class MSSprite2DRender;
class DX11TextureManager;
struct StageData {
	//ステージデータの入っているファイルのパス
	std::string mStageScriptName;
	//モデルデータの情報が入っているスクリプトファイルパス
	std::string mModelScriptName;
	//AIマップファイルパス
	std::string mAIMapFileName;
	//FileName is spawn map for BlackTeam.
	std::string mBlackSpawnMapFileName;
	//FileName is spawn map for WhiteTeam.
	std::string mWhiteSpawnMapFileName;
	//FileName is spawn map for Ball.
	std::string mBallSpawnMapFileName;
	//mTitleName is Stage Title Name.
	MSSprite2DResource mStageTitle;
	//mThumbnail is Stage Thumbnail Image.
	MSSprite2DResource mThumbnail;
};

/*
--StageListFrame
StageListFramePosition={13,13}
StageListFrameSize={300,140}
StageListFrameScale={1.0,1.0}

StageListFrameTexturePath="Resource/UI/StageSelect/StageTextList.png"

--Thumbnail
ThumbnailPosition={532,180}
ThumbnailSize={407,285}
ThumbnailScale={1.0,1.0}
*/

namespace ValueStageData {

	static const char* cTitleFilePath = "TitleFilePath";
	static const char* cStageScriptFilePath = "StageScriptFilePath";
	static const char* cModelScriptFilePath = "ModelScriptFilePath";
	static const char* cAIMapFilePath = "AIMapFilePath";
	static const char* cBlackSpawnMapFilePath = "BlackSpawnMapFilePath";
	static const char* cWhiteSpawnMapFilePath = "WhiteSpawnMapFilePath";
	static const char* cBallSpawnMapFilePath = "BallSpawnMapFilePath";
	static const char* cThumnailFilePath = "ThumnailFilePath";
	static const int cThumnailID = 100;
	static const int cTitleID= 110;

}

class StageSelectList :public UIBase {
public:
	//ステージセレクトファイルの読み込み
	void Initialize(NcgLuaDatabase*aLuaDb, NcgLuaManager* aSelectManager,DX11TextureManager*aTexManager, const std::vector<std::string>&mStageFiles,const int aIDOffset);
	void UpdateStatus(StatusBase* aStatus);


	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;

private:
	//リスト表示するフレームの画像
	MSSprite2DResource mListFrame;
	//ステージデータ群
	std::vector<StageData>mStages;
	//アクティブなサムネイル
	MSSprite2DResource* mActiveThumnail;

	//ステージリスト
	std::vector<Button>mButtons;
	Toggle mstageToggle;
};