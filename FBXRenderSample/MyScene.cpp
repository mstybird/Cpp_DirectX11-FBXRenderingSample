#include"MyScene.h"
#include"MSCollision.h"
MyMSScene::MyMSScene() :
	mdBox{ make_shared<MSFbxManager>() },
	rBox1{ make_shared<DX11RenderResource>() },
	rBox2{ make_shared<DX11RenderResource>() },
	rMe{ make_shared<DX11RenderResource>() },
	rLook{ make_shared<DX11RenderResource>() },
	shader{ make_shared<My3DShader>() },
	render{ make_shared<MS3DRender>() }
{
}

void MyMSScene::Initialize()
{



	shader->Init();
	shader->InitVertex("Simple.hlsl");
	shader->InitPixel("Simple.hlsl");

	mdBox->LoadFile("res/box.fbx", false);
	//コリジョン生成
	mdBox->CreateCollisionSphere();
	//生成したコリジョンを登録
	mdBox->RegisterCollision(rBox1);
	mdBox->RegisterCollision(rBox2);
	//どのシェーダーでレンダリングするか登録
	render->SetShader(shader);
	//このリソースをレンダリング用に使う
	rMe->InitRenderMatrix();
	//レンダーにカメラを登録
	render->SetRenderTarget(rMe);


	auto lWorld = rMe->GetWorld();
	auto lView = rMe->GetCamera();
	auto lProjection = rMe->GetProjection();

	lWorld.lock()->SetT(40, 50, -140);
	rBox1->GetWorld().lock()->SetT(25, 0, 0);
	rBox2->GetWorld().lock()->SetT(-25, 0, 0);
	rBox1->GetWorld().lock()->SetS(0.1f, 0.1f, 0.1f);
	rBox2->GetWorld().lock()->SetS(0.1f, 0.1f, 0.1f);



	lView.lock()->SetCamera(rMe->GetWorld(), rLook->GetWorld());

	lProjection.lock()->SetViewAngle(45);
	lProjection.lock()->SetPlaneNear(0.1f);
	lProjection.lock()->SetPlaneFar(2000.0f);

}

void MyMSScene::Update() {

	MSCollisionRay lRay;
	lRay.SetRay(*rMe->GetCamera().lock());
	if (MSCollisionRay::Collision(lRay, *rMe, *rBox1, *mdBox)) {
		printf("Rayed\n");
	}
	else {
		printf("\n");

	}

	if (rBox1->CollisionSphere(rBox2)) {
		//printf("Hit\n");
	}
}

void MyMSScene::KeyHold(MSKEY pKey)
{
	switch (pKey)
	{
	case MSKEY::CH_W:
		rBox1->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, 1, { 0,0,1 });
		break;
	case MSKEY::CH_S:
		rBox1->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, 1, { 0,0,-1 });
		break;
	case MSKEY::CH_A:
		rBox1->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, 1, { -1,0,0 });
		break;
	case MSKEY::CH_D:
		rBox1->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, 1, { 1,0,0 });
		break;
	default:
		break;
	}
}

void MyMSScene::Render()
{
	////画面クリア
	MS3DRender::Clear({ 0.2f,0.2f,0.2f,1 });
	mdBox->Update();
	//ground->GetWorld().lock()->SetS(1, 1, 1);
	rBox1->GetWorld().lock()->AddRC(0, 1, 0);
	render->Render(mdBox, rBox1);
	render->Render(mdBox, rBox2);
}

/*
	タスク続き
		ビルボード実装
		スプライトの回転実装
		イベントメソッド書き足し
		ダイナミクス実装
*/
