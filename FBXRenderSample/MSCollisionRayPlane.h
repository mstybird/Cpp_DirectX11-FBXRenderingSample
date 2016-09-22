#pragma once
#include"MSCollisionBase.h"

//線分のレイとメッシュの衝突判定
class MSCollisionRayPlane :public MSCollisionBase {
public:


	struct T_HitPolygon {

	};

	MSCollisionRayPlane();
	~MSCollisionRayPlane();
	//オブジェクトから指定方向にレイを飛ばす
	/*
		レイを飛ばすリソース(向いている方向に飛ばす)
		pRayPosition	:	レイを発射する位置を含むリソース
		pRayLength		:	レイの方向と長さ
		pRayTarget		:	レイを当てるターゲットのリソース
		pFbxTarget		:	レイを当てるターゲットのメッシュ
		pOutDistance	:	レイが当たった場合、一番近いポリゴンとの距離を返す
	*/
	bool Collision(
		DX11RenderResource&pRayPosition,
		const DXVector3& pRayLength,
		DX11RenderResource&pRayTarget,
		MSFbxManager&pFbxTarget,
		float& pOutDistance,
		DXVector3&pNormal
		);

	//記憶している座標から移動した方向に向かってレイを飛ばす
	/*
		レイを飛ばすリソース(向いている方向に飛ばす)
		pRayPosition	:	レイを発射する位置を含むリソース
		pRayLength		:	レイの方向と長さ
		pRayTarget		:	レイを当てるターゲットのリソース
		pFbxTarget		:	レイを当てるターゲットのメッシュ
		pOutDistance	:	レイが当たった場合、一番近いポリゴンとの距離を返す

	*/
	bool Collision(DX11RenderResource&pRayPosition,
		const float& pRayLength,
		DX11RenderResource&pRayTarget,
		MSFbxManager&pFbxTarget,
		float& pOutDistance,
		DXVector3&pOutNormal
	);

	//方向ベクトルを計算するためにフレームの位置を記憶させる
	void SetFramePosition(DX11RenderResource&pRayPosition);

	//滑らせるリソース
	void Slip(DXVector3&pOutSlipVector, DX11RenderResource&pUpdPosition,DXVector3&pNormal);

private:

	//有限遠平面判定をする
	bool IsInside(
		 DXVector3&pvI,
		 DXVector3&pvA,
		 DXVector3&pvB,
		 DXVector3&pvC
		);

	std::unique_ptr<DXVector3> mFramePosition;
	//最後に当たったポリゴンの法線
//	std::unique_ptr<DXVector3> mLastHitNormal;
};

//レイ判定で距離を取得する