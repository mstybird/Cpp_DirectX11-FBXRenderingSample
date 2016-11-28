#include"MyScene.h"
#include"MSUtility.h"
#include"BarGauge.h"
/*
タスク：
攻撃を受けるとダメージ
弾を二種類用意
弾ごとにコストを設定
チャージ速度など

UI設計
*/

//const int ValueMyScene::UI::cUILuaID = 100;

MyMSScene::MyMSScene()
{
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
	//エフェクト初期化
	{
		//using namespace Comfort;
		mEfkRender.Initialize(MSDirect::GetDevice(), MSDirect::GetDeviceContext());
		mEfkManager.Initialize(mEfkRender.GetRenderer());
		mEfkDb.Initialize(mEfkManager.GetManager());
		mEfkDb.Load("Resource/Effect/test.efk", 0);

		mEfkObj.SetManager(&mEfkManager);
		decltype(auto) lEfk = mEfkDb.Get(0);
		mEfkObj.SetEffect(lEfk);
		mEfkObj.SetPosition({ 11.0f,0.0f,7.5f });

	}

	{
		//スプライト初期化
		m2DShader.Init();
		m2DShader.InitVertex("Resource/HLSL/Sprite2D.hlsl");
		m2DShader.InitPixel("Resource/HLSL/Sprite2D.hlsl");
		m2DRender.SetViewPort(MSDirect::GetViewPort());
		m2DRender.SetShader(m2DShader);

		mTexManager.RegistFromFile("Resource/Texture/MyScene/Chips_Cover.jpg", 0);
		mTexManager.RegistFromFile("Resource/Texture/MyScene/Grass.png", 1);

		mImage.SetTexture(mTexManager, 0);
		mImage.SetSize({ 150,100 });
		mImage.SetPosition({ 50, 25 });
		mImage.SetSplitSizeX({ 0.5f,1.0f });
		mImage.CreateBuffer();






	}

	InitializeUI();

	{
		mBall.Initialize();
		mFieldStatus.CreateFieldNodes();
		mFieldStatus.CreateSpawnCharaNodes();
		mFieldStatus.CreateSpawnBallNodes();
		mFieldStatus.InitGoalIndex();
		mFieldStatus.mBallIsField = true;
		mFieldStatus.mBall = &mBall;
		mFieldStatus.RespawnBall();
	}

	//AI読み込み
	mLuaDb.Load("Resource/AIPlanning/EnemyTask.lua", 0, "EnemyAI");

	shader.Init();
	shader.InitVertex("Resource/HLSL/Simple.hlsl");
	shader.InitPixel("Resource/HLSL/Simple.hlsl");

	//mdDB.Load("Resource/Model/box.fbx", false, cbullet);
	////mdDB.Load("Resource/Model/box.fbx", false, cChara);
	//mdDB.Load("Resource/Model/FoxyGirl_Static.fbx", true, cChara);

	//mdDB.Load("Resource/Model/FieldCollision.fbx", false, cFieldD);
	//mdDB.Load("Resource/Model/FieldCollision.fbx", false, cFieldC);
	//mdDB.Load("Resource/Model/ball.fbx", false, cBall);



	//敵の初期化
	enemy.push_back(make_unique<Enemy>());
	enemy.push_back(make_unique<Enemy>());
	enemy.push_back(make_unique<Enemy>());
	enemy.push_back(make_unique<Enemy>());

	mFieldStatus.RegisterTeamMember(enemy[0].get(), eTeamType::Black);
	mFieldStatus.RegisterTeamMember(enemy[1].get(), eTeamType::White);
	mFieldStatus.RegisterTeamMember(enemy[2].get(), eTeamType::Black);
	mFieldStatus.RegisterTeamMember(enemy[3].get(), eTeamType::White);

	mFieldStatus.RegisterTeamMember(&mPlayer, eTeamType::White);

	//enemy.push_back(make_unique<Enemy>());



	for (uint16_t i = 0; i < enemy.size(); ++i) {
		enemy[i]->Initialize(mFieldStatus);
		enemy[i]->SetAI(mLuaDb.GetManager(0));

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
		
		
		enemy[i]->SetRenderer(&render);
		enemy[i]->SetShader(&shader);
		//enemy[i]->SetBulletMesh(*mdDB.Get(cbox));
		enemy[i]->Respawn();
	}



	mField.Initialize();
	SetMeshResouce(&mField, ValueMyScene::Model::cFieldDesignID, ValueMyScene::Model::cFieldCollisionID);
	mField.SetRenderer(&render);
	mField.SetShader(&shader);
	mField.GetWorld()->SetT(0, -1, 0);

	SetMeshResouce(&mBall, ValueMyScene::Model::cBallDesignID, ValueMyScene::Model::cBallCollisionID);
	mBall.SetRenderer(&render);
	mBall.SetShader(&shader);

	mPlayer.Initialize(mFieldStatus);


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
	mPlayer.Respawn();

	mPlayer.AddCollisionTarget(&mField);
	mPlayer.AddCollisionTarget(&mBall);

	for (uint32_t i = 0; i < enemy.size(); ++i) {
		//enemy[i]->AddSearchTarget(&mPlayer);
		enemy[i]->AddSearchTarget(&mBall);
		//自分以外のエネミーを追加
		for (auto&lEnemy : enemy) {
			if (enemy[i] != lEnemy) {
				if (mFieldStatus.GetTeamAlly(lEnemy.get())->IsMember(enemy[i].get()) == false) {
					enemy[i]->AddSearchTarget(&*lEnemy);
				}
			}
		}
		enemy[i]->AddCollisionTarget(&mField);
		enemy[i]->AddCollisionTarget(&mBall);
	}

	//バレットマネージャの初期化
	{
		bltManager.Initialize();
		bltManager.RegisterMesh(
			mdDB.Get(ValueMyScene::Model::cBulletDesignID),
			mdDB.Get(ValueMyScene::Model::cBulletCollisionID),
			0);
		bltManager.RegisterShader(&shader, 0);
		bltManager.RegisterChara(&mPlayer, 0);
		
		mPlayer.mBltManager = &bltManager;
		for (auto&lEnemy : enemy) {
			lEnemy->mBltManager = &bltManager;
			bltManager.RegisterChara(lEnemy.get(), 0);
		}


		//リソースにオブジェクトを登録
		render.SetShader(&shader);
		render.SetRenderTarget(*mPlayer.GetTransform());

		//mPlayer.GetWorld()->SetS(scaleChara, scaleChara, scaleChara);
		mPlayer.GetView()->SetCamera(*mPlayer.GetWorld(), { 0.0f,6.6f,-10.0f });
		mPlayer.GetProj()->SetProjection(60, 0.1f, 500.0f);
		for (auto&lEnemy : enemy) {
			mPlayer.AddSearchTarget(&*lEnemy);
		}


		//enemy[0]->GetWorld()->SetT(-15, 0, 0);
		//enemy[1]->GetWorld()->SetT(-5, 0, 8);
		//enemy[2]->GetWorld()->SetT(-10, 0, 10);
		//enemy[0]->SetGoalIndex(19);
		//enemy[1]->SetGoalIndex(20);

		for (uint32_t i = 0; i < enemy.size(); ++i) {
			enemy[i]->GetView()->SetCamera(*enemy[i]->GetWorld(), { 0,0,-5 });
			enemy[i]->GetProj()->SetProjection(45, 0.01f, 50.0f);
			enemy[i]->InitFinal();
		}


		::Comfort::EffectCamera cam;
		::Comfort::EffectProjection proj;
		cam.SetDXCamera(mPlayer.GetView());
		proj.SetDXProjection(mPlayer.GetProj());
		mEfkRender.SetCamera(&cam);
		mEfkRender.SetProjection(&proj);
		mEfkObj.Play();

	}
}

