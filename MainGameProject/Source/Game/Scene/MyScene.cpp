#include"MSUtility.h"
#include"BarGauge.h"
#include<ScoreView.h>
#include<TimeView.h>
#include"MyScene.h"
#include"Title.h"
/*
タスク：
攻撃を受けるとダメージ
弾を二種類用意
弾ごとにコストを設定
チャージ速度など

UI設計
*/
//ベクトル正規化

//const int ValueMyScene::UI::cUILuaID = 100;
const int cEnemyCount = 4;
MyMSScene::MyMSScene()
{
	mIsTimeOver = false;
	mSceneCounter = 0;
}

MyMSScene::~MyMSScene()
{
	mEfkDb.Release();
	mEfkManager.Release();
	//音声破棄はこのタイミング
	mEfkRender.Release();
}

void MyMSScene::Initialize()
{


	InitializeFont();
	InitializeModel();
	InitializeEffect();


	{
		//スプライト初期化
		m2DShader.Init();
		m2DShader.InitVertex("Resource/HLSL/Sprite2D.hlsl");
		m2DShader.InitPixel("Resource/HLSL/Sprite2D.hlsl");
		m2DRender.SetViewPort(MSDirect::GetViewPort());
		m2DRender.SetShader(m2DShader);


	}
	//AI読み込み
	mLuaDb.Load("Resource/AIPlanning/EnemyTask.lua", 0, "EnemyAI");

	shader.Init();
	//レンダラーシェーダーにはアドレスを記憶させるようにする
	mBall.SetRenderer(&render);
	mBall.SetShader(&shader);
	shader.InitVertex("Resource/HLSL/Simple.hlsl");
	shader.InitPixel("Resource/HLSL/Simple.hlsl");

	InitializeBall();
	InitializeCharaFirst();
	//敵数、プレイヤーを含むチーム分けも中で初期化している
	InitializeFieldStatus();
	InitializeBulletManager();
	InitializePlayer();
	InitializeEnemy();

	//UIの初期化
	InitializeUI();
	InitializeTimeOver();
	InitializeResult();

	//enemy.push_back(make_unique<Enemy>());


	mField.Initialize();
	SetMeshResouce(&mField, ValueMyScene::Model::cFieldDesignID, ValueMyScene::Model::cFieldCollisionID);
	mField.SetRenderer(&render);
	mField.SetShader(&shader);
	mField.GetWorld()->SetT(0, -1, 0);

	




	//バレットマネージャの初期化


		//リソースにオブジェクトを登録
	render.SetShader(&shader);
	render.SetRenderTarget(*mPlayer.GetTransform());

	//mPlayer.GetWorld()->SetS(scaleChara, scaleChara, scaleChara);
	mPlayer.GetView()->SetCamera(*mPlayer.GetWorld(), { 0.0f,6.6f,-10.0f });
	mPlayer.GetProj()->SetProjection(60, 0.01f, 100.0f);
	

	mPlayer.GetWorld()->AddRC({ 0,0,0 });


	::Comfort::EffectCamera cam;
	::Comfort::EffectProjection proj;
	cam.SetDXCamera(mPlayer.GetView());
	proj.SetDXProjection(mPlayer.GetProj());
	mEfkRender.SetCamera(&cam);
	mEfkRender.SetProjection(&proj);
	mEfkObj.Play();
	mFieldStatus.GameStart();
}

void MyMSScene::Update() {







	mEfkManager.Update();
	mField.Update();
	for (uint32_t i = 0; i < enemy.size(); ++i) {
		enemy[i]->Update();
	}

	mField.ClearTrans();
	mPlayer.Update();
	DXVector3 v;
	mPlayer.GetWorld()->GetMatrix().lock()->GetT(v);
	mEfkObj.SetPosition({ v.x,v.y,v.z });
	::Comfort::EffectCamera cam;
	cam.SetDXCamera(mPlayer.GetView());
	mEfkRender.SetCamera(&cam);
	
	UpdateUI();

	UpdateTimeOver();

	++mSceneCounter;
}

void MyMSScene::KeyDown(MSKEY pKey)
{
	static int count = 0;
	if (mIsTimeOver == false) {
		switch (pKey)
		{
		case MSKEY::CH_W:
			mPlayer.GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimRun);
			mPlayer.GetTransform()->GetMesh()->SetLoopFlag(true);
			break;
		case MSKEY::CH_S:
			mPlayer.GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimRun);
			mPlayer.GetTransform()->GetMesh()->SetLoopFlag(true);
			break;
		case MSKEY::CH_A:
			mPlayer.GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimRun);
			mPlayer.GetTransform()->GetMesh()->SetLoopFlag(true);
			break;
		case MSKEY::CH_D:
			mPlayer.GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimRun);
			mPlayer.GetTransform()->GetMesh()->SetLoopFlag(true);
			break;
		case MSKEY::SPACE:
			mPlayer.AddBullet();
			break;
		case MSKEY::ENTER:
		{
			DXVector3 Pos;
			mPlayer.GetWorld()->GetMatrix().lock()->GetT(Pos);
			printf("%.2f,%.2f,%.2f\n", Pos.x, Pos.y, Pos.z);
			++count;
		}
		break;
		default:
			break;
		}

	}
	else {
		if (!mTimeOver.IsUpdateing()) {

			switch (pKey) {
			
			case MSKEY::UP:
				mResult.ButtonBack();
				break;
			case MSKEY::DOWN:
				mResult.ButtonNext();
				break;
			case MSKEY::ENTER:
				mResult.ButtonPush();

				switch (mResult.GetButtonActiveIndex())
				{
				case 0:
					//リトライ
					MSDirect::SetScene(std::make_unique<MyMSScene>());
					break;
				case 1:
					//タイトルへ
					MSDirect::SetScene(std::make_unique<SceneTitle>());
					break;
				default:
					break;
				}

				break;
			}
		}
	}
}

