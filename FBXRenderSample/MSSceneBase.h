#pragma once

//ゲームシーン
class MSSceneBase abstract{
public:
	virtual void Initialize() {};
	virtual void InitalizeEnd()final {};
	virtual void Update() {};
	virtual void Render() {};
	virtual void Destroy() {};
	virtual ~MSSceneBase() {};
	virtual void KeyDown(int pKey) {};
private:
	//シーンが初期化されるとtrueになる
	bool bInitializeFlag;
};
