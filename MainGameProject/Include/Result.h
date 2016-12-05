#pragma once
#include"MSSprite2DResource.h"
#include"Graphic_Text.h"
#include"Toggle.h"
#include"Button.h"
#include"StatusField.h"
struct ResultValue {
	IssueFlag mIssue;
	int mWhiteScore;
	int mBlackScore;
};

class DX11TextureManager;
class MSSprite2DRender;
class MySceneResult {
public:
	MySceneResult();
	~MySceneResult();
	void SetFrameTexture(DX11TextureManager * aManager, const int aFrameID);
	void SetLogoTexture(DX11TextureManager * aManager, const int aWinID, const int aLoseID, const int aDrawID);
	void SetButtonRetryTexture(DX11TextureManager * aManager, const int aNormalID, const int aActiveID, const int aPushID, const int aDisableID);
	void SetButtonToTitleTexture(DX11TextureManager * aManager, const int aNormalID, const int aActiveID, const int aPushID, const int aDisableID);

	void SetFramePosition(const float aX, const float aY);
	void SetLogoPosition(const float aX, const float aY);
	void SetButtonPosition(const float aX, const float aY);
	void SetTextPosition(const float aX, const float aY);

	void SetFrameSize(const float aX, const float aY);
	void SetLogoSize(const float aX, const float aY);
	void SetButtonSize(const float aX, const float aY);

	void SetButtonPadding(const float aX, const float aY);

	//リザルトに使うデータを初期化する
	void SetValues(const ResultValue& aValue);
	//リザルト描画
	void Render(MSSprite2DRender&aRender);

	//ボタン操作(前へ)
	void ButtonBack();
	//ボタン操作(次へ)
	void ButtonNext();
	//ボタンを押す
	void ButtonPush();
	//ボタンを離す
	void ButtonUp();
	//アクティブなボタンのインデクスを取得する
	int GetButtonActiveIndex();

private:
	TextManager mTextMan;
	//フレーム画像
	MSSprite2DResource mFrame;
	//勝利ロゴ
	MSSprite2DResource mLogoWin;
	//引き分けロゴ
	MSSprite2DResource mLogoDraw;
	//敗北ロゴ
	MSSprite2DResource mLogoLose;
	//使用するロゴ
	MSSprite2DResource* mActiveLogo;

	//スコアテキスト
	std::shared_ptr<TextGraphic>mTextScore;
	DXVector2 mTextPos;

	//ロゴ情報
	DXVector2 mLogoPos;
	DXVector2 mLogoSize;

	Toggle mButtonList;
	Button mButtonRetry;
	Button mButtonToTitle;

};