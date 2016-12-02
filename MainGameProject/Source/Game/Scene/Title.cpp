#include "Title.h"
#include"MyScene.h"
#include"MSDirect.h"
#include"MS3DRender.h"
void SceneTitle::Initialize()
{
}
void SceneTitle::Update()
{
}
void SceneTitle::KeyDown(MSKEY pKey)
{
	if (pKey == MSKEY::CH_Z) {
		MSDirect::SetScene(std::make_unique<MyMSScene>());
	}
}

void SceneTitle::KeyHold(MSKEY pKey)
{
}

void SceneTitle::Render()
{
	MS3DRender::Clear({ 0.2f,0.8f,0.2f,1 });
}