void MyMSScene::KeyHold(MSKEY pKey)
{

	if (mIsTimeOver == true)return;
		DXVector3 data;
		float speed = 0.25f;
		switch (pKey)
		{
		case MSKEY::CH_0:
			render.SetRenderTarget(*mPlayer.GetTransform());
			break;
		case MSKEY::CH_1:
			render.SetRenderTarget(*enemy[0]->GetTransform());
			break;
		case MSKEY::CH_2:
			render.SetRenderTarget(*enemy[1]->GetTransform());
			break;
		case MSKEY::CH_3:
			render.SetRenderTarget(*enemy[2]->GetTransform());
			break;
		case MSKEY::CH_4:
			render.SetRenderTarget(*enemy[3]->GetTransform());
			break;
		case MSKEY::CH_W:
			mPlayer.GetWorld()->AddT(DXWorld::TYPE_ROTATE, speed, { 0,0,1 });
			break;
		case MSKEY::CH_S:
			mPlayer.GetWorld()->AddT(DXWorld::TYPE_ROTATE, speed, { 0,0,-1 });
			break;
		case MSKEY::CH_A:
			mPlayer.GetWorld()->AddT(DXWorld::TYPE_ROTATE, speed, { -1,0,0 });
			break;
		case MSKEY::CH_D:
			mPlayer.GetWorld()->AddT(DXWorld::TYPE_ROTATE, speed, { 1,0,0 });
			break;
		case MSKEY::LEFT:
			mPlayer.GetWorld()->AddRC({ 0.0f,-2.5f,0.0f });
			break;
		case MSKEY::RIGHT:
			mPlayer.GetWorld()->AddRC({ 0.0f,2.5f,0.0f });
			break;
		default:
			break;
		}



}

void MyMSScene::KeyUp(MSKEY pKey)
{
	switch (pKey) {
	case MSKEY::CH_W:
		mPlayer.GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimIdle);
		mPlayer.GetTransform()->GetMesh()->SetLoopFlag(true);
		break;
	case MSKEY::CH_S:
		mPlayer.GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimIdle);
		mPlayer.GetTransform()->GetMesh()->SetLoopFlag(true);
		break;
	case MSKEY::CH_A:
		mPlayer.GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimIdle);
		mPlayer.GetTransform()->GetMesh()->SetLoopFlag(true);
		break;
	case MSKEY::CH_D:
		mPlayer.GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimIdle);
		mPlayer.GetTransform()->GetMesh()->SetLoopFlag(true);
		break;
	}
}

void MyMSScene::Render()
{
	//text.Create("Hello", 0, 0, 720, 960, logFont);

	MS3DRender::Clear({ 0.2f,0.2f,0.2f,1 });
	//画面クリア
	for (uint32_t i = 0; i < enemy.size(); ++i) {
		enemy[i]->Render();
	}
	mPlayer.Render();
	mBall.Render();
	mField.Render();
	mEfkRender.RenderAll(&mEfkManager);

	mFieldStatus.GetTeamBase(eTeamType::White)->Render();
	mFieldStatus.GetTeamBase(eTeamType::Black)->Render();

	ui.Render(m2DRender);

	if (mIsTimeOver == true) {
		if (mTimeOver.IsUpdateing()) {
			mTimeOver.Render(m2DRender);

		}
		else {
			mResult.Render(m2DRender);
		}
	}



}

void MyMSScene::InitializeFont()
{
	//フォントの準備
	FontLog	logFont;
	::ZeroMemory(&logFont, sizeof(logFont));
//	logFont.lfHeight = 40;	//フォントサイズ
	logFont.SetFontSize(40);
	logFont.lfWidth = 0;
	logFont.lfEscapement = 0;
	logFont.lfOrientation = 0;
	logFont.lfWeight = FW_EXTRABOLD;
	logFont.lfItalic = 0;
	logFont.lfUnderline = 0;
	logFont.lfStrikeOut = 0;
	logFont.lfCharSet = SHIFTJIS_CHARSET;
	logFont.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logFont.lfQuality = PROOF_QUALITY;
	logFont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;

#ifdef UNICODE
	//wcscpy_sの第二引数は文字(配列)数(バッファサイズだとデバッガが終了しなかったりする)
	wcscpy_s(logFont.lfFaceName, LF_FACESIZE, _T("ＭＳ 明朝"));
#else
	strcpy_s(logFont.lfFaceName, LF_FACESIZE, ("ＭＳ 明朝"));
#endif	
	textMan.RegisterFont(logFont, 0);
	textMan.SetDefaultSize(640, 960);
	textMan.SetFont(0);

}

