#pragma once
#include<memory>
#include<iostream>
#include"MSwrap.h"
#include"IncludeForward.h"
class MyMSScene :public MSSceneBase {
public:
	MyMSScene();
	~MyMSScene() {
	}
private:
	void Initialize();
	void Update()override;
	void KeyHold(MSKEY pKey)override;
	void Render();
	void Destroy() override {}

private:

	std::shared_ptr<MSFbxManager> mdBox;
	std::shared_ptr<MSFbxManager> mdField;				//���f���f�[�^
	std::shared_ptr<DX11RenderResource> rBox1;
	std::shared_ptr<DX11RenderResource> rBox2;
	std::shared_ptr<DX11RenderResource> rMe;	//���g
	std::shared_ptr<DX11RenderResource> rLook;	//���g

	//�`��Ɏg���V�F�[�_
	std::shared_ptr<My3DShader> shader;
	//�V�[���Ɉ��
	std::shared_ptr<MS3DRender> render;

	MSCollisionRayPicking lRayPick;
	MSCollisionRayPlane lRayPlane;
};