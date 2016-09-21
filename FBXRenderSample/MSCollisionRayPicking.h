#pragma once
#include"MSCollisionBase.h"
//レイピッキング判定用
class MSCollisionRayPicking :public MSCollisionBase {
public:

	MSCollisionRayPicking();

	//現在の位置を記憶する
	void SetFramePosition(DX11RenderResource&pRayPosion);
	//すべらせる場合はtrueを与える
	void SetSlipFlag(bool pSlipFlag);
	//現在の滑りフラグを取得
	bool GetSlipFlag();

	//直前のフレームの位置も使ってレイピッキング判定
	/*
	pResultPos:	SlipFlag=true	:滑った後の最終的な座標
	SlipFlag=false	:衝突位置を返すのみ
	pRayAfterPos:ターゲットに衝突予定のリソース
	pRayTarget:ターゲットとなるリソース
	pFbxTarget:ターゲットとなるFBXデータ
	*/
	bool Collision(
		DXVector3&pResultPosition,
		DX11RenderResource&pRayPosition,
		DX11RenderResource&pRayTarget,
		MSFbxManager&pFbxTarget
		);
private:
	//滑らせるかどうか
	bool mSlipFlag;
	//直前のフレームの座標
	std::unique_ptr<DXVector3>mPosBefore;
};