#pragma once
#include"MSCollisionBase.h"

//線分のレイとメッシュの衝突判定
class MSCollisionRayPlane :public MSCollisionBase {
public:

	MSCollisionRayPlane();
	~MSCollisionRayPlane();
	//オブジェクトから指定方向にレイを飛ばす
	/*
		レイを飛ばすリソース(向いている方向に飛ばす)
	*/
	bool Collision(
		DX11RenderResource&pRayPosition,
		const DXVector3& pRayLength,
		DX11RenderResource&pRayTarget,
		MSFbxManager&pFbxTarget
		);
private:

	bool IsInside(
		 DXVector3&pvI,
		 DXVector3&pvA,
		 DXVector3&pvB,
		 DXVector3&pvC
		);

	DXVector3* vPoint;
	DXVector3* vDirection;
	DXVector3* vPos;
	float fYaw;
};