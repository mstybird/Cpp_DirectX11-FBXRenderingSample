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

	std::shared_ptr<MSFbxManager> mbox;				//モデルデータ
	std::shared_ptr<DX11RenderResource> ground;
	std::shared_ptr<DX11RenderResource> me;	//自身

	//描画に使うシェーダ
	std::shared_ptr<My3DShader> shader;
	//シーンに一つ
	std::shared_ptr<MS3DRender> render;

};