void MyMSScene::InitializeUI()
{
	using namespace ValueMyScene::UI;
	mLuaDb.Load("Resource/Script/GamePlayUI.lua", cUILuaID, "GamePlayUI");
	auto lManager = mLuaDb.GetManager(cUILuaID);


	//テクスチャ一括登録
	{
		std::unordered_map<int, std::string>lFileNameMap;
		std::string lFileName;
		lManager->GetGlobal(cStatusFrameTexturePath, lFileName);
		lFileNameMap[cStatusFrameID] = lFileName;
		lManager->GetGlobal(cHPBarOutTexturePath, lFileName);
		lFileNameMap[cHPBarOutID] = lFileName;
		lManager->GetGlobal(cHPBarInTexturePath, lFileName);
		lFileNameMap[cHPBarInID] = lFileName;
		lManager->GetGlobal(cEPBarOutTexturePath, lFileName);
		lFileNameMap[cEPBarOutID] = lFileName;
		lManager->GetGlobal(cEPBarInTexturePath, lFileName);
		lFileNameMap[cEPBarInID] = lFileName;

		lManager->GetGlobal(cScoreBarOutTexturScoreath, lFileName);
		lFileNameMap[cScoreBarOutID] = lFileName;
		lManager->GetGlobal(cScoreBarLeftTexturScoreath, lFileName);
		lFileNameMap[cScoreBarLeftID] = lFileName;
		lManager->GetGlobal(cScoreBarRightTexturScoreath, lFileName);
		lFileNameMap[cScoreBarRightID] = lFileName;

		mTexManager.RegisterFileList(lFileNameMap);
	}


	std::vector<float>lPosition;
	std::vector<float>lSize;
	std::vector<float>lScale;
	std::vector<float>lOffset;
	//解放用
	auto ClearTemp = [&]() {
		lPosition.clear();
		lSize.clear();
		lScale.clear();
		lOffset.clear();
	};

	//ステータスフレームのレイアウト情報
	{
		lManager->GetGlobal(cStatusFramePositionName, lPosition);
		lManager->GetGlobal(cStatusFrameSize, lSize);
		lManager->GetGlobal(cStatusFrameScale, lScale);

		auto mStatusFrame = ui.mStatusFrame.get();
		mStatusFrame->SetGlobalPosition(lPosition[0], lPosition[1]);
		mStatusFrame->SetGlobalScale(lScale[0], lScale[1]);
		mStatusFrame->SetSize(lSize[0], lSize[1]);
		mStatusFrame->SetTexture(mTexManager, cStatusFrameID);
		mStatusFrame->Update();
		ClearTemp();
	}

	//HPバーのレイアウト情報
	{
		auto lHPBar = ui.mStatusFrame->GetHPBar();
		MSProgress data;
		data.Set(100, 0, 30);

		lManager->GetGlobal(cHPBarPosition, lPosition);
		lManager->GetGlobal(cHPBarSize, lSize);
		lManager->GetGlobal(cHPBarInScale, lScale);
		lManager->GetGlobal(cHPTextOffset, lOffset);


		lHPBar->SetGlobalPosition(lPosition[0], lPosition[1]);
		lHPBar->SetGaugeScale(lScale[0], lScale[1]);
		lHPBar->SetSize(lSize[0], lSize[1]);
		lHPBar->SetTextures(
		{ mTexManager,cHPBarOutID },
		{ mTexManager,cHPBarInID }
		);
		
		ui.mStatusFrame->SetHPTextOffset(lOffset[0], lOffset[1]);
		//lHPBar->Update();
		ClearTemp();

	}

		//EPバーのレイアウト情報
	{
		auto lEPBar = ui.mStatusFrame->GetEPBar();
		
		lManager->GetGlobal(cEPBarPosition, lPosition);
		lManager->GetGlobal(cEPBarSize, lSize);
		lManager->GetGlobal(cEPBarInScale, lScale);
		lManager->GetGlobal(cEPTextOffset, lOffset);

		lEPBar->SetGlobalPosition(lPosition[0], lPosition[1]);
		lEPBar->SetGaugeScale(lScale[0], lScale[1]);
		lEPBar->SetSize(lSize[0], lSize[1]);
		
		lEPBar->SetTextures(
		{ mTexManager,cEPBarOutID },
		{ mTexManager,cEPBarInID }
		);

		ui.mStatusFrame->SetEPTextOffset(lOffset[0], lOffset[1]);

		ClearTemp();

	}

	/*
		HP,EPテキストの座標などの情報取得追加
		スコアテキスト情報追加
	*/

	{
		
		auto lScoreView = ui.GetScoreView();
		lManager->GetGlobal(cScoreBarPosition, lPosition);
		lManager->GetGlobal(cScoreBarSize, lSize);
		lManager->GetGlobal(cScoreBarInScale, lScale);
		lManager->GetGlobal(cScoreTextOffset, lOffset);
		
		lScoreView->SetGlobalPosition(lPosition[0], lPosition[1]);
		lScoreView->SetSize(lSize[0], lSize[1]);
		lScoreView->SetGaugeScale(lScale[0], lScale[1]);
		lScoreView->SetTextOffset(lOffset[0], lOffset[1]);
		lScoreView->SetTextures(&mTexManager, cScoreBarOutID, cScoreBarLeftID, cScoreBarRightID);
		ClearTemp();
	}

	{
		auto lTimeView = ui.GetTimeView();
		lManager->GetGlobal(cTimeTextPosition, lPosition);
		lTimeView->SetGlobalPosition(lPosition[0], lPosition[1]);

	}

}

