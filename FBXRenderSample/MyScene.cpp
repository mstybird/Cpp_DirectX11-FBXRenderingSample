#include"MyScene.h"
//#include"Dx11Manager.h"
#include"MSwrap.h"
#include"IncludeInstance.h"
#include"MSDirect.h"
#include"MySprite3DShader.h"
MyMSScene::MyMSScene() :
	textureManager{ make_shared<DX11TextureManager>() },
	fbx{ make_shared<MSFbxManager>() },
	mbox{ make_shared<MSFbxManager>() },
	box{ make_shared<DX11RenderResource>() },
	ground{ make_shared<DX11RenderResource>() },
	me{ make_shared<DX11RenderResource>() },
	spRes{ make_shared<MSSprite2DResource>() },
	sprite{ make_shared<My2DSpriteShader>() },
	render2D{ make_shared<MSSprite2DRender>() },
	sp3DRes{ make_shared<MSSprite3DResource>() },
	sprite3D{ make_shared<My3DSpriteShader>() },
	render3D{ make_shared<MSSprite3DRender>() },
	shader{ make_shared<My3DShader>() },
	render{ make_shared<MS3DRender>() }
{
}

void MyMSScene::Initialize()
{
	//テクスチャ登録
	textureManager->RegisterFile("res/Chips_Cover.jpg", 0);

	//2Dスプライト
	sprite->Init();
	sprite->InitVertex("Sprite2D.hlsl");
	sprite->InitPixel("Sprite2D.hlsl");

	render2D->SetShader(sprite);
	render2D->SetViewPort(MSDirect::GetViewPort());

	spRes->SetTexture(textureManager, 0);
	spRes->SetSize({ 200.0f, 200.0f });
	spRes->SetPosition({ 0,0 });
	spRes->SetPivot({ 0.5f,0.5f });
	spRes->SetScale({ 2,2 });

	//3Dスプライト
	sprite3D->Init();
	sprite3D->InitVertex("Sprite3D.hlsl");
	sprite3D->InitPixel("Sprite3D.hlsl");

	render3D->SetShader(sprite3D);

	sp3DRes->SetTexture(textureManager, 0);
	sp3DRes->SetSize({ 20,20 });
	sp3DRes->SetPivot({ 0.5f,0.5f });


	shader->Init();
	shader->InitVertex("Simple.hlsl");
	shader->InitPixel("Simple.hlsl");

	mbox->LoadFile("res/Chips.fbx", false);

	//どのシェーダーでレンダリングするか登録
	render->SetShader(shader);
	//このリソースをレンダリング用に使う
	me->InitRenderMatrix();
	//レンダーにカメラを登録
	render->SetRenderTarget(me);
	render3D->SetRenderTarget(me);


	auto lWorld = me->GetWorld();
	auto lView = me->GetCamera();
	auto lProjection = me->GetProjection();

	auto world = me->GetWorld();
	world.lock()->SetT(0, 0, -80);
	lView.lock()->SetCamera(me->GetWorld(), box->GetWorld());

	lProjection.lock()->SetViewAngle(45);
	lProjection.lock()->SetPlaneNear(0.1f);
	lProjection.lock()->SetPlaneFar(2000.0f);

	box->GetWorld().lock()->SetT(0, 0, 0);
	//box->GetWorld()->SetRC(45, 45, 0);

	ground->GetWorld().lock()->SetS(1.0f, 0.001f, 1.0f);

}

void MyMSScene::Render()
{
	////画面クリア
	MS3DRender::Clear({ 1,0,0,1 });
	mbox->Update();
	ground->GetWorld().lock()->SetS(1, 1, 1);
	ground->GetWorld().lock()->AddRC(1, 0, 0);
	//render->Render(mbox, ground);

	render2D->Render(spRes);
	render3D->Render(sp3DRes);

}

/*
	タスク続き
		ビルボード実装
		スプライトの回転実装
		イベントメソッド書き足し
		ダイナミクス実装
*/
