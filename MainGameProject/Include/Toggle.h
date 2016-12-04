#pragma once
#include"UIBase.h"
#include<DXMath.hpp>
#include<vector>
class Button;
class DXVector2;
class Toggle :public UIBase {
public:
	Toggle();
	~Toggle();
	//ボタンを追加する
	void AddButton(Button* aButton);
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
	//描画処理
	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;



private:

	//パディング
	DXVector2 mPadding;

	//ボタンの参照リスト
	std::vector<Button*>mButtonArray;
	//アクティブインデクス
	int mActiveIndex;
};