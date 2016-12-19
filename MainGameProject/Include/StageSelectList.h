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
	//���f���f�[�^�̏�񂪓����Ă���X�N���v�g�t�@�C���p�X
	std::string mModelScriptName;

	//AI�}�b�v�t�@�C���p�X
	std::string mAIMapFileName;
	//FileName is spawn map for BlackTeam.
	std::string mBlackSpawnMapFileName;
	//FileName is spawn map for WhiteTeam.
	std::string mWhiteSpawnMapFileName;
	//FileName is spawn map for Ball.
	std::string mBallSpawnMapFileName;
	//mThumbnail is Stage Thumbnail Image.
	MSSprite2DResource mThumbnail;
};



namespace ValueStageData {

	static const char* cButtonNormalFilePath = "ButtonNormalFilePath";
	static const char* cButtonActiveFilePath = "ButtonActiveFilePath";
	static const char* cButtonPushFilePath = "ButtonPushFilePath";
	static const char* cButtonDisableFilePath = "ButtonDisableFilePath";
	static const char* cStageScriptFilePath = "StageScriptFilePath";
	static const char* cModelScriptFilePath = "ModelScriptFilePath";
	static const char* cAIMapFilePath = "AIMapFilePath";
	static const char* cBlackSpawnMapFilePath = "BlackSpawnMapFilePath";
	static const char* cWhiteSpawnMapFilePath = "WhiteSpawnMapFilePath";
	static const char* cBallSpawnMapFilePath = "BallSpawnMapFilePath";
	static const char* cThumnailFilePath = "ThumnailFilePath";
	static const int cThumnailID = 1000;
	static const int cButtonNormalID = 1100;
	static const int cButtonActiveID = 1200;
	static const int cButtonPushID = 1300;
	static const int cButtonDisableID= 1400;

}
	struct Resource3D {
		std::vector<float>lPosition;
		std::vector<float>lSize;
		std::vector<float>lScale;
		std::vector<float>lOffset;
	};
class StageSelectList :public UIBase {
public:
	//�X�e�[�W�Z���N�g�t�@�C���̓ǂݍ���
	void Initialize(NcgLuaDatabase*aLuaDb, NcgLuaManager* aSelectManager,DX11TextureManager*aTexManager, const std::vector<std::string>&mStageFiles,const int aIDOffset);
	//�{�^������߂�
	void ActiveBack();
	//�{�^������i�߂�
	void ActiveNext();
	//�{�^��������
	void PushButton();
	//�{�^���𗣂�
	void PopButton();
	//�A�N�e�B�u�f�[�^�̎Q�Ƃ��擾����
	StageData* GetActiveData();

	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
	MSSprite2DResource* GetActiveThumbnail();

	Resource3D* GetResThumbnail() { return &ButtonsResource; }
	Resource3D* GetResToggle() { return &ThumbnailResource; }

private:

	void SetActiveThumbnail();


	//���X�g�\������t���[���̉摜
	MSSprite2DResource mListFrame;
	//�X�e�[�W�f�[�^�Q
	std::vector<StageData>mStages;
	//�T���l�C��
	std::vector<MSSprite2DResource>mThumnails;
	//�A�N�e�B�u�ȃT���l�C��
	MSSprite2DResource* mActiveThumbnail;

	//�X�e�[�W���X�g
	std::vector<Button>mButtons;
	Toggle mstageToggle;
	Resource3D ButtonsResource;
	Resource3D ThumbnailResource;
	Resource3D FrameResource;

};