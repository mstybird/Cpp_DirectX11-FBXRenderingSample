#include"MyScene.h"
#include"MSUtility.h"
MyMSScene::MyMSScene() :
	mdBox{ make_shared<MSFbxManager>() },
	mdField{ make_shared<MSFbxManager>() },
	shader{ make_shared<My3DShader>() },
	render{ make_shared<MS3DRender>() }
{
}

void MyMSScene::Initialize()
{
	//AI読み込み
	mLuaDb.Load("EnemyAI/htn_attack.lua", 0, "EnemyAI");

	shader->Init();
	shader->InitVertex("Simple.hlsl");
	shader->InitPixel("Simple.hlsl");

	mdBox->LoadFile("res/SD_QUERY_01.fbx", true);
	mdField->LoadFile("res/field.fbx", false);

	//敵の初期化
	enemy.push_back(make_unique<Enemy>());
	enemy.push_back(make_unique<Enemy>());
	enemy.push_back(make_unique<Enemy>());



	for (int i = 0; i < enemy.size(); ++i) {
		enemy[i]->Initialize();
		enemy[i]->SetAI(mLuaDb.GetManager(0));
		enemy[i]->SetMesh(*mdBox);
		enemy[i]->SetRenderer(render.get());
		enemy[i]->SetShader(shader.get());
		enemy[i]->SetBulletMesh(*mdBox);

	}

	mField.Initialize();
	mField.SetMesh(*mdField);
	mField.SetRenderer(render.get());
	mField.SetShader(shader.get());

	mPlayer.Initialize();
	mPlayer.SetMesh(*mdBox);
	mPlayer.SetRenderer(render.get());
	mPlayer.SetShader(shader.get());
	mPlayer.SetBulletMesh(*mdBox);

	mPlayer.AddCollisionTarget(mField.GetTransform());

	for (int i = 0; i < enemy.size(); ++i) {
		enemy[i]->AddSearchTarget(mPlayer.GetTransform());
		enemy[i]->AddCollisionTarget(mField.GetTransform());
	}


	//リソースにオブジェクトを登録
	render->SetShader(shader.get());
	render->SetRenderTarget(*mPlayer.GetTransform());

	mPlayer.GetWorld()->SetT(-5, 0, -8);
	mPlayer.GetWorld()->SetRC({ 0,0,0 });
	mPlayer.GetWorld()->SetS(0.3f, 0.3f, 0.3f);
	mPlayer.GetView()->SetCamera(*mPlayer.GetWorld(), { 0.0f,6.6f,-10.0f });
	mPlayer.GetProj()->SetProjection(60, 0.1f, 500.0f);

	mField.GetWorld()->SetS(0.1f, 0.1f, 0.1f);
	mField.GetWorld()->SetT(-3, -1, 0);

	enemy[0]->GetWorld()->SetT(-15, 0, 0);
	enemy[1]->GetWorld()->SetT(-5, 0, 8);
	enemy[2]->GetWorld()->SetT(-10, 0, 10);
	for (int i = 0; i < enemy.size(); ++i) {
		enemy[i]->GetWorld()->SetS(0.3f, 0.3f, 0.3f);
		enemy[i]->GetWorld()->SetRC(0, 0, 0);
		enemy[i]->GetView()->SetCamera(*enemy[i]->GetWorld(), { 0,0,-5 });
		enemy[i]->GetProj()->SetProjection(45, 0.01f, 50.0f);
		enemy[i]->InitFinal();
	}
	

}

void MyMSScene::Update() {
	mPlayer.Update();
	mField.Update();
	for (int i = 0; i < enemy.size(); ++i) {
	enemy[i]->Update();

	}
}

void MyMSScene::KeyDown(MSKEY pKey)
{
	printf("Key\n");

	switch (pKey)
	{
		case MSKEY::ENTER:
			mPlayer.AddBullet();
			break;
		case MSKEY::SPACE:
		{
			DXVector3 Pos;
			mPlayer.GetWorld()->GetMatrix().lock()->GetT(Pos);
			printf("%3.2f, %3.2f, %3.2f\n", Pos.x, Pos.y, Pos.z);

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
	for (int i = 0; i < enemy.size(); ++i) {
		enemy[i]->Render();
	}
	mField.Render();
	mPlayer.Render();

}

/*
	タスク続き
		ビルボード実装
		スプライトの回転実装
		イベントメソッド書き足し
		ダイナミクス実装
*/

