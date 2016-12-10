#pragma once
#include"MSCollisionBase.h"
class MSFbxObject;
class GameObjectBase;
//レイ判定基底用
class MSCollisionRay :public MSCollisionBase {
public:
	MSCollisionRay();
	//指定した方向にレイを飛ばす
	void SetRay(const DXVector3&pRayDirection);
	//カメラが向いている方向でレイを作成
	void SetRay(const DXCamera&pRayCamera);

	/*
	レイ判定
	pRay			:
	pRayPosition	:レイ発射位置を含むリソース
	pResource		:ターゲットのリソース
	pFbxManager		:ターゲットのメッシュが含まれるFBXマネージャ
	*/
	static bool Collision(
		const MSCollisionRay&pRay,
		DX11RenderResource&pRayPosition,
		DX11RenderResource&pResource,
		MSFbxObject&pFbxManager
		);

private:
	//レイ発射方向
	std::shared_ptr<DXVector3>mRayDirection;
};

class MS2CollisionRay {
	static float Collision(
		DXCamera*aCamera,
		GameObjectBase*aCollision
	);
};