void MyMSScene::InitializeEffect()
{
	mEfkRender.Initialize(MSDirect::GetDevice(), MSDirect::GetDeviceContext());
	mEfkManager.Initialize(mEfkRender.GetRenderer());
	mEfkDb.Initialize(mEfkManager.GetManager());
	mEfkDb.Load("Resource/Effect/Hit.efk", ValueMyScene::Effect::cHitID);
	mEfkDb.Load("Resource/Effect/Shot.efk", ValueMyScene::Effect::cShotID);
	mEfkDb.Load("Resource/Effect/Kill.efk", ValueMyScene::Effect::cKillID);
	mEfkDb.Load("Resource/Effect/Goal.efk", ValueMyScene::Effect::cGoalInID);

	mEfkObj.SetManager(&mEfkManager);
	decltype(auto) lEfk = mEfkDb.Get(ValueMyScene::Effect::cHitID);
	mEfkObj.SetEffect(lEfk);
	mEfkObj.SetPosition({ 11.0f,0.0f,7.5f });
}

void MyMSScene::InitializeModel()
{
	using namespace ValueMyScene::Model;
	mLuaDb.Load("Resource/Script/GamePlayModel.lua", cLuaID, "GamePlayUI");
	auto lManager = mLuaDb.GetManager(cLuaID);

	//モデルファイルマップ
	std::unordered_map<int, std::string>lFileNameMap;
	std::unordered_map<int, bool>lIsAnimationMap;
	std::string lFileName;
	float lScale;
	bool lIsAnimation;
	//ノーマル弾
	lManager->GetGlobal(cBulletDesignModel, lFileName);
	lFileNameMap[cBulletDesignID] = lFileName;
	lManager->GetGlobal(cBulletCollisionModel, lFileName);
	lFileNameMap[cBulletCollisionID] = lFileName;

	lManager->GetGlobal(cBulletDesignAnimation, lIsAnimation);
	lIsAnimationMap[cBulletDesignID] = lIsAnimation;
	lManager->GetGlobal(cBulletCollisionAnimation, lIsAnimation);
	lIsAnimationMap[cBulletCollisionID] = lIsAnimation;

	lManager->GetGlobal(cBulletDesignScale, lScale);
	mFbxScaleMap[cBulletDesignID] = lScale;
	lManager->GetGlobal(cBulletCollisionScale, lScale);
	mFbxScaleMap[cBulletCollisionID] = lScale;


	//黒チームキャラ
	lManager->GetGlobal(cCharaBlackDesignModel, lFileName);
	lFileNameMap[cCharaBlackDesignID] = lFileName;
	lManager->GetGlobal(cCharaBlackCollisionModel, lFileName);
	lFileNameMap[cCharaBlackCollisionID] = lFileName;

	lManager->GetGlobal(cCharaBlackDesignAnimation, lIsAnimation);
	lIsAnimationMap[cCharaBlackDesignID] = lIsAnimation;
	lManager->GetGlobal(cCharaBlackCollisionAnimation, lIsAnimation);
	lIsAnimationMap[cCharaBlackCollisionID] = lIsAnimation;

	lManager->GetGlobal(cCharaBlackDesignScale, lScale);
	mFbxScaleMap[cCharaBlackDesignID] = lScale;
	lManager->GetGlobal(cCharaBlackCollisionScale, lScale);
	mFbxScaleMap[cCharaBlackCollisionID] = lScale;


	//白チームキャラ
	lManager->GetGlobal(cCharaWhiteDesignModel, lFileName);
	lFileNameMap[cCharaWhiteDesignID] = lFileName;
	lManager->GetGlobal(cCharaWhiteCollisionModel, lFileName);
	lFileNameMap[cCharaWhiteCollisionID] = lFileName;

	lManager->GetGlobal(cCharaWhiteDesignAnimation, lIsAnimation);
	lIsAnimationMap[cCharaWhiteDesignID] = lIsAnimation;
	lManager->GetGlobal(cCharaWhiteCollisionAnimation, lIsAnimation);
	lIsAnimationMap[cCharaWhiteCollisionID] = lIsAnimation;

	lManager->GetGlobal(cCharaWhiteDesignScale, lScale);
	mFbxScaleMap[cCharaWhiteDesignID] = lScale;
	lManager->GetGlobal(cCharaWhiteCollisionScale, lScale);
	mFbxScaleMap[cCharaWhiteCollisionID] = lScale;

	//黒チーム拠点
	lManager->GetGlobal(cBaseBlackDesignModel, lFileName);
	lFileNameMap[cBaseBlackDesignID] = lFileName;
	lManager->GetGlobal(cBaseBlackCollisionModel, lFileName);
	lFileNameMap[cBaseBlackCollisionID] = lFileName;

	lManager->GetGlobal(cBaseBlackDesignAnimation, lIsAnimation);
	lIsAnimationMap[cBaseBlackDesignID] = lIsAnimation;
	lManager->GetGlobal(cBaseBlackCollisionAnimation, lIsAnimation);
	lIsAnimationMap[cBaseBlackCollisionID] = lIsAnimation;

	lManager->GetGlobal(cBaseBlackDesignScale, lScale);
	mFbxScaleMap[cBaseBlackDesignID] = lScale;
	lManager->GetGlobal(cBaseBlackCollisionScale, lScale);
	mFbxScaleMap[cBaseBlackCollisionID] = lScale;

	//白チーム拠点
	lManager->GetGlobal(cBaseWhiteDesignModel, lFileName);
	lFileNameMap[cBaseWhiteDesignID] = lFileName;
	lManager->GetGlobal(cBaseWhiteCollisionModel, lFileName);
	lFileNameMap[cBaseWhiteCollisionID] = lFileName;

	lManager->GetGlobal(cBaseWhiteDesignAnimation, lIsAnimation);
	lIsAnimationMap[cBaseWhiteDesignID] = lIsAnimation;
	lManager->GetGlobal(cBaseWhiteCollisionAnimation, lIsAnimation);
	lIsAnimationMap[cBaseWhiteCollisionID] = lIsAnimation;

	lManager->GetGlobal(cBaseWhiteDesignScale, lScale);
	mFbxScaleMap[cBaseWhiteDesignID] = lScale;
	lManager->GetGlobal(cBaseWhiteCollisionScale, lScale);
	mFbxScaleMap[cBaseWhiteCollisionID] = lScale;

	//フィールドマップ
	lManager->GetGlobal(cFieldDesignModel, lFileName);
	lFileNameMap[cFieldDesignID] = lFileName;
	lManager->GetGlobal(cFieldCollisionModel, lFileName);
	lFileNameMap[cFieldCollisionID] = lFileName;

	lManager->GetGlobal(cFieldDesignAnimation, lIsAnimation);
	lIsAnimationMap[cFieldDesignID] = lIsAnimation;
	lManager->GetGlobal(cFieldCollisionAnimation, lIsAnimation);
	lIsAnimationMap[cFieldCollisionID] = lIsAnimation;

	lManager->GetGlobal(cFieldDesignScale, lScale);
	mFbxScaleMap[cFieldDesignID] = lScale;
	lManager->GetGlobal(cFieldCollisionScale, lScale);
	mFbxScaleMap[cFieldCollisionID] = lScale;

	
	//ボール
	lManager->GetGlobal(cBallDesignModel, lFileName);
	lFileNameMap[cBallDesignID] = lFileName;
	lManager->GetGlobal(cBallCollisionModel, lFileName);
	lFileNameMap[cBallCollisionID] = lFileName;

	lManager->GetGlobal(cBallDesignAnimation, lIsAnimation);
	lIsAnimationMap[cBallDesignID] = lIsAnimation;
	lManager->GetGlobal(cBallCollisionAnimation, lIsAnimation);
	lIsAnimationMap[cBallCollisionID] = lIsAnimation;

	lManager->GetGlobal(cBallDesignScale, lScale);
	mFbxScaleMap[cBallDesignID] = lScale;
	lManager->GetGlobal(cBallCollisionScale, lScale);
	mFbxScaleMap[cBallCollisionID] = lScale;




	//モデル読み込み
	for (auto&lFilePair : lFileNameMap) {
		mdDB.Load(
			lFilePair.second.c_str(),
			lIsAnimationMap[lFilePair.first],
			lFilePair.first
			);
	}



}

