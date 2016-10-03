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

	mdBox->LoadFile("res/box.fbx", false);
	mdField->LoadFile("res/field.fbx", false);

	//�R���W��������
	mdBox->CreateCollisionSphere();
	//���������R���W������o�^
	mdBox->RegisterCollision(rMe);
	mdBox->RegisterCollision(rBox1);
	mdBox->RegisterCollision(rField);
	//�ǂ̃V�F�[�_�[�Ń����_�����O���邩�o�^
	render->SetShader(shader);
	//���̃��\�[�X�Ƀr���[�s��Ǝˉe�s���ǉ�
	rMe->InitRenderMatrix();
	rBox1->InitRenderMatrix();
	//�����_�[�ɃJ������o�^
	render->SetRenderTarget(rMe);


	auto lWorld = rMe->GetWorld();
	auto lView = rMe->GetCamera();
	auto lProjection = rMe->GetProjection();

	lWorld.lock()->SetT(0, 0, 0);
	lWorld.lock()->SetRC({ 0,0,0 });
	lWorld.lock()->SetS(0.01f, 0.01f, 0.01f);

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
	//�����ʒu��ݒ�
	lRayPick.SetSlipFlag(true);
	lRayPick.SetFramePosition(*rMe);
	
	lRayPlane.SetFramePosition(*rMe);

}

void MyMSScene::Update() {


	rMe->GetCamera().lock()->SetCamera(rMe->GetWorld(), { 0,5,-10});


	//������J�����O
	MSCullingFrustum cf;

	//�G���猩�ăv���C���[�����E�ɋ��邩
	if (cf.IsCullingWorld(*rBox1, *rMe)) {
		if (MSCullingOcculusion::IsCullingWorld(
			*render, *rBox1, rMe, mdBox,0.05f,
			[&]() {
			render->Render(mdField, rField);
		}
			)) {

			float lAng = MSHormingY(*rBox1, *rMe, 1.0f);
			rBox1->GetWorld().lock()->AddRC(0, lAng, 0);
			if (IsZero(lAng,0.1f)) {
				//rBox1->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, 0.1f, { 0,0,1 });

			}
		}
		else {

		}
	}



}

void MyMSScene::KeyDown(MSKEY pKey)
{
	switch (pKey)
	{
		case MSKEY::SPACE:
		{
			DXVector3 Pos;
			rMe->GetWorld().lock()->GetMatrix().lock()->GetT(Pos);
			printf("%3.2f %3.2f %3.2f\n", Pos.x, Pos.y, Pos.z);

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
	////��ʃN���A
	mdBox->Update();
	render->Render(mdBox, rBox1);
	render->Render(mdField, rField);
	render->Render(mdBox, rMe);

}

/*
	�^�X�N����
		�r���{�[�h����
		�X�v���C�g�̉�]����
		�C�x���g���\�b�h��������
		�_�C�i�~�N�X����
*/

