#include"MyScene.h"
#include"MSUtility.h"
MyMSScene::MyMSScene() :
	mdBox{ make_shared<MSFbxManager>() },
	mdField{ make_shared<MSFbxManager>() },
	rBox1{ make_shared<DX11RenderResource>() },
	rField{ make_shared<DX11RenderResource>() },
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

	mdBox->LoadFile("res/humanoid2.fbx", true);
	mdField->LoadFile("res/field.fbx", false);

	//リソースにオブジェクトを登録
	rMe->mMesh.Initialize(*mdBox);
	rBox1->mMesh.Initialize(*mdBox);
	rField->mMesh.Initialize(*mdField);

	//タスク:メッシュにコリジョン生成機能をつける
	//どのシェーダーでレンダリングするか登録
	render->SetShader(shader);
	//このリソースにビュー行列と射影行列を追加
	rMe->InitRenderMatrix();
	rBox1->InitRenderMatrix();
	//レンダーにカメラを登録
	render->SetRenderTarget(rMe);


	auto lWorld = rMe->GetWorld();
	auto lView = rMe->GetCamera();
	auto lProjection = rMe->GetProjection();

	lWorld.lock()->SetT(-5, 0, -8);
	lWorld.lock()->SetRC({ 0,0,0 });
	lWorld.lock()->SetS(0.03f, 0.03f, 0.03f);

	rField->GetWorld().lock()->SetS(0.01f, 0.01f, 0.01f);
	rBox1->GetWorld().lock()->SetT(-15, 0, 0);
	rField->GetWorld().lock()->SetT(-3, -1, 0);
	rBox1->GetWorld().lock()->SetS(0.01f, 0.01f, 0.01f);
	rBox1->GetWorld().lock()->SetRC(0, 0, 0);
	rField->GetWorld().lock()->SetS(0.1f, 0.1f, 0.1f);



	lView.lock()->SetCamera(rMe->GetWorld(), { 0,20,-10 });

	rBox1->GetCamera().lock()->SetCamera(rBox1->GetWorld(), { 0,0,-5 });

	rBox1->GetProjection().lock()->SetViewAngle(45);
	rBox1->GetProjection().lock()->SetPlaneNear(0.1f);
	rBox1->GetProjection().lock()->SetPlaneFar(2000.0f);

	lProjection.lock()->SetViewAngle(45);
	lProjection.lock()->SetPlaneNear(0.1f);
	lProjection.lock()->SetPlaneFar(2000.0f);
	//初期位置を設定
	lRayPick.SetSlipFlag(true);
	lRayPick.SetFramePosition(*rMe);
	//初期位置を設定
	lRayPickEnemy.SetSlipFlag(true);
	lRayPickEnemy.SetFramePosition(*rBox1);




	//AI初期化
	ai.CreateNodes();

	DXVector3 lPosition;
	rBox1->GetWorld().lock()->GetMatrix().lock()->GetT(lPosition);
	auto lNearNode = ai.GetNearNodeList(lPosition)[0];
	ai.SetStartNode(lNearNode->GetID());
}

void MyMSScene::Update() {

	rMe->mMesh.NextFrame();
	rMe->mMesh.Update();
	rBox1->mMesh.NextFrame();
	rBox1->mMesh.Update();

	DXVector3 lResult;
	if (lRayPick.Collision(lResult, *rMe, *rField)) {
		
		rMe->GetWorld().lock()->SetT(lResult);
	}
	lRayPick.SetFramePosition(*rMe);
	rMe->GetCamera().lock()->SetCamera(rMe->GetWorld(), { 0,25,-10 });



	//視錘台カリング
	MSCullingFrustum cf;



	//ルートがない場合
	if (ai.IsRootEmpty() == true) {
		//ルートを生成する
		ai.CreateNextRoot(false);
	}
	MyNode*lAINode;
	static int count = 0;
	ai.GetFrontNode(lAINode);
	float lAng = MSHormingY(*rBox1, lAINode->Position, 15.0f);
	rBox1->GetWorld().lock()->AddRC(0, lAng, 0);
	if (IsZero(lAng, 5.1f)) {
		rBox1->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, 0.1f, { 0,0,1 });
		DXVector3 lLength;
		rBox1->GetWorld().lock()->GetMatrix().lock()->GetT(lLength);
		lLength = lLength - lAINode->Position;
		if (lLength.GetDistance() < 0.5f) {
			printf("%d\n", lAINode->GetID());
			if (lAINode->GetID() == 1) {
				++count;
			}
			ai.SetNextNode();
		}
	}

	if (lRayPickEnemy.Collision(lResult, *rBox1, *rField)) {
		rBox1->GetWorld().lock()->SetT(lResult);
	}
	lRayPickEnemy.SetFramePosition(*rBox1);

	if (count == 3) {
		printf("%d\n", count);

	}

	//rMeにはコリジョンが入っている
	//敵から見てプレイヤーが視界に居るか
	//if (cf.IsCullingWorld(*rBox1, *rMe)) {
	//	if (MSCullingOcculusion::IsCullingWorld(
	//		*render, *rBox1, rMe,0.05f,
	//		[&]() {
	//		render->Render(rField);
	//	}
	//		)) {

	//		float lAng = MSHormingY(*rBox1, *rMe, 1.0f);
	//		//rBox1->GetWorld().lock()->AddRC(0, lAng, 0);
	//		if (IsZero(lAng,0.1f)) {
	//			//rBox1->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, 0.1f, { 0,0,1 });

	//		}
	//	}
	//	else {

	//	}
	//}



}

void MyMSScene::KeyDown(MSKEY pKey)
{
	printf("Key\n");

	switch (pKey)
	{
		case MSKEY::SPACE:
		{
			DXVector3 Pos;
			/*rMe->GetWorld().lock()->GetMatrix().lock()->GetT(Pos);
			printf("%3.2f, %3.2f, %3.2f\n", Pos.x, Pos.y, Pos.z);
*/
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
	MS3DRender::Clear({ 0.2f,0.2f,0.2f,1 });
	////画面クリア
	//mdBox->Update();
	render->Render(rBox1);
	render->Render(rField);
	render->Render(rMe);

}

/*
	タスク続き
		ビルボード実装
		スプライトの回転実装
		イベントメソッド書き足し
		ダイナミクス実装
*/