void MyMSScene::InitializeFieldStatus()
{
	mFieldStatus.Initialize();
	mFieldStatus.InitializeTime(70);
	mFieldStatus.InitEffect(&mEfkManager, &mEfkDb, ValueMyScene::Effect::cGoalInID, ValueMyScene::Effect::cKillID);
	mFieldStatus.CreateFieldNodes();
	mFieldStatus.CreateSpawnCharaNodes();
	mFieldStatus.CreateSpawnBallNodes();
	mFieldStatus.InitGoalIndex(19, 80);
	mFieldStatus.mBallIsField = true;
	mFieldStatus.mBall = &mBall;
	mFieldStatus.RespawnBall();

	for (int i = 0; i < cEnemyCount; ++i) {
		enemy.push_back(make_unique<Enemy>());
		enemy[i]->Initialize(mFieldStatus);
	}
	mPlayer.Initialize(mFieldStatus);


	mFieldStatus.RegisterTeamMember(enemy[0].get(), eTeamType::Black);
	mFieldStatus.RegisterTeamMember(enemy[1].get(), eTeamType::White);
	mFieldStatus.RegisterTeamMember(enemy[2].get(), eTeamType::Black);
	mFieldStatus.RegisterTeamMember(enemy[3].get(), eTeamType::White);
	mFieldStatus.RegisterTeamMember(&mPlayer, eTeamType::White);
	SetMeshResouce(mFieldStatus.GetTeamBase(eTeamType::Black), ValueMyScene::Model::cBaseBlackDesignID, ValueMyScene::Model::cBaseBlackCollisionID);
	SetMeshResouce(mFieldStatus.GetTeamBase(eTeamType::White), ValueMyScene::Model::cBaseWhiteDesignID, ValueMyScene::Model::cBaseWhiteCollisionID);

	{
		DXVector3 lWhitePosition;
		DXVector3 lBlackPosition;
		lWhitePosition = mFieldStatus.GetNodePosition(19);
		lBlackPosition = mFieldStatus.GetNodePosition(80);
		mFieldStatus.GetTeamBase(eTeamType::White)->GetWorld()->SetT(lWhitePosition);
		mFieldStatus.GetTeamBase(eTeamType::Black)->GetWorld()->SetT(lBlackPosition);
		mFieldStatus.InitRenderAndShader(render, shader);
	}

}

