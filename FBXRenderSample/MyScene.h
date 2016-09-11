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

	std::shared_ptr<MSFbxManager> fbx;				//モデルデータ
	std::shared_ptr<MSFbxManager> mbox;				//モデルデータ
	std::shared_ptr<DX11RenderResource> box;	//ボックス移動
	std::shared_ptr<DX11RenderResource> ground;
	std::shared_ptr<DX11RenderResource> me;	//自身

	std::shared_ptr<MSSprite2DResource> spRes;	//スプライトリソース
	std::shared_ptr<My2DSpriteShader>sprite;	//スプライトシェーダ
	std::shared_ptr<MSSprite2DRender> render2D;//スプライトレンダラー


	//3Dスプライト
	std::shared_ptr<MSSprite3DResource>sp3DRes;
	std::shared_ptr<My3DSpriteShader>sprite3D;
	std::shared_ptr<MSSprite3DRender>render3D;

	std::shared_ptr<My3DShader> shader;//描画に使うシェーダ
										 //シーンに一つ
	std::shared_ptr<MS3DRender> render;

};