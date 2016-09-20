#include"MyScene.h"
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
	//�R���W��������
	mdBox->CreateCollisionSphere();
	//���������R���W������o�^
	mdBox->RegisterCollision(rBox1);
	mdBox->RegisterCollision(rBox2);
	//�ǂ̃V�F�[�_�[�Ń����_�����O���邩�o�^
	render->SetShader(shader);
	//���̃��\�[�X�������_�����O�p�Ɏg��
	rMe->InitRenderMatrix();
	//�����_�[�ɃJ������o�^
	render->SetRenderTarget(rMe);


	auto lWorld = rMe->GetWorld();
	auto lView = rMe->GetCamera();
	auto lProjection = rMe->GetProjection();

	lWorld.lock()->SetT(40, 50, -140);
	rBox1->GetWorld().lock()->SetT(30, 0, 0);
	rBox2->GetWorld().lock()->SetT(-50, 0, 0);
	rBox1->GetWorld().lock()->SetS(0.1f, 0.1f, 0.1f);
	rBox2->GetWorld().lock()->SetS(0.1f, 0.1f, 0.1f);



	lView.lock()->SetCamera(rMe->GetWorld(), rLook->GetWorld());

	lProjection.lock()->SetViewAngle(45);
	lProjection.lock()->SetPlaneNear(0.1f);
	lProjection.lock()->SetPlaneFar(2000.0f);
	//�����ʒu��ݒ�
	lRayPick.SetSlipFlag(true);
	lRayPick.SetFramePosition(*rBox1);
	
}

void MyMSScene::Update() {

	MSCollisionRay lRay;
	lRay.SetRay(*rMe->GetCamera().lock());
	if (MSCollisionRay::Collision(lRay, *rMe, *rBox1, *mdBox)) {
		//���C
	}
	DXVector3 lResult;
	if (lRayPick.Collision(lResult, *rBox1, *rBox2, *mdBox)) {
		//���C�s�b�L���O
		rBox1->GetWorld().lock()->SetT(lResult);
	}
	//���W�̍X�V
	lRayPick.SetFramePosition(*rBox1);
	

	if (rBox1->CollisionSphere(rBox2)) {
		//���E��
	}
}

void MyMSScene::KeyHold(MSKEY pKey)
{
	DXVector3 data;

	switch (pKey)
	{
	case MSKEY::CH_W:
		//rMe->GetCamera().lock()->Translation(DXCamera::TYPE_PARALLEL, 1, { 0,0,1 }, false);
		rBox1->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, 1, { 0,0,1 });
		break;
	case MSKEY::CH_S:
		//rMe->GetCamera().lock()->Translation(DXCamera::TYPE_PARALLEL, -1, { 0,0,1 }, false);
		rBox1->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, 1, { 0,0,-1 });
		break;
	case MSKEY::CH_A:
		//rMe->GetCamera().lock()->Translation(DXCamera::TYPE_PARALLEL, -1, { 1,0,0 }, false);
		rBox1->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, 1, { -1,0,0 });
		break;
	case MSKEY::CH_D:
		//rMe->GetCamera().lock()->Translation(DXCamera::TYPE_PARALLEL, 1, { 1,0,0 }, false);
		rBox1->GetWorld().lock()->AddT(DXWorld::TYPE_ROTATE, 1, { 1,0,0 });
		break;
	default:
		break;
	}
}

void MyMSScene::Render()
{
	////��ʃN���A
	MS3DRender::Clear({ 0.2f,0.2f,0.2f,1 });
	mdBox->Update();
	//ground->GetWorld().lock()->SetS(1, 1, 1);
	//rBox1->GetWorld().lock()->AddRC(0, 1, 0);
	render->Render(mdBox, rBox1);
	render->Render(mdBox, rBox2);
}

/*
	�^�X�N����
		�r���{�[�h����
		�X�v���C�g�̉�]����
		�C�x���g���\�b�h��������
		�_�C�i�~�N�X����
*/
