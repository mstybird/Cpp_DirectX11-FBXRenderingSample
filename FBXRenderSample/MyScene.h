#pragma once
#include<memory>

#include"MSInclude.h"
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

	std::shared_ptr<DX11TextureManager> textureManager;

	std::shared_ptr<MSFbxManager> fbx;				//���f���f�[�^
	std::shared_ptr<MSFbxManager> mbox;				//���f���f�[�^
	std::shared_ptr<DX11RenderResource> box;	//�{�b�N�X�ړ�
	std::shared_ptr<DX11RenderResource> ground;
	std::shared_ptr<DX11RenderResource> me;	//���g

	std::shared_ptr<MSSprite2DResource> spRes;	//�X�v���C�g���\�[�X
	std::shared_ptr<My2DSpriteShader>sprite;	//�X�v���C�g�V�F�[�_
	std::shared_ptr<MSSprite2DRender> render2D;//�X�v���C�g�����_���[


	//3D�X�v���C�g
	std::shared_ptr<MSSprite3DResource>sp3DRes;
	std::shared_ptr<My3DSpriteShader>sprite3D;
	std::shared_ptr<MSSprite3DRender>render3D;

	std::shared_ptr<My3DShader> shader;//�`��Ɏg���V�F�[�_
										 //�V�[���Ɉ��
	std::shared_ptr<MS3DRender> render;

};