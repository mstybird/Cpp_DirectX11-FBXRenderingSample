#include"MyScene.h"
#include"MSUtility.h"

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

void MyMSScene::Initialize()
{
	{
		//スプライト初期化
		m2DShader.Init();
		m2DShader.InitVertex("Sprite2D.hlsl");
		m2DShader.InitPixel("Sprite2D.hlsl");
		m2DRender.SetViewPort(MSDirect::GetViewPort());
		m2DRender.SetShader(m2DShader);

		mTexManager.RegisterFile("res/yutapon lumia.png", 0);
		mImage.SetTexture(mTexManager, 0);
		mImage.SetSize({ 150,100 });
		mImage.SetPosition({50, 25});
		mImage.SetSplitSizeX({ 0.5f,1.0f });
	}

	{
		mBall.Initialize();
		mFieldStatus.CreateFieldNodes();
		mFieldStatus.CreateSpawnCharaNodes();
		mFieldStatus.CreateSpawnBallNodes();
		mFieldStatus.mBallIsField = true;
		mFieldStatus.mBall = &mBall;
		mFieldStatus.RespawnBall();
	}


	//AI読み込み
	mLuaDb.Load("EnemyAI/htn_attack.lua", 0, "EnemyAI");

	shader.Init();
	shader.InitVertex("Simple.hlsl");
	shader.InitPixel("Simple.hlsl");

	mdBullet.LoadFile("res/box.fbx", false);
	mdChara.LoadFile("res/SD_QUERY_01.fbx", true);

	mdField.LoadFile("res/FieldDesign.fbx", false);
	mdFieldCol.LoadFile("res/FieldCollision.fbx", false);
	mdBall.LoadFile("res/ball.fbx", false);

	//敵の初期化
	enemy.push_back(make_unique<Enemy>());
	//enemy.push_back(make_unique<Enemy>());

	float scaleBall = 0.01f;
	float scaleChara = 0.3f;
	float scaleField = 1.0f;


	for (uint16_t i = 0; i < enemy.size(); ++i) {
		enemy[i]->Initialize(mFieldStatus);
		enemy[i]->SetAI(mLuaDb.GetManager(0));
		enemy[i]->SetMesh(mdChara);
		enemy[i]->SetCollisionMesh(mdBullet);
		enemy[i]->SetCollisionScale(scaleBall, scaleBall, scaleBall);
		enemy[i]->SetRenderer(&render);
		enemy[i]->SetShader(&shader);
		enemy[i]->SetBulletMesh(mdBullet);
		enemy[i]->Respawn();
	}
	mField.Initialize();
	mField.SetMesh(mdField);
	mField.SetCollisionMesh(mdFieldCol);
	mField.SetCollisionScale(scaleField, scaleField, scaleField);
	mField.SetRenderer(&render);
	mField.SetShader(&shader);

	mBall.SetMesh(mdBall);
	mBall.SetCollisionMesh(mdBall);
	mBall.SetRenderer(&render);
	mBall.SetShader(&shader);
	mBall.GetWorld()->SetT(4, 0, 5);
	mBall.GetWorld()->SetS(scaleBall, scaleBall, scaleBall);


	mPlayer.Initialize(mFieldStatus);
	mPlayer.SetMesh(mdChara);
	mPlayer.SetCollisionMesh(mdBullet);
	mPlayer.SetCollisionScale(scaleBall, scaleBall, scaleBall);
	mPlayer.SetRenderer(&render);
	mPlayer.SetShader(&shader);
	mPlayer.SetBulletMesh(mdBullet);
	mPlayer.Respawn();
	//mPlayer.SetField(mFieldStatus);

	mPlayer.AddCollisionTarget(&mField);
	mPlayer.AddCollisionTarget(&mBall);

	for (uint32_t i = 0; i < enemy.size(); ++i) {
		enemy[i]->AddSearchTarget(&mPlayer);
		enemy[i]->AddSearchTarget(&mBall);
		//自分以外のエネミーを追加
		for (auto&lEnemy : enemy) {
			if (enemy[i] != lEnemy) {
				enemy[i]->AddSearchTarget(&*lEnemy);
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
	mField.GetWorld()->SetT(-3, -1, 0);

	//enemy[0]->GetWorld()->SetT(-15, 0, 0);
	//enemy[1]->GetWorld()->SetT(-5, 0, 8);
	//enemy[2]->GetWorld()->SetT(-10, 0, 10);
	enemy[0]->SetGoalIndex(19);
	//enemy[1]->SetGoalIndex(20);

	for (uint32_t i = 0; i < enemy.size(); ++i) {
		enemy[i]->GetWorld()->SetS(scaleChara, scaleChara, scaleChara);
		enemy[i]->GetWorld()->SetRC(0, 0, 0);
		enemy[i]->GetView()->SetCamera(*enemy[i]->GetWorld(), { 0,0,-5 });
		enemy[i]->GetProj()->SetProjection(45, 0.01f, 50.0f);
		enemy[i]->InitFinal();
	}
	

}

void MyMSScene::Update() {
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
			printf("new MyNode{ %d,\"obj%d\",{%.2f,%.2f,%.2f}\n", count, count, Pos.x, Pos.y, Pos.z);
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
	float speed = 0.25;
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
	for (uint32_t i = 0; i < enemy.size(); ++i) {
		enemy[i]->Render();
	}
	mField.Render();
	mPlayer.Render();
	mBall.Render();
	//m2DRender.Render(mImage);

}

/*
	タスク続き
		ビルボード実装
		スプライトの回転実装
		イベントメソッド書き足し
		ダイナミクス実装
*/

