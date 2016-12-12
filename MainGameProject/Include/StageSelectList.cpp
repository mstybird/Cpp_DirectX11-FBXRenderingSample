#include "StageSelectList.h"
#include"StageSelect.h"
#include"Button.h"
#include<NcgLua.hpp>
#include<DX11TextureManager.hpp>
void StageSelectList::Initialize(
	NcgLuaDatabase * aLuaDb,
	NcgLuaManager* aSelectManager, 
	DX11TextureManager*aTexManager, 
	const std::vector<std::string>&mStageFiles, 
	const int aIDOffset)
{
	std::vector<NcgLuaManager*>lManagers;
	//Script“o˜^
	int lIdCount = 0;
	for (auto ScriptPath : mStageFiles) {
		aLuaDb->Load(ScriptPath, aIDOffset + lIdCount);
		lManagers.push_back(aLuaDb->GetManager(aIDOffset+lIdCount));
		++lIdCount;
	}

	std::unordered_map<int, std::string>lFileNameMap;
	std::string lFileName;

	struct ValueBuffer {
		std::vector<float>lPosition;
		std::vector<float>lSize;
		std::vector<float>lScale;
	};

	ValueBuffer lFrame;

	for (auto lManager : lManagers) {
		StageData lStage;
		using namespace ValueStageData;
		//ƒtƒ@ƒCƒ‹–¼ˆêŠ‡“o˜^
		lManager->GetGlobal(cStageScriptFilePath, lStage.mStageScriptName);
		lManager->GetGlobal(cModelScriptFilePath, lStage.mModelScriptName);
		lManager->GetGlobal(cAIMapFilePath, lStage.mAIMapFileName);
		lManager->GetGlobal(cBlackSpawnMapFilePath, lStage.mBlackSpawnMapFileName);
		lManager->GetGlobal(cWhiteSpawnMapFilePath, lStage.mWhiteSpawnMapFileName);
		lManager->GetGlobal(cBallSpawnMapFilePath, lStage.mBallSpawnMapFileName);
		//ƒeƒNƒXƒ`ƒƒˆêŠ‡“o˜^
		lManager->GetGlobal(cTitleFilePath, lFileName);
		lFileNameMap[cTitleID] = lFileName;
		lManager->GetGlobal(cThumnailFilePath, lFileName);
		lFileNameMap[cThumnailID] = lFileName;
		aTexManager->RegisterFileList(lFileNameMap);

		lStage.mStageTitle


	}



}

void StageSelectList::Render(MSSprite2DRender & aRender, UIBase * aParent)
{
}
