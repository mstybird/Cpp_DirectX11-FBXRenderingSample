#pragma once
#include"MSSprite2DResource.h"
#include"Graphic_Text.h"
#include"Toggle.h"
#include"Button.h"
#include"StatusField.h"
#include<DXAL.hpp>

namespace ValueResult {
	//ID:10000~19999
	namespace Sound {
		static const int cLuaID = 10000;
		
		static const char* cLuaPath = "Resource/Script/ResultSound.lua";
		static const char* cTimeUpPath = "SETimeUpPath";
		static const char* cSelectPath = "SESelectPath";
		static const char* cEnterPath = "SEEnterPath";
		static const char* cBGMPath = "BGMPath";
	}
}

struct ResultValue {
	IssueFlag mIssue;
	int mWhiteScore;
	int mBlackScore;
};

class DX11TextureManager;
class MSSprite2DRender;
class NcgLuaDatabase;
class MySceneResult {
public:
	MySceneResult();
	~MySceneResult();
	void InitializeSound(NcgLuaDatabase& aDb);
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

	//タイムアップSEを鳴らす
	void PlaySETimeUp();
	//BGMを再生する
	void PlayBGM();



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
	//ボタンリストのトグル
	Toggle mButtonList;
	//リトライボタン
	Button mButtonRetry;
	//タイトルへ戻るボタン
	Button mButtonToTitle;
	//サウンドデバイス
	SoundDevice mSoundDevice;
	//リザルトBGM
	SoundPlayer mBGM;
	//決定音
	SoundPlayer mSEEnter;
	//選択音
	SoundPlayer mSESelect;
	//時間切れの音
	SoundPlayer mSETimeUp;


};