void MyMSScene::InitializeBulletManager()
{
	auto lNBulletStatus = LoadBulletStatus("Resource/Script/BulletNormal.lua", BulletUniqueID::NBullet);

	bltManager.Initialize(&lNBulletStatus);
	bltManager.InitEffect(&mEfkManager, &mEfkDb, ValueMyScene::Effect::cShotID, ValueMyScene::Effect::cHitID, ValueMyScene::Effect::cKillID);
	bltManager.RegisterMesh(
		&mdDB,
		&mFbxScaleMap,
		ValueMyScene::Model::cBulletDesignID,
		ValueMyScene::Model::cBulletCollisionID,
		BulletUniqueID::NBullet
	);

	bltManager.RegisterShader(&shader, BulletUniqueID::NBullet);
	bltManager.RegisterChara(&mPlayer, BulletUniqueID::NBullet);

	mPlayer.mBltManager = &bltManager;
	for (auto&lEnemy : enemy) {
		lEnemy->mBltManager = &bltManager;
		bltManager.RegisterChara(lEnemy.get(), BulletUniqueID::NBullet);
	}
}

void MyMSScene::InitializeCharaFirst()
{
	mLuaDb.Load("Resource/Script/CharacterDefaultStatus.lua", ValueMyScene::Chara::cStatusID);

	auto lManager = mLuaDb.GetManager(ValueMyScene::Chara::cStatusID);
	int lHp, lEnergy;
	lManager->GetGlobal(ValueStatusBase::cHp, lHp);
	lManager->GetGlobal(ValueStatusBase::cEnergy, lEnergy);
	mDefaultStatus.mAllyNear = false;
	mDefaultStatus.mBall = nullptr;
	mDefaultStatus.mBallAllyNear = false;
	mDefaultStatus.mEnergy.Set(lEnergy, 0.0f, lEnergy);
	mDefaultStatus.mHp.Set(lHp, 0.0f, lHp);
	mDefaultStatus.mLive = CharaStateFlag::ALIVE;
	mDefaultStatus.mTargetChara = nullptr;
}

void MyMSScene::InitializeEnemy()
{

	//全キャラ分
	for (uint16_t i = 0; i < enemy.size(); ++i) {
		enemy[i]->SetDefaultStatus(mDefaultStatus);

		//AIの設定
		enemy[i]->SetAI(mLuaDb.GetManager(0));
		//チーム別にメッシュを変えて差別化をする
		auto lTeamType = mFieldStatus.GetTeamType(enemy[i].get());
		switch (lTeamType)
		{
		case eTeamType::White:
			SetMeshResouce(
				enemy[i].get(),
				ValueMyScene::Model::cCharaWhiteDesignID,
				ValueMyScene::Model::cCharaWhiteCollisionID
			);
			break;
		case eTeamType::Black:
			SetMeshResouce(
				enemy[i].get(),
				ValueMyScene::Model::cCharaBlackDesignID,
				ValueMyScene::Model::cCharaBlackCollisionID
			);
			break;
		default:
			break;
		}
		//ボールを探索対象とする
		enemy[i]->AddSearchTarget(&mBall);

		//自分以外のエネミーを追加
		for (auto&lEnemy : enemy) {
			if (enemy[i] != lEnemy) {
				//チームじゃないメンバーを追加する
				if (mFieldStatus.GetTeamAlly(lEnemy.get())->IsMember(enemy[i].get()) == false) {
					enemy[i]->AddSearchTarget(&*lEnemy);
				}
			}
		}


		enemy[i]->SetRenderer(&render);
		enemy[i]->SetShader(&shader);
		//enemy[i]->SetBulletMesh(*mdDB.Get(cbox));
		enemy[i]->Respawn();
		if (mFieldStatus.GetTeamAlly(&mPlayer)->IsMember(enemy[i].get()) == false) {
			enemy[i]->AddSearchTarget(&mPlayer);
		}

		enemy[i]->AddCollisionTarget(&mField);
		enemy[i]->AddCollisionTarget(&mBall);
		enemy[i]->GetView()->SetCamera(*enemy[i]->GetWorld(), { 0.0f,0.0f,2.00f });
		enemy[i]->GetProj()->SetProjection(60, 0.01f, 500.0f);
		enemy[i]->InitFinal();
	}



}