void MyMSScene::Update() {
	mPlayer.Update();
	DXVector3 v;
	mPlayer.GetWorld()->GetMatrix().lock()->GetT(v);
	mEfkObj.SetPosition({ v.x,v.y,v.z });
	::Comfort::EffectCamera cam;
	cam.SetDXCamera(mPlayer.GetView());
	mEfkRender.SetCamera(&cam);




	mEfkManager.Update();
	mField.Update();
	for (uint32_t i = 0; i < enemy.size(); ++i) {
		enemy[i]->Update();
	}
}

void MyMSScene::KeyDown(MSKEY pKey)
{
	static int count = 0;
	switch (pKey)
	{
	case MSKEY::ENTER:
		mPlayer.AddBullet();
		break;
	case MSKEY::SPACE:
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

void MyMSScene::KeyHold(MSKEY pKey)
{
	DXVector3 data;
	float speed = 0.5;
	switch (pKey)
	{
	case MSKEY::CH_W:
		//rMe->GetCamera().lock()->Translation(DXCamera::TYPE_PARALLEL, 0.1, { 0,0,1 }, false);
		mPlayer.GetWorld()->AddT(DXWorld::TYPE_ROTATE, speed, { 0,0,1 });
		break;
	case MSKEY::CH_S:
		//rMe->GetCamera().lock()->Translation(DXCamera::TYPE_PARALLEL, -0.1, { 0,0,1 }, false);
		mPlayer.GetWorld()->AddT(DXWorld::TYPE_ROTATE, speed, { 0,0,-1 });
		break;
	case MSKEY::CH_A:
		//rMe->GetCamera().lock()->Translation(DXCamera::TYPE_PARALLEL, -0.1, { 1,0,0 }, false);
		mPlayer.GetWorld()->AddT(DXWorld::TYPE_ROTATE, speed, { -1,0,0 });
		break;
	case MSKEY::CH_D:
		//rMe->GetCamera().lock()->Translation(DXCamera::TYPE_PARALLEL, 0.1, { 1,0,0 }, false);
		mPlayer.GetWorld()->AddT(DXWorld::TYPE_ROTATE, speed, { 1,0,0 });
		break;
	case MSKEY::LEFT:
		mPlayer.GetWorld()->AddRC({ 0,-5,0 });
		break;
	case MSKEY::RIGHT:
		mPlayer.GetWorld()->AddRC({ 0,5,0 });
		break;
	default:
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
	mField.Render();
	mPlayer.Render();
	mBall.Render();
	mEfkRender.RenderAll(&mEfkManager);


	ui.Render(m2DRender);


}

void MyMSScene::InitializeFont()
{
	//フォントの準備
	FontLog	logFont;
	::ZeroMemory(&logFont, sizeof(logFont));
	logFont.lfHeight = 40;	//フォントサイズ
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

	{
		auto lHPBar = ui.mStatusFrame->GetHPBar();
		MSProgress data;
		data.Set(100, 0, 30);

		lManager->GetGlobal(cHPBarPosition, lPosition);
		lManager->GetGlobal(cHPBarSize, lSize);
		lManager->GetGlobal(cHPBarInScale, lScale);

		lHPBar->SetGlobalPosition(lPosition[0], lPosition[1]);
		lHPBar->SetGaugeScale(lScale[0], lScale[1]);
		lHPBar->SetSize(lSize[0], lSize[1]);
		lHPBar->SetParam(data);
		lHPBar->SetTextures(
		{ mTexManager,cHPBarOutID },
		{ mTexManager,cHPBarInID }
		);
		//lHPBar->Update();
		ClearTemp();

	}


	{
		auto lEPBar = ui.mStatusFrame->GetEPBar();
		MSProgress data;
		data.Set(100, 0, 50);

		lManager->GetGlobal(cEPBarPosition, lPosition);
		lManager->GetGlobal(cEPBarSize, lSize);
		lManager->GetGlobal(cEPBarInScale, lScale);

		lEPBar->SetGlobalPosition(lPosition[0], lPosition[1]);
		lEPBar->SetGaugeScale(lScale[0], lScale[1]);
		lEPBar->SetSize(lSize[0], lSize[1]);
		lEPBar->SetParam(data);
		lEPBar->SetTextures(
		{ mTexManager,cEPBarOutID },
		{ mTexManager,cEPBarInID }
		);
		//lEPBar->Update();
		ClearTemp();

	}


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

/*
タスク続き
ビルボード実装
スプライトの回転実装
イベントメソッド書き足し
ダイナミクス実装
*/

