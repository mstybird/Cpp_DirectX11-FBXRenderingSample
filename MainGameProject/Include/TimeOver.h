#pragma once
#include"MSSprite2DResource.h"
#include"StatusField.h"
enum class SceneTimeOverSequence {
	FirstWait,
	Finishing,
	FinishedWait,
	Issuing,
	IssuedWait,

};

class DX11TextureManager;
class MSSprite2DRender;
class MySceneTimeOver
{

public:
	MySceneTimeOver();
	~MySceneTimeOver();
	//テクスチャの設定
	void SetTextures(DX11TextureManager * aManager, const int aFinishID, const int aWinID, const int aLoseID,const int aDrawID);
	//タイムリミットロゴの位置
	void SetFinishPosition(const DXVector2&aPosition);
	//タイムリミットロゴのサイズ
	void SetFinishSize(const DXVector2&aSize);
	//タイムリミットロゴのスケール
	void SetFinishScale(const DXVector2&aScale);
	//勝敗ロゴの位置
	void SetIssuePosition(const DXVector2&aPosition);
	//勝敗ロゴのサイズ
	void SetIssueSize(const DXVector2&aSize);
	//勝敗ロゴのスケール
	void SetIssueScale(const DXVector2&aScale);
	//Updateを開始する(勝利したかどうかを決定させる)
	void UpdateStart(const IssueFlag aIsWin);

	//タイムオーバー処理の更新
	SceneTimeOverSequence Update();

	//まだ更新中か(Updateを終了してもいいかどうか)
	bool IsUpdateing();

	//描画
	void Render(MSSprite2DRender& aRender);
private:
	//フィニッシュロゴ表示前の待機画面
	void UpdateFirstWait();
	//フィニッシュロゴ表示中
	void UpdateFinishing();
	//フィニッシュロゴ表示後の待機
	void UpdateFinishedWait();
	//勝敗表示
	void UpdateIssuing();
	//勝敗表示後
	void UpdateIssuedWait();

private:
	MSSprite2DResource mLogoFinish;
	MSSprite2DResource mLogoWin;
	MSSprite2DResource mLogoDraw;
	MSSprite2DResource mLogoLose;

	MSSprite2DResource* mActiveLogo;

	DXVector2 mIssuePosition;
	DXVector2 mIssueSize;
	DXVector2 mIssueScale;

	SceneTimeOverSequence mSequence;

	//カウンタ
	int mFrameCounter;
	//アップデート中のフラグ
	bool mIsUpdating;
	//勝利フラグ
	IssueFlag mIsWin;
};
