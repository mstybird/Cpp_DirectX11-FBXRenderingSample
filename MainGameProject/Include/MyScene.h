#pragma once
#include<memory>
#include<iostream>
#include<vector>
#include"MSwrap.h"
#include"IncludeForward.h"
#include"Enemy.h"
#include"Player.h"
#include"StaticObject.h"
#include"Ball.h"
#include"StatusField.h"
#include<NcgLua.hpp>
#include"Effect.hxx"
#include"MSFbxDatabase.h"
#include"StatusFrame.h"
#include"BarGaugeVS.h"
#include"MySceneUI.h"
#include"Graphic_Text.h"
#include"BulletManager.h"
#include<DX11TextureManager.hpp>
#include"TimeOver.h"
#include"Result.h"
#include"StageSelectList.h"
#include"3DCollisionShader.h"
#include<DXAL.hpp>


namespace ValueMyScene {
	
	//Effect50~99
	namespace Effect {
		static const int cShotID = 50;
		static const int cHitID = 51;
		static const int cGoalInID = 52;
		static const int cKillID = 53;
	}

	//UI�l:100~499
	namespace UI {

		static const int cUILuaID = 100;

		static const int cStatusFrameID = 100;

		static const int cHPBarOutID = 110;
		static const int cHPBarInID = 111;

		static const int cEPBarOutID = 120;
		static const int cEPBarInID = 121;

		static const int cScoreBarOutID = 130;
		static const int cScoreBarLeftID = 131;
		static const int cScoreBarRightID = 132;

		static const int cLogoFinishID = 140;
		static const int cLogoWinID = 141;
		static const int cLogoLoseID = 142;
		static const int cLogoDrawID = 143;

		static const int cLogoReadyID = 200;
		static const int cLogoStartID = 210;

		//�X�e�[�^�X�t���[��
		static const char* cStatusFramePositionName = "StatusFramePosition";
		static const char* cStatusFrameSize = "StatusFrameSize";
		static const char* cStatusFrameScale = "StatusFrameScale";
		static const char* cStatusFrameTexturePath = "StatusFrameTexturePath";

		//HP�o�[
		static const char* cHPBarPosition = "HPBarPosition";
		static const char* cHPBarSize = "HPBarSize";
		static const char* cHPBarInScale = "HPBarInScale";
		static const char* cHPBarOutTexturePath = "HPBarOutTexturePath";
		static const char* cHPBarInTexturePath = "HPBarInTexturePath";

		//EP�o�[
		static const char* cEPBarPosition = "EPBarPosition";
		static const char* cEPBarSize = "EPBarSize";
		static const char* cEPBarInScale = "EPBarInScale";
		static const char* cEPBarOutTexturePath = "EPBarOutTexturePath";
		static const char* cEPBarInTexturePath = "EPBarInTexturePath";

		//�X�R�A�o�[
		static const char* cScoreBarPosition = "ScoreBarPosition";
		static const char* cScoreBarSize = "ScoreBarSize";
		static const char* cScoreBarInScale = "ScoreBarInScale";
		static const char* cScoreBarOutTexturScoreath = "ScoreBarOutTexturePath";
		static const char* cScoreBarLeftTexturScoreath = "ScoreBarLeftTexturePath";
		static const char* cScoreBarRightTexturScoreath = "ScoreBarRightTexturePath";

		//�e�L�X�g�I�t�Z�b�g
		static const char* cHPTextOffset = "HPTextOffset";
		static const char* cEPTextOffset = "EPTextOffset";
		static const char* cScoreTextOffset = "ScoreTextOffset";
		static const char* cTimeTextPosition = "TimeTextPosition";

		//�J�n�O���S(Ready)
		static const char* cLogoReadyPosition = "LogoReadyPosition";
		static const char* cLogoReadySize = "LogoReadySize";
		static const char* cLogoReadyScale = "LogoReadyScale";
		static const char* cLogoReadyTexturePath = "LogoReadyTexturePath";

