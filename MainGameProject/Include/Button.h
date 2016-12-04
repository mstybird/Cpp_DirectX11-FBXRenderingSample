#pragma once
#include"UIBase.h"
#include"MSSprite2DResource.h"

enum class ButtonState {
	Normal,
	Active,
	Push,
	Disable
};

class Button :public UIBase {
public:

	Button();
	~Button();
	//テクスチャをセットする
	void SetTextures(
		DX11TextureManager* aManager,
		const int aNormalID,
		const int aActiveID,
		const int aPushID,
		const int aDisableID
	);

	void SetSize(const float&aWidth, const float&aHeight);
	//GlobalScaleと掛け合わせたサイズの取得

	DXVector2 GetScalingSize(UIBase*aParent = nullptr);
	//指定した状態にし、直前のステートを返す
	ButtonState SetState(const ButtonState aState);
	ButtonState GetState();

	//選択状態にし、直前のステートを返す
	ButtonState SetActive();
	//無効状態にし、直前のステートを返す
	ButtonState SetDisable();
	//通常状態にし、直前のステートを返す
	ButtonState SetNormal();
	//押している状態にし、直前のステートを返す
	ButtonState SetPushing();


	//現在アクティブか調べる
	bool IsActive();
	//現在無効状態か調べる
	bool IsDisable();
	//現在通常時(選択も無効もされていない)か調べる
	bool IsNormal();
	//現在か押されているか調べる
	bool IsPushing();
	//親UIを渡して子要素に適用させる
	virtual void Render(MSSprite2DRender& aRender, UIBase*aParent = nullptr)override;
private:
	//現在の状態のスプライトリソースを取得する
	MSSprite2DResource* GetSelecttingImage();

private:
	//画像リソース
	//通常時(非選択時)
	MSSprite2DResource mImageNormal;
	//選択時
	MSSprite2DResource mImageActive;
	//押したとき
	MSSprite2DResource mImagePush;
	//無効な時
	MSSprite2DResource mImageDisable;

	//ボタンの状態
	ButtonState mState;

	//ボタンのサイズ
	DXVector2 mSize;
};