void MyMSScene::InitializePlayer()
{
	auto lTeamType = mFieldStatus.GetTeamType(&mPlayer);
	switch (lTeamType)
	{
	case eTeamType::White:
		SetMeshResouce(
			&mPlayer,
			ValueMyScene::Model::cCharaWhiteDesignID,
			ValueMyScene::Model::cCharaWhiteCollisionID
		);
		break;
	case eTeamType::Black:
		SetMeshResouce(
			&mPlayer,
			ValueMyScene::Model::cCharaBlackDesignID,
			ValueMyScene::Model::cCharaBlackCollisionID
		);
		break;
	default:
		break;
	}

	mPlayer.SetRenderer(&render);
	mPlayer.SetShader(&shader);
	mPlayer.SetDefaultStatus(mDefaultStatus);
	mPlayer.Respawn();

	mPlayer.AddCollisionTarget(&mField);
	mPlayer.AddCollisionTarget(&mBall);
	for (auto&lEnemy : enemy) {
		if (mFieldStatus.GetTeamAlly(&mPlayer)->IsMember(lEnemy.get()) == false) {
			mPlayer.AddSearchTarget(&*lEnemy);
		}
	}

}

void MyMSScene::InitializeBall()
{
	mBall.Initialize();
	SetMeshResouce(&mBall, ValueMyScene::Model::cBallDesignID, ValueMyScene::Model::cBallCollisionID);
}

void MyMSScene::InitializeTimeOver()
{
	using namespace ValueMyScene::UI;
	auto lManager = mLuaDb.GetManager(cUILuaID);

	std::unordered_map<int, std::string>lFileNameMap;
	std::string lFileName;
	using namespace ValueMyScene::TimeOver;
	lManager->GetGlobal(cLogoFinishTexturePath, lFileName);
	lFileNameMap[cLogoFinishID] = lFileName;
	lManager->GetGlobal(cLogoIssueWinTexturePath, lFileName);
	lFileNameMap[cLogoWinID] = lFileName;
	lManager->GetGlobal(cLogoIssueLoseTexturePath, lFileName);
	lFileNameMap[cLogoLoseID] = lFileName;
	lManager->GetGlobal(cLogoIssueDrawTexturePath, lFileName);
	lFileNameMap[cLogoDrawID] = lFileName;
	mTexManager.RegisterFileList(lFileNameMap);

	std::vector<float>lPosition;
	std::vector<float>lSize;
	std::vector<float>lScale;
	//解放用
	auto ClearTemp = [&]() {
		lPosition.clear();
		lSize.clear();
		lScale.clear();
	};

	//FinishLogo
	{
		lManager->GetGlobal(cLogoFinishPosition, lPosition);
		lManager->GetGlobal(cLogoFinishSize, lSize);
		lManager->GetGlobal(cLogoFinishScale, lScale);
		
		mTimeOver.SetTextures(&mTexManager, cLogoFinishID, cLogoWinID, cLogoLoseID, cLogoDrawID);

		mTimeOver.SetFinishPosition({ lPosition[0], lPosition[1] });
		mTimeOver.SetFinishScale({ lScale[0], lScale[1] });
		mTimeOver.SetFinishSize({ lSize[0], lSize[1] });

		lManager->GetGlobal(cLogoIssuePosition, lPosition);
		lManager->GetGlobal(cLogoIssueSize, lSize);
		lManager->GetGlobal(cLogoIssueScale, lScale);
		mTimeOver.SetIssuePosition({ lPosition[0], lPosition[1] });
		mTimeOver.SetIssueScale({ lScale[0], lScale[1] });
		mTimeOver.SetIssueSize({ lSize[0], lSize[1] });
	}

}

