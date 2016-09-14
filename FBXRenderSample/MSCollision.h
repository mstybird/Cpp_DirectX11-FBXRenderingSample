#pragma once
#include<memory>
#include"DXVector3.h"
#include"DXMatrix.h"
//衝突判定関係
//class DXVector3;
class MSFbxManager;
struct FBXModelData;
class DXMatrix;
class DXWorld;
class DXCamera;
class DX11RenderResource;
class MSCollisionBase {
public:
	MSCollisionBase();
	MSCollisionBase(const MSCollisionBase& _co);
	~MSCollisionBase();
protected:
	std::unique_ptr<DXMatrix>mGlobalPosition;
};

//境界球判定用
class MSCollisionSphere :public MSCollisionBase {
public:
	MSCollisionSphere();
	//指定したFBXメッシュを使って境界球を作成する
	void CreateCollision(const FBXModelData&pMeshmconst,const DXMatrix&pGlobalPosition);
	//境界球と当たり判定を行う
	static bool Collision(
		MSCollisionSphere&pCollision1,
		const DXWorld&pWorldMatrix1,
		MSCollisionSphere&pCollision2,
		const DXWorld&pWorldMatrix2
		);
private:
	float mRadius;//当たり判定半径
	std::unique_ptr<DXVector3>mCenter;//中心座標
};

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
		MSFbxManager&pFbxManager
		);

private:
	//レイ発射方向
	std::shared_ptr<DXVector3>mRayDirection;
};

//