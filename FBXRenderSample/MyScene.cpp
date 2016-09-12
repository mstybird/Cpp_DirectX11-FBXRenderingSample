#include"MyScene.h"

MyMSScene::MyMSScene() :
	mbox{ make_shared<MSFbxManager>() },
	ground{ make_shared<DX11RenderResource>() },
	me{ make_shared<DX11RenderResource>() },
	shader{ make_shared<My3DShader>() },
	render{ make_shared<MS3DRender>() }
{
}

void MyMSScene::Initialize()
{
	struct VERT {
		DXVector3 Pos;
		DXVector2 UV;
	};

	VERT v[4] = {
		{
			{0,0,0},{0,0}
		},
		{
			{ 1,0,0 },{ 0,0 }
		},
		{
			{ 0,1,0 },{ 0,0 }
		},
		{
			{ 1,1,0 },{ 0,0 }
		}
	};

	float a, b;

	shader->Init();
	shader->InitVertex("Simple.hlsl");
	shader->InitPixel("Simple.hlsl");

	mbox->LoadFile("res/box.fbx", false);

	//どのシェーダーでレンダリングするか登録
	render->SetShader(shader);
	//このリソースをレンダリング用に使う
	me->InitRenderMatrix();
	//レンダーにカメラを登録
	render->SetRenderTarget(me);


	auto lWorld = me->GetWorld();
	auto lView = me->GetCamera();
	auto lProjection = me->GetProjection();

	lWorld.lock()->SetT(30, 60, -140);
	lView.lock()->SetCamera(me->GetWorld(), ground->GetWorld());

	lProjection.lock()->SetViewAngle(45);
	lProjection.lock()->SetPlaneNear(0.1f);
	lProjection.lock()->SetPlaneFar(2000.0f);

}

void MyMSScene::Render()
{
	////画面クリア
	MS3DRender::Clear({ 0.2f,0.2f,0.2f,1 });
	mbox->Update();
	ground->GetWorld().lock()->SetS(0.1, 0.1, 0.1);
	//ground->GetWorld().lock()->SetS(1, 1, 1);
	render->Render(mbox, ground);
}

/*
	タスク続き
		ビルボード実装
		スプライトの回転実装
		イベントメソッド書き足し
		ダイナミクス実装
*/
