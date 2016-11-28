#pragma once
#include"MSSceneBase.h"

class SceneTitle :public MSSceneBase{
private:
	void KeyDown(MSKEY pKey)override;
	void Render()override;
};