		//�J�n�O���S(Start)
		static const char* cLogoStartPosition = "LogoStartPosition";
		static const char* cLogoStartSize = "LogoStartSize";
		static const char* cLogoStartScale = "LogoStartScale";
		static const char* cLogoStartTexturePath = "LogoStartTexturePath";


	}


	//Model�I�t�Z�b�g�l500~999
	namespace Model {
		static const int cLuaID = 500;

		static const int cBulletDesignID = 510;
		static const int cBulletCollisionID = 511;

		static const int cCharaBlackDesignID = 520;
		static const int cCharaBlackCollisionID = 521;

		static const int cBaseBlackDesignID = 522;
		static const int cBaseBlackCollisionID = 523;

		static const int cCharaWhiteDesignID = 530;
		static const int cCharaWhiteCollisionID = 531;

		static const int cBaseWhiteDesignID = 532;
		static const int cBaseWhiteCollisionID = 533;

		static const int cFieldDesignID = 540;
		static const int cFieldCollisionID = 541;

		static const int cBallDesignID = 550;
		static const int cBallCollisionID = 551;

		static const char* cBulletDesignModel = "BulletDesignModel";
		static const char* cBulletDesignScale = "BulletDesignScale";
		static const char* cBulletDesignAnimation = "BulletDesignAnimation";
		static const char* cBulletCollisionModel = "BulletCollisionModel";
		static const char* cBulletCollisionScale = "BulletCollisionScale";
		static const char* cBulletCollisionAnimation = "BulletCollisionAnimation";

		static const char* cCharaBlackDesignModel = "CharaBlackDesignModel";
		static const char* cCharaBlackDesignScale = "CharaBlackDesignScale";
		static const char* cCharaBlackDesignAnimation = "CharaBlackDesignAnimation";
		static const char* cCharaBlackCollisionModel = "CharaBlackCollisionModel";
		static const char* cCharaBlackCollisionScale = "CharaBlackCollisionScale";
		static const char* cCharaBlackCollisionAnimation = "CharaBlackCollisionAnimation";

		static const char* cBaseBlackDesignModel = "BaseBlackDesignModel";
		static const char* cBaseBlackDesignScale = "BaseBlackDesignScale";
		static const char* cBaseBlackDesignAnimation = "BaseBlackDesignAnimation";
		static const char* cBaseBlackCollisionModel = "BaseBlackCollisionModel";
		static const char* cBaseBlackCollisionScale = "BaseBlackCollisionScale";
		static const char* cBaseBlackCollisionAnimation = "BaseBlackCollisionAnimation";


		static const char* cCharaWhiteDesignModel = "CharaWhiteDesignModel";
		static const char* cCharaWhiteDesignScale = "CharaWhiteDesignScale";
		static const char* cCharaWhiteDesignAnimation = "CharaWhiteDesignAnimation";
		static const char* cCharaWhiteCollisionModel = "CharaWhiteCollisionModel";
		static const char* cCharaWhiteCollisionScale = "CharaWhiteCollisionScale";
		static const char* cCharaWhiteCollisionAnimation = "CharaWhiteCollisionAnimation";

		static const char* cBaseWhiteDesignModel = "BaseWhiteDesignModel";
		static const char* cBaseWhiteDesignScale = "BaseWhiteDesignScale";
		static const char* cBaseWhiteDesignAnimation = "BaseWhiteDesignAnimation";
		static const char* cBaseWhiteCollisionModel = "BaseWhiteCollisionModel";
		static const char* cBaseWhiteCollisionScale = "BaseWhiteCollisionScale";
		static const char* cBaseWhiteCollisionAnimation = "BaseWhiteCollisionAnimation";

		static const char* cFieldDesignModel = "FieldDesignModel";
		static const char* cFieldDesignScale = "FieldDesignScale";
		static const char* cFieldDesignAnimation = "FieldDesignAnimation";
		static const char* cFieldCollisionModel = "FieldCollisionModel";
		static const char* cFieldCollisionScale = "FieldCollisionScale";
		static const char* cFieldCollisionAnimation = "FieldCollisionAnimation";

