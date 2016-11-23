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
	//エフェクト初期化
	{
		//using namespace Comfort;
		mEfkRender.Initialize(MSDirect::GetDevice(), MSDirect::GetDeviceContext());
		mEfkManager.Initialize(mEfkRender.GetRenderer());
		mEfkDb.Initialize(mEfkManager.GetManager());
		mEfkDb.Load("res/test.efk", 0);

		mEfkObj.SetManager(&mEfkManager);
		decltype(auto) lEfk = mEfkDb.Get(0);
		mEfkObj.SetEffect(lEfk);
		mEfkObj.SetPosition({ 11.0f,0.0f,7.5f });

	}

	{
			//スプライト初期化
			m2DShader.Init();
			m2DShader.InitVertex("Sprite2D.hlsl");
			m2DShader.InitPixel("Sprite2D.hlsl");
			m2DRender.SetViewPort(MSDirect::GetViewPort());
			m2DRender.SetShader(m2DShader);

			mTexManager.RegistFromFile("res/Chips_Cover.jpg", 0);
			mTexManager.RegistFromFile("res/Grass.png", 1);
			
			mImage.SetTexture(mTexManager, 0);
			mImage.SetSize({ 150,100 });
			mImage.SetPosition({ 50, 25 });
			mImage.SetSplitSizeX({ 0.5f,1.0f });


			//フォントの準備
			LOGFONT	logFont;
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

			text.Create("abcdefghijklmn魔材ンゴ", 0, 0, 300, 600, logFont);


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
	mLuaDb.Load("EnemyAI/EnemyTask.lua", 0, "EnemyAI");

	shader.Init();
	shader.InitVertex("Simple.hlsl");
	shader.InitPixel("Simple.hlsl");

	mdDB.Load("res/box.fbx", false, cbox);
	mdDB.Load("res/box.fbx", false, cChara);

	mdDB.Load("res/box.fbx", false, cFieldD);
	mdDB.Load("res/box.fbx", false, cFieldC);
	mdDB.Load("res/ball.fbx", false, cBall);

	//mdDB.Load("res/box.fbx", false, cbox);
	//mdDB.Load("res/SD_QUERY_01.fbx", true, cChara);
	// 
	//mdDB.Load("res/FieldCollision.fbx", false, cFieldD);
	//mdDB.Load("res/FieldCollision.fbx", false, cFieldC);
	//mdDB.Load("res/ball.fbx", false, cBall);

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

	float scaleBall = 0.01f;
	float scaleChara = 0.25f;
	float scaleField = 1.0f;


	for (uint16_t i = 0; i < enemy.size(); ++i) {
		enemy[i]->Initialize(mFieldStatus);
		enemy[i]->SetAI(mLuaDb.GetManager(0));
		enemy[i]->SetMesh(*mdDB.Get(cChara));
		enemy[i]->SetCollisionMesh(*mdDB.Get(cbox));
		enemy[i]->SetCollisionScale(scaleBall, scaleBall, scaleBall);
		enemy[i]->SetRenderer(&render);
		enemy[i]->SetShader(&shader);
		enemy[i]->SetBulletMesh(*mdDB.Get(cbox));
		enemy[i]->Respawn();
	}
	mField.Initialize();
	mField.SetMesh(*mdDB.Get(cFieldD));
	mField.SetCollisionMesh(*mdDB.Get(cFieldC));
	mField.SetCollisionScale(scaleField, scaleField, scaleField);
	mField.SetRenderer(&render);
	mField.SetShader(&shader);

	mBall.SetMesh(*mdDB.Get(cBall));
	mBall.SetCollisionMesh(*mdDB.Get(cBall));
	mBall.SetRenderer(&render);
	mBall.SetShader(&shader);
	mBall.GetWorld()->SetT(4, 0, 5);
	mBall.GetWorld()->SetS(scaleBall, scaleBall, scaleBall);


	mPlayer.Initialize(mFieldStatus);
	mPlayer.SetMesh(*mdDB.Get(cChara));
	mPlayer.SetCollisionMesh(*mdDB.Get(cbox));
	mPlayer.SetCollisionScale(scaleBall, scaleBall, scaleBall);
	mPlayer.SetRenderer(&render);
	mPlayer.SetShader(&shader);
	mPlayer.SetBulletMesh(*mdDB.Get(cbox));
	mPlayer.Respawn();
	//mPlayer.SetField(mFieldStatus);

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


	//リソースにオブジェクトを登録
	render.SetShader(&shader);
	render.SetRenderTarget(*mPlayer.GetTransform());

	mPlayer.GetWorld()->SetS(scaleChara, scaleChara, scaleChara);
	mPlayer.GetView()->SetCamera(*mPlayer.GetWorld(), { 0.0f,6.6f,-10.0f });
	mPlayer.GetProj()->SetProjection(60, 0.1f, 500.0f);
	for (auto&lEnemy : enemy) {
		mPlayer.AddSearchTarget(&*lEnemy);
	}


	mField.GetWorld()->SetS(scaleField, scaleField, scaleField);
	mField.GetWorld()->SetT(0, -1, 0);

	//enemy[0]->GetWorld()->SetT(-15, 0, 0);
	//enemy[1]->GetWorld()->SetT(-5, 0, 8);
	//enemy[2]->GetWorld()->SetT(-10, 0, 10);
	//enemy[0]->SetGoalIndex(19);
	//enemy[1]->SetGoalIndex(20);

	for (uint32_t i = 0; i < enemy.size(); ++i) {
		enemy[i]->GetWorld()->SetS(scaleChara, scaleChara, scaleChara);
		enemy[i]->GetWorld()->SetRC(0, 0, 0);
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
	//mEfkObj.Play();

}

void MyMSScene::Update() {
	return;
	mEfkManager.Update();
	mPlayer.Update();
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
	MS3DRender::Clear({ 0.2f,0.2f,0.2f,1 });
	////画面クリア
	//for (uint32_t i = 0; i < enemy.size(); ++i) {
	//	enemy[i]->Render();
	//}
	//mField.Render();
	//mPlayer.Render();
	//mBall.Render();
	//mEfkRender.RenderAll(&mEfkManager);

	//ui.Render(m2DRender);

	text.Render(m2DRender);
//	m2DRender.Render(mImage);

}

void MyMSScene::InitializeUI()
{
	ui.SetGlobalPosition({50,250 });
	
	

	auto& mStatusFrame = *ui.mStatusFrame;
	mStatusFrame.SetGlobalPosition({ 20.0f,20.0f });
	mStatusFrame.SetGlobalScale({ 1.0f,1.0f });
	mStatusFrame.SetTexture(mTexManager, 1);
	mStatusFrame.SetSize(300, 300);
	

	auto lHPBar = mStatusFrame.GetHPBar();
	MSProgress data;
	data.Set(100, 0, 30);
	lHPBar->SetGlobalPosition(20, 10);
	lHPBar->SetSize(100, 20);
	lHPBar->SetScale(0.9, 0.5);
	lHPBar->SetParam(data);
	lHPBar->SetTextures(
	{ mTexManager,0 },
	{mTexManager,1}
	);
	lHPBar->Update();
	
	
	
	auto lEPBar = mStatusFrame.GetEPBar();

	lEPBar->SetGlobalPosition(20, 100);
	lEPBar->SetSize(100, 20);
	lEPBar->SetScale(0.5, 0.9);
	lEPBar->SetParam(data);
	lEPBar->SetTextures(
	{ mTexManager,0 },
	{ mTexManager,1 }
	);
	lEPBar->Update();


	MSProgress m1, m2;
	m1.Set(10, 0, 2);
	m2.Set(10, 0, 1);
	auto& mGauge = *ui.bar;
	mGauge.SetGlobalPosition({ 20.0f,20.0f });
	mGauge.SetGlobalScale({ 1.0f,1.0f });
	mGauge.SetTextures(
	{ mTexManager,0 },
	{ mTexManager,1 },
	{ mTexManager,0 }
	);
	mGauge.SetScale(0.9, 0.8);
	mGauge.SetParam(m1, m2);
	mGauge.SetSize(300, 100);

}

/*
	タスク続き
		ビルボード実装
		スプライトの回転実装
		イベントメソッド書き足し
		ダイナミクス実装
*/

