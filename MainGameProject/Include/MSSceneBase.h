#pragma once
#include<Windows.h>

enum class MouseType {
	Left,
	Center,
	Right
};

//ゲームシーン
enum class MSKEY;
class MSSceneBase abstract{
public:
	//シーンの一度のみの初期化
	virtual void Initialize() {};
	//シーン初期化完了フラグセット
	virtual void InitializeEnd()final { bInitializeFlag = true; };
	//フレームの更新処理
	virtual void Update() {};
	//描画処理
	virtual void Render() {};
	//シーン破棄処理
	virtual void Destroy() {};

	
	virtual ~MSSceneBase() {};


	virtual bool IsInitialized()final { return bInitializeFlag; }

	//イベントプロシージャ
	virtual void KeyDown(MSKEY pKey) {};
	virtual void KeyHold(MSKEY pKey) {};
	virtual void KeyUp(MSKEY pKey) {};
	virtual void MouseMove(const POINT & aNowPosition, const POINT & aDiffPosition) {};
	virtual void MouseDown(const MouseType aType) {};
	virtual void MouseUp(const MouseType aType) {};
private:
	//シーンが初期化されるとtrueになる
	bool bInitializeFlag = false;
};
