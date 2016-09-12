#pragma once

//ゲームシーン
enum class MSKEY;
class MSSceneBase abstract{
public:
	//シーンの一度のみの初期化
	virtual void Initialize() {};
	//シーン初期化完了フラグセット
	virtual void InitalizeEnd()final {};
	//フレームの更新処理
	virtual void Update() {};
	//描画処理
	virtual void Render() {};
	//シーン破棄処理
	virtual void Destroy() {};

	
	virtual ~MSSceneBase() {};

	//イベントプロシージャ
	virtual void KeyFirst(MSKEY pKey) {};
	virtual void KeyDown(MSKEY pKey) {};
	virtual void KeyHold(MSKEY pKey) {};
	virtual void KeyUp(MSKEY pKey) {};
private:
	//シーンが初期化されるとtrueになる
	bool bInitializeFlag;
};
