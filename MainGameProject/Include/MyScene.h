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
namespace ValueMyScene {

	//Effect50~99
	namespace Effect {
		static const int cShotID = 50;
		static const int cHitID = 51;
		static const int cGoalInID = 52;
		static const int cKillID = 53;
	}

	//UI値:100~499
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

		static const char* cStatusFramePositionName = "StatusFramePosition";
		static const char* cStatusFrameSize = "StatusFrameSize";
		static const char* cStatusFrameScale = "StatusFrameScale";
		static const char* cStatusFrameTexturePath = "StatusFrameTexturePath";

		static const char* cHPBarPosition = "HPBarPosition";
		static const char* cHPBarSize = "HPBarSize";
		static const char* cHPBarInScale = "HPBarInScale";
		static const char* cHPBarOutTexturePath = "HPBarOutTexturePath";
		static const char* cHPBarInTexturePath = "HPBarInTexturePath";

		static const char* cEPBarPosition = "EPBarPosition";
		static const char* cEPBarSize = "EPBarSize";
		static const char* cEPBarInScale = "EPBarInScale";
		static const char* cEPBarOutTexturePath = "EPBarOutTexturePath";
		static const char* cEPBarInTexturePath = "EPBarInTexturePath";

		static const char* cScoreBarPosition = "ScoreBarPosition";
		static const char* cScoreBarSize = "ScoreBarSize";
		static const char* cScoreBarInScale = "ScoreBarInScale";
		static const char* cScoreBarOutTexturScoreath = "ScoreBarOutTexturePath";
		static const char* cScoreBarLeftTexturScoreath = "ScoreBarLeftTexturePath";
		static const char* cScoreBarRightTexturScoreath = "ScoreBarRightTexturePath";

		//テキストオフセット
		static const char* cHPTextOffset = "HPTextOffset";
		static const char* cEPTextOffset = "EPTextOffset";
		static const char* cScoreTextOffset = "ScoreTextOffset";

		static const char* cTimeTextPosition = "TimeTextPosition";

	}


	//Modelオフセット値500~999
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


	//CharaBase値:1500~1999
	namespace Chara {
		static const int cStatusID = 1500;

		//例外(FBX Animation ID)
		static const int cAnimIdle = 0;
		static const int cAnimRun = 1;
		static const int cAnimWalk = 2;
		static const int cAnimSkill = 3;
		static const int cAnimAttack = 4;
		static const int cAnimAttacked = 5;


	}

	//Result値:2000~2499
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
	void Render()override;
	void Destroy() override {}

private:
	//フォントの初期化
	void InitializeFont();
	//UIの初期化
	void InitializeUI();
	//Effectの初期化
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

	void UpdateUI();
	//タイムオーバー時の処理
	void UpdateTimeOver();

private:

	MSFbxDatabase mdDB;
	//描画に使うシェーダ
	My3DShader shader;
	//シーンに一つ
	MS3DRender render;

	//MSCollisionRayPicking lRayPick;


	std::vector<unique_ptr<Enemy>> enemy;
	Player mPlayer;
	StaticObject mField;
	Ball mBall;
	StatusField mFieldStatus;

	NcgLuaDatabase mLuaDb;

	DX11TextureManager mTexManager;

	//2Dスプライト関係
	MySprite2DShader m2DShader;
	MSSprite2DRender m2DRender;

	//UI
	MySceneUI ui;

	//テキストマネージャ
	TextManager textMan;

	BulletManager bltManager;
	std::vector<std::unique_ptr<NBullet>> blts;

	//タイムオーバー処理クラス
	MySceneTimeOver mTimeOver;
	//リザルト処理クラス
	MySceneResult mResult;

	//Effekseer関係
	::Comfort::EfkRenderer mEfkRender;
	::Comfort::EfkManager mEfkManager;
	::Comfort::EffectDatabase mEfkDb;
	::Comfort::EfkObject mEfkObj;

	//ID,拡大率
	std::unordered_map<int, float>mFbxScaleMap;

	//デフォルトステータス
	StatusBase mDefaultStatus;

	//シーンカウンタ
	int mSceneCounter;
	//時間制限が来たかどうか
	bool mIsTimeOver;
};