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
	//Script�o�^
	int lIdCount = 0;
	for (auto ScriptPath : mStageFiles) {
		aLuaDb->Load(ScriptPath, aIDOffset + lIdCount);
		lManagers.push_back(aLuaDb->GetManager(aIDOffset+lIdCount));
		++lIdCount;
	}

	std::unordered_map<int, std::string>lFileNameMap;
	std::string lFileName;

	//���X�g�t���[��
	{
		using namespace ValueSS::UI;
		Resource3D lRes;
		std::string lFileName;

		aSelectManager->GetGlobal(cStageListFramePosition, lRes.lPosition);
		aSelectManager->GetGlobal(cStageListFrameSize, lRes.lSize);
		aSelectManager->GetGlobal(cStageListFrameScale, lRes.lScale);
		aSelectManager->GetGlobal(cStageListFrameTexturePath, lFileName);

		mListFrame.SetTexture(*aTexManager, cStageListFrameID);
		mListFrame.SetPosition({ lRes.lPosition[0], lRes.lPosition[1] });
		mListFrame.SetSize({lRes.lSize[0], lRes.lSize[1]});
		mListFrame.SetScale({ lRes.lScale[0],lRes.lScale[1] });


	}



	/*
		�{�^���A�T���l�C���A�X�e�[�W���X�g�t���[��
		���ꂼ��̍��W�A�T�C�Y�Ȃǂ�ǂݍ���
	*/
	{
		using namespace ValueSS::UI;
		aSelectManager->GetGlobal(cButtonPosition, ButtonsResource.lPosition);
		aSelectManager->GetGlobal(cButtonSize, ButtonsResource.lSize);
		aSelectManager->GetGlobal(cButtonScale, ButtonsResource.lScale);
		aSelectManager->GetGlobal(cButtonOffset, ButtonsResource.lOffset);

		aSelectManager->GetGlobal(cThumnailPosition, ThumbnailResource.lPosition);
		aSelectManager->GetGlobal(cThumnailSize, ThumbnailResource.lSize);
		aSelectManager->GetGlobal(cThumnailScale, ThumbnailResource.lScale);

		aSelectManager->GetGlobal(cStageListFramePosition, FrameResource.lPosition);
		aSelectManager->GetGlobal(cStageListFrameSize, FrameResource.lSize);
		aSelectManager->GetGlobal(cStageListFrameScale, FrameResource.lScale);


	}
	//�g�O���̏�����
	mstageToggle.SetGlobalPosition(ButtonsResource.lPosition[0], ButtonsResource.lPosition[1]);
	mstageToggle.SetPadding(ButtonsResource.lOffset[0], ButtonsResource.lOffset[1]);


	int lMapIDOffset = 0;
	for (auto lManager : lManagers) {
		//�t�@�C���p�X�}�b�v�̏�����
		lFileNameMap.clear();


		StageData lStage;
		using namespace ValueStageData;
		//�t�@�C�����ꊇ�o�^
		lManager->GetGlobal(cModelScriptFilePath, lStage.mModelScriptName);
		lManager->GetGlobal(cAIMapFilePath, lStage.mAIMapFileName);
		lManager->GetGlobal(cBlackSpawnMapFilePath, lStage.mBlackSpawnMapFileName);
		lManager->GetGlobal(cWhiteSpawnMapFilePath, lStage.mWhiteSpawnMapFileName);
		lManager->GetGlobal(cBallSpawnMapFilePath, lStage.mBallSpawnMapFileName);
		//�e�N�X�`���ꊇ�o�^
		lManager->GetGlobal(cButtonNormalFilePath, lFileName);
		lFileNameMap[cButtonNormalID + lMapIDOffset] = lFileName;
		lManager->GetGlobal(cButtonActiveFilePath, lFileName);
		lFileNameMap[cButtonActiveID + lMapIDOffset] = lFileName;
		lManager->GetGlobal(cButtonPushFilePath, lFileName);
		lFileNameMap[cButtonPushID + lMapIDOffset] = lFileName;
		lManager->GetGlobal(cButtonDisableFilePath, lFileName);
		lFileNameMap[cButtonDisableID + lMapIDOffset] = lFileName;

		lManager->GetGlobal(cThumnailFilePath, lFileName);
		lFileNameMap[cThumnailID + lMapIDOffset] = lFileName;
		aTexManager->RegisterFileList(lFileNameMap);



		lStage.mThumbnail.SetTexture(*aTexManager, cThumnailID + lMapIDOffset);
		lStage.mThumbnail.SetSize({ ThumbnailResource.lSize[0], ThumbnailResource.lSize[1] });
		lStage.mThumbnail.SetPosition({ThumbnailResource.lPosition[0], ThumbnailResource.lPosition[1]});
		mStages.push_back(std::move(lStage));
		//�{�^���̍쐬
		Button lButton;
		lButton.SetTextures(aTexManager, cButtonNormalID + lMapIDOffset, cButtonActiveID + lMapIDOffset, cButtonPushID + lMapIDOffset, cButtonDisableID + lMapIDOffset);
		lButton.SetSize(ButtonsResource.lSize[0], ButtonsResource.lSize[1]);
		mButtons.push_back(std::move(lButton));

		++lMapIDOffset;

	}

	for (auto&lButton : mButtons) {
		mstageToggle.AddButton(&lButton);
	}

	/*
		�����F
		�{�^��4��̑Ή�
		�X�e�[�W�I����A�Q�[���v���C��ʂֈڍs������
	*/

	SetActiveThumbnail();

}

void StageSelectList::ActiveBack()
{
	mstageToggle.ActiveBack();
}

void StageSelectList::ActiveNext()
{
	mstageToggle.ActiveNext();
}

void StageSelectList::PushButton()
{
	mstageToggle.PushButton();
}

void StageSelectList::PopButton()
{
	mstageToggle.PopButton();
}

StageData * StageSelectList::GetActiveData()
{
	int lIndex = mstageToggle.GetActiveIndex();
	return &mStages[lIndex];
}

void StageSelectList::Render(MSSprite2DRender & aRender, UIBase * aParent)
{
	//aRender.Render(mListFrame);
	if (mActiveThumbnail != nullptr) {
		aRender.Render(*mActiveThumbnail);
	}
	mstageToggle.Render(aRender, this);
}

void StageSelectList::SetActiveThumbnail()
{
	int lIndex = mstageToggle.GetActiveIndex();
	mActiveThumbnail = &mStages[lIndex].mThumbnail;
	mActiveThumbnail->SetPosition({ ThumbnailResource.lPosition[0], ThumbnailResource.lPosition[1] });
}
