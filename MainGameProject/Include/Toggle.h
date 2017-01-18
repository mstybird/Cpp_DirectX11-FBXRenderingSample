#pragma once
#include"UIBase.h"
#include<DXMath.hpp>
#include<vector>
class Button;
class DXVector2;
class Toggle :public UIBase {
public:
	Toggle();
	virtual ~Toggle();
	//ボタンを追加する
	void AddButton(Button* aButton);
	//ボタンの参照をクリアする
	void ClearButton();
	//アクティブにするボタンをインデクスで指定する
	bool SetActive(const int aIndex);
	//ボタンの間隔を設定する
	void SetPadding(DXVector2 aPadding);
	//ボタンの間隔を設定する
	void SetPadding(float aX,float aY);

	//トグルに登録済みのボタン数を取得する
	int GetCount();
	//現在アクティブなボタンのインデックスを取得する
	int GetActiveIndex();
	//直接IDを指定する
	void SetActiveIndex(const int aID);
	//現在アクティブなボタンを取得する
	Button* GetActiveButton();
	//次のボタンをアクティブにする
	int ActiveNext();
	//前のボタンをアクティブにする
	int ActiveBack();
	//アクティブなボタンを押す
	void PushButton();
	//アクティブなボタンを離す
	void PopButton();
	//トグルに登録済みのボタンと衝突判定を行い、衝突したボタンのIDを取得
	int CollisionPoint(int aX, int aY);
	//ピボットの設定
	void SetPivot(int aX, int aY);
	

	//描画処理
	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;

	virtual void OnChangeIndex(const int Direction) {};

protected:
	//最後に参照した親UI
	UIBase* mLastParent;

	//パディング
	DXVector2 mPadding;
	//ピボット
	DXVector2 mPivot;
	//ボタンの参照リスト
	std::vector<Button*>mButtonArray;
	//アクティブインデクス
	int mActiveIndex;
};