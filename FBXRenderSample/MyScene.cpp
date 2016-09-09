#include"MyScene.h"
//#include"Dx11Manager.h"
#include"MSwrap.h"
#include"IncludeInstance.h"
#include"MSDirect.h"

MyMSScene::MyMSScene() :
	textureManager{ std::make_shared<DX11TextureManager>() },
	fbx{ std::make_shared<MSFbxManager>() },
	mbox{ std::make_shared<MSFbxManager>() },
	box{ std::make_shared<DX11RenderResource>() },
	ground{ std::make_shared<DX11RenderResource>() },
	me{ std::make_shared<DX11RenderResource>() },
	spRes{ std::make_shared<MSSprite2DResource>() },
	sprite{ std::make_shared<My2DSpriteShader>() },
	render2D{ std::make_shared<DX11Sprite2DRender>() },
	shader{ std::make_shared<My3DShader>() },
	render{ std::make_shared<MS3DRender>() }
{
}

void MyMSScene::Initialize()
{
	//テクスチャ登録
	textureManager->RegisterFile("res/Chips_Cover.jpg", 0);

	sprite->Init();
	sprite->InitVertex("Sprite2D.hlsl");
	sprite->InitPixel("Sprite2D.hlsl");

	render2D->SetShader(sprite);
	render2D->SetViewPort(MSDirect::GetViewPort());

	spRes->SetTexture(textureManager, 0);
	spRes->SetSize({ 200.0f, 200.0f });

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

	auto lWorld = me->GetWorld();
	auto lView = me->GetCamera();
	auto lProjection = me->GetProjection();

	auto world = me->GetWorld();
	world.lock()->SetT(70, 20, -80);
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
	render->Render(mbox, ground);

	render2D->Render(spRes);

}
