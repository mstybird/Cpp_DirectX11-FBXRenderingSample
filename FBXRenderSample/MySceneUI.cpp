#include "MySceneUI.h"
#include"MSSprite2DRender.h"
#include"StatusFrame.h"
MySceneUI::MySceneUI():
	mStatusFrame{std::make_unique<StatusFrame>()}
{
}

MySceneUI::~MySceneUI()
{
}

void MySceneUI::Render(MSSprite2DRender & aRender, UIBase * aParent)
{
	mStatusFrame->Render(aRender, this);
}

StatusFrame * MySceneUI::GetStatusFrame()
{
	return mStatusFrame.get();
}
