#include"MyScene.h"
MyMSScene::MyMSScene() :
	mdBox{ make_shared<MSFbxManager>() },
	mdField{ make_shared<MSFbxManager>() },
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
	mdField->LoadFile("res/field.fbx", false);

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

	lWorld.lock()->SetT(0, 0, 0);
	lWorld.lock()->SetRC({ 0,0,0 });
	lWorld.lock()->SetS(0.01f, 0.01f, 0.01f);
	rBox2->GetWorld().lock()->SetS(0.01f, 0.01f, 0.01f);
	rBox1->GetWorld().lock()->SetT(-15, 0, 0);
	rBox2->GetWorld().lock()->SetT(-3, -1, 0);
	rBox1->GetWorld().lock()->SetS(0.01f, 0.01f, 0.01f);
	rBox2->GetWorld().lock()->SetS(0.1f, 0.1f, 0.1f);



	lView.lock()->SetCamera(rMe->GetWorld(), { 0,20,-10 });

	lProjection.lock()->SetViewAngle(45);
	lProjection.lock()->SetPlaneNear(0.1f);
	lProjection.lock()->SetPlaneFar(2000.0f);
	//初期位置を設定
	lRayPick.SetSlipFlag(true);
	lRayPick.SetFramePosition(*rMe);
	
	lRayPlane.SetFramePosition(*rMe);

}

void MyMSScene::Update() {

	MSCollisionRay lRay;
	lRay.SetRay(*rMe->GetCamera().lock());
	if (MSCollisionRay::Collision(lRay, *rMe, *rBox1, *mdBox)) {
		//レイ
	}
	DXVector3 lResult;
	if (lRayPick.Collision(lResult, *rMe, *rBox2, *mdField)) {
		//レイピッキング
		rMe->GetWorld().lock()->SetT(lResult);
	}
	//座標の更新
	lRayPick.SetFramePosition(*rMe);
	

	if (rBox1->CollisionSphere(rBox2)) {
		//境界球
	}

	rMe->GetCamera().lock()->SetCamera(rMe->GetWorld(), { 0,20,-10});


	lRayPlane.SetFramePosition(*rMe);

	//視錘台カリング
	MSCullingFrustum cf;

	if (cf.IsCullingWorld(*rMe, *rBox1, *rMe->GetProjection().lock())) {
		DXProjection lP;
		lP.SetViewAngle(45);
		lP.SetPlaneNear(1.0f);
		lP.SetPlaneFar(1000.0f);

		if (MSCullingOcculusion::IsCullingWorld(
			*render, *rMe, lP, rBox1, mdBox,0.05f,
			[&]() {
			render->Render(mdField, rBox2);
		}
			)) {
			printf("Occluse\n");
		}
		else {
			printf("NoOccluse\n");

		}
	}
	else {
		printf("\n");
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
		rMe->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, speed, { 0,0,1 });
		break;
	case MSKEY::CH_S:
		//rMe->GetCamera().lock()->Translation(DXCamera::TYPE_PARALLEL, -0.1, { 0,0,1 }, false);
		rMe->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, speed, { 0,0,-1 });
		break;
	case MSKEY::CH_A:
		//rMe->GetCamera().lock()->Translation(DXCamera::TYPE_PARALLEL, -0.1, { 1,0,0 }, false);
		rMe->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, speed, { -1,0,0 });
		break;
	case MSKEY::CH_D:
		//rMe->GetCamera().lock()->Translation(DXCamera::TYPE_PARALLEL, 0.1, { 1,0,0 }, false);
		rMe->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, speed, { 1,0,0 });
		break;
	case MSKEY::LEFT:
		rMe->GetWorld().lock()->AddRC({ 0,-5,0 });
		break;
	case MSKEY::RIGHT:
		rMe->GetWorld().lock()->AddRC({ 0,5,0 });
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
	render->Render(mdBox, rBox1);
	render->Render(mdField, rBox2);
	render->Render(mdBox, rMe);

}

/*
	タスク続き
		ビルボード実装
		スプライトの回転実装
		イベントメソッド書き足し
		ダイナミクス実装
*/