		static const char* cBallDesignModel = "BallDesignModel";
		static const char* cBallDesignScale = "BallDesignScale";
		static const char* cBallDesignAnimation = "BallDesignAnimation";
		static const char* cBallCollisionModel = "BallCollisionModel";
		static const char* cBallCollisionScale = "BallCollisionScale";
		static const char* cBallCollisionAnimation = "BallCollisionAnimation";

	}

	namespace TimeOver {
		static const char* cLogoFinishPosition = "LogoFinishPosition";
		static const char* cLogoFinishSize = "LogoFinishSize";
		static const char* cLogoFinishScale = "LogoFinishScale";
		static const char* cLogoFinishTexturePath = "LogoFinishTexturePath";

		static const char* cLogoIssuePosition = "LogoIssuePosition";
		static const char* cLogoIssueSize = "LogoIssueSize";
		static const char* cLogoIssueScale = "LogoIssueScale";
		static const char* cLogoIssueWinTexturePath = "LogoIssueWinTexturePath";
		static const char* cLogoIssueLoseTexturePath = "LogoIssueLoseTexturePath";
		static const char* cLogoIssueDrawTexturePath = "LogoIssueDrawTexturePath";

	}


	//CharaBase�l:1500~1999
	namespace Chara {
		static const int cStatusID = 1500;

		//��O(FBX Animation ID)
		static const int cAnimIdle = 0;
		static const int cAnimRun = 1;
		static const int cAnimWalk = 2;
		static const int cAnimSkill = 3;
		static const int cAnimAttack = 4;
		static const int cAnimAttacked = 5;
		static const int cAnimDown = 6;


	}

	//Result�l:2000~2499
	namespace Result {
		static const int cLuaID = 2000;

		//TextureIDs
		static const int cFrameID = 2100;

		static const int cLogoWinID = 2200;
		static const int cLogoLoseID = 2201;
		static const int cLogoDrawID = 2202;

		static const int cButtonRetryNormalID = 2300;
		static const int cButtonRetryActiveID = 2301;
		static const int cButtonRetryPushID = 2302;
		static const int cButtonRetryDisableID = 2303;

		static const int cButtonToTitleNormalID = 2310;
		static const int cButtonToTitleActiveID = 2311;
		static const int cButtonToTitlePushID = 2312;
		static const int cButtonToTitleDisableID = 2313;

		static const char* cFramePosition = "FramePosition";
		static const char* cFrameSize = "FrameSize";
		static const char* cFrameScale = "FrameScale";
		static const char* cFrameTexturePath = "FrameTexturePath";

		static const char* cLogoPosition = "LogoPosition";
		static const char* cLogoSize = "LogoSize";
		static const char* cLogoScale = "LogoScale";
		static const char* cLogoWinTexturePath = "LogoWinTexturePath";
		static const char* cLogoLoseTexturePath = "LogoLoseTexturePath";
		static const char* cLogoDrawTexturePath = "LogoDrawTexturePath";


		static const char* cTextPosition = "TextPosition";

		static const char* cTogglePosition = "TogglePosition";

		static const char* cButtonRetrySize = "ButtonRetrySize";
		static const char* cButtonRetryScale = "ButtonRetryScale";
		static const char* cButtonRetryNormalTexturePath = "ButtonRetryNormalTexturePath";
		static const char* cButtonRetryActiveTexturePath = "ButtonRetryActiveTexturePath";
		static const char* cButtonRetryPushTexturePath = "ButtonRetryPushTexturePath";
		static const char* cButtonRetryDisableTexturePath = "ButtonRetryDisableTexturePath";

		static const char* cButtonToTitleSize = "ButtonToTitleSize";
		static const char* cButtonToTitleScale = "ButtonToTitleScale";
		static const char* cButtonToTitleNormalTexturePath = "ButtonToTitleNormalTexturePath";
		static const char* cButtonToTitleActiveTexturePath = "ButtonToTitleActiveTexturePath";
		static const char* cButtonToTitlePushTexturePath = "ButtonToTitlePushTexturePath";
		static const char* cButtonToTitleDisableTexturePath = "ButtonToTitleDisableTexturePath";



	}
	//ID:2500~2999
	namespace Sound {
		static const int cLuaID = 2500;