void MyMSScene::InitializeResult()
{
	using namespace ValueMyScene::Result;
	mLuaDb.Load("Resource/Script/Result.lua", cLuaID);
	auto lManager = mLuaDb.GetManager(cLuaID);

	//テクスチャ一括登録
	{
		std::unordered_map<int, std::string>lFileNameMap;
		std::string lFileName;

		lManager->GetGlobal(cFrameTexturePath, lFileName);
		lFileNameMap[cFrameID] = lFileName;

		lManager->GetGlobal(cLogoWinTexturePath, lFileName);
		lFileNameMap[cLogoWinID] = lFileName;
		lManager->GetGlobal(cLogoLoseTexturePath, lFileName);
		lFileNameMap[cLogoLoseID] = lFileName;
		lManager->GetGlobal(cLogoDrawTexturePath, lFileName);
		lFileNameMap[cLogoDrawID] = lFileName;

		lManager->GetGlobal(cButtonRetryNormalTexturePath, lFileName);
		lFileNameMap[cButtonRetryNormalID] = lFileName;
		lManager->GetGlobal(cButtonRetryActiveTexturePath, lFileName);
		lFileNameMap[cButtonRetryActiveID] = lFileName;
		lManager->GetGlobal(cButtonRetryPushTexturePath, lFileName);
		lFileNameMap[cButtonRetryPushID] = lFileName;
		lManager->GetGlobal(cButtonRetryDisableTexturePath, lFileName);
		lFileNameMap[cButtonRetryDisableID] = lFileName;

		lManager->GetGlobal(cButtonToTitleNormalTexturePath, lFileName);
		lFileNameMap[cButtonToTitleNormalID] = lFileName;
		lManager->GetGlobal(cButtonToTitleActiveTexturePath, lFileName);
		lFileNameMap[cButtonToTitleActiveID] = lFileName;
		lManager->GetGlobal(cButtonToTitlePushTexturePath, lFileName);
		lFileNameMap[cButtonToTitlePushID] = lFileName;
		lManager->GetGlobal(cButtonToTitleDisableTexturePath, lFileName);
		lFileNameMap[cButtonToTitleDisableID] = lFileName;

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
	//Init Frame
	{
		lManager->GetGlobal(cFramePosition, lPosition);
		lManager->GetGlobal(cFrameSize, lSize);
		mResult.SetFrameTexture(&mTexManager, cFrameID);
		mResult.SetFramePosition(lPosition[0], lPosition[1]);
		mResult.SetFrameSize(lSize[0], lSize[1]);
		ClearTemp();
	}


	//Init Button
	{
		lManager->GetGlobal(cTogglePosition, lPosition);
		mResult.SetButtonRetryTexture(&mTexManager, cButtonRetryNormalID, cButtonRetryActiveID, cButtonRetryPushID, cButtonRetryDisableID);
		mResult.SetButtonToTitleTexture(&mTexManager, cButtonToTitleNormalID, cButtonToTitleActiveID, cButtonToTitlePushID, cButtonToTitleDisableID);
		mResult.SetButtonPadding(0, 20);
		mResult.SetButtonPosition(lPosition[0], lPosition[1]);

		lManager->GetGlobal(cButtonRetrySize, lSize);
		mResult.SetButtonSize(lSize[0], lSize[1]);
		ClearTemp();
	}


	//Init Logo
	{
		lManager->GetGlobal(cLogoPosition, lPosition);
		lManager->GetGlobal(cLogoSize, lSize);
		mResult.SetLogoTexture(&mTexManager, cLogoWinID, cLogoLoseID, cLogoDrawID);
		mResult.SetLogoPosition(lPosition[0], lPosition[1]);
		mResult.SetLogoSize(lSize[0], lSize[1]);
		ClearTemp();
	}

	//InitText
	{
		lManager->GetGlobal(cTextPosition, lPosition);
		mResult.SetTextPosition(lPosition[0],lPosition[1]);
		ClearTemp();
	}

}

void MyMSScene::SetMeshResouce(GameObjectBase*aObject, const int aDesignID, const int aCollisionID)
{
	
	aObject->SetMesh(*mdDB.Get(aDesignID));
	aObject->SetMeshScale(
		mFbxScaleMap[aDesignID],
		mFbxScaleMap[aDesignID],
		mFbxScaleMap[aDesignID]
	);

	aObject->SetCollisionMesh(*mdDB.Get(aCollisionID));
	aObject->SetCollisionScale(
		mFbxScaleMap[aCollisionID],
		mFbxScaleMap[aCollisionID],
		mFbxScaleMap[aCollisionID]
	);

}

StatusBulletBase MyMSScene::LoadBulletStatus(const std::string& aFileName, const int aBulletID)
{
	mLuaDb.Load(aFileName, aBulletID);
	auto lManager = mLuaDb.GetManager(aBulletID);
	StatusBulletBase lStatus;
	
	{
		using namespace ValueStatusBullet;
		lManager->GetGlobal(cCost, lStatus.mCost);
		lManager->GetGlobal(cAtk, lStatus.mAtk);
		float lInterval;
		lManager->GetGlobal(cInterval, lInterval);
		lStatus.mInterval.Set(lInterval, 0.0f, lInterval);
		lManager->GetGlobal(cIntervalRecovery, lStatus.mIntervalRecovery);

		int lDamageTypeValue;
		lManager->GetGlobal(cDamageType, lDamageTypeValue);
		lStatus.mType = ConvertDamageType(lDamageTypeValue);

		lManager->GetGlobal(cFiringRange, lStatus.mFiringRange);
		lManager->GetGlobal(cVelocity, lStatus.mVelocity);

		std::vector<float>lDirection;
		lManager->GetGlobal(cDirection, lDirection);
		lStatus.mDirection.Set(lDirection[0], lDirection[1], lDirection[2]);
	}

	return lStatus;
}

void MyMSScene::UpdateUI()
{
	int lMinutes, lSeconds;
	mFieldStatus.UpdateTime();
	mFieldStatus.GetRemainTime(lMinutes, lSeconds);
	if (lSeconds < 0)lSeconds = 0;
	if (lMinutes < 0)lMinutes = 0;
	ui.GetTimeView()->UpdateTime(lMinutes, lSeconds);

	auto lWhiteScore = mFieldStatus.GetScoreWhite();
	auto lBlackScore = mFieldStatus.GetScoreBlack();

	ui.GetScoreView()->UpdateScore(lWhiteScore, lBlackScore);
	ui.mStatusFrame->UpdateStatus(mPlayer.GetStatus());
}

void MyMSScene::UpdateTimeOver()
{
	//時間制限が来た時の処理
	if (mFieldStatus.IsTimeOver()) {
		if (mIsTimeOver == false) {
			mIsTimeOver = true;
			auto lIssue = mFieldStatus.IsWin(&mPlayer);
			mTimeOver.UpdateStart(lIssue);
			ResultValue lResult;
			lResult.mBlackScore = mFieldStatus.GetScoreBlack();
			lResult.mWhiteScore = mFieldStatus.GetScoreWhite();
			lResult.mIssue = lIssue;
			mResult.SetValues(lResult);

			for (auto&lEnemy : enemy) {
				lEnemy->StopAI();
			}

		}
	}

	if (mIsTimeOver == true) {
		mTimeOver.Update();

		//Updateが終わったらリザルト移行
		if (mTimeOver.IsUpdateing()==false) {

		}

	}

}


