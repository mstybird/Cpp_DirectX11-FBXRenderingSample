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
	void Update()override {}
	void Render();
	void Destroy() override {}

private:

	std::shared_ptr<MSFbxManager> mbox;				//���f���f�[�^
	std::shared_ptr<DX11RenderResource> ground;
	std::shared_ptr<DX11RenderResource> me;	//���g

	//�`��Ɏg���V�F�[�_
	std::shared_ptr<My3DShader> shader;
	//�V�[���Ɉ��
	std::shared_ptr<MS3DRender> render;

};