		static const char* cLuaPath = "Resource/Script/GamePlaySound.lua";
		static const char* cBGMPath = "BGMPath";

	}

}


class MyMSScene :public MSSceneBase {
public:

	MyMSScene();
	~MyMSScene();
private:
	void Initialize()override;
	void Update()override;
	void KeyDown(MSKEY pKey)override;
	void KeyHold(MSKEY pKey)override;
	void KeyUp(MSKEY pKey)override;
	void MouseMove(const POINT & aNowPosition, const POINT & aDiffPosition)override;
	virtual void MouseDown(const MouseType aType)override;
	virtual void MouseUp(const MouseType aType)override;
	void Render()override;
	void Destroy() override {}

private:
	//�t�H���g�̏�����
	void InitializeFont();
	//�X�e�[�W�f�[�^���̓ǂݍ���
	void InitializeStageData();
	//�T�E���h�̏�����
	void InitializeSound();
	//UI�̏�����
	void InitializeUI();
	//Effect�̏�����
	void InitializeEffect();
	void InitializeModel();
	void InitializeFieldStatus();
	void InitializeBulletManager();
	void InitializeCharaFirst();
	void InitializeEnemy();
	void InitializePlayer();
	void InitializeBall();
	void InitializeTimeOver();
	void InitializeResult();
	void SetMeshResouce(GameObjectBase*aObject, const int aDesignID, const int aCollisionID);
	StatusBulletBase LoadBulletStatus(const std::string& aFileName, const int aBulletID);

	//�Q�[���J�n�O�̏���
	void UpdateReady();

	//UI�̍X�V
	void UpdateUI();
	//�^�C���I�[�o�[���̏���
	void UpdateTimeOver();

private:

	MSFbxDatabase mdDB;
	//�`��Ɏg���V�F�[�_
	My3DShader shader;
	//�J�����O�Ɏg���V�F�[�_
	Collision3DShader mCollisionShader;
	//�V�[���Ɉ��
	MS3DRender render;

	//MSCollisionRayPicking lRayPick;


	std::vector<unique_ptr<Enemy>> enemy;
	Player mPlayer;
	StaticObject mField;
	Ball mBall;
	StatusField mFieldStatus;

	NcgLuaDatabase mLuaDb;

	DX11TextureManager mTexManager;

	//2D�X�v���C�g�֌W
	MySprite2DShader m2DShader;
	MSSprite2DRender m2DRender;

	//UI
	MySceneUI ui;

	//�e�L�X�g�}�l�[�W��
	TextManager textMan;

	BulletManager bltManager;
	std::vector<std::unique_ptr<BulletObjectBase>> blts;

	//�^�C���I�[�o�[�����N���X
	MySceneTimeOver mTimeOver;
	//���U���g�����N���X
	MySceneResult mResult;

	//Effekseer�֌W
	::Comfort::EfkRenderer mEfkRender;
	::Comfort::EfkManager mEfkManager;
	::Comfort::EffectDatabase mEfkDb;
	::Comfort::EfkObject mEfkObj;

	//ID,�g�嗦
	std::unordered_map<int, float>mFbxScaleMap;

	//�f�t�H���g�X�e�[�^�X
	StatusBase mDefaultStatus;

	//�V�[���J�E���^
	int mSceneCounter;
	//���Ԑ������������ǂ���
	bool mIsTimeOver;
	//�Q�[�����n�܂�O�Ȃ�true
	bool mIsReady;
	//�X�e�[�W�f�[�^�Q
	StageData mStageData;

	//ReadyLogo
	MSSprite2DResource mLogoReady;
	MSSprite2DResource mLogoStart;

	SoundDevice mSoundDevice;
	SoundPlayer mBGM;
};