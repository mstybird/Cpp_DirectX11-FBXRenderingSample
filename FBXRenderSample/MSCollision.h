#pragma once
#include<memory>
#include"DXVector3.h"
#include"DXMatrix.h"
//衝突判定関係
//class DXVector3;
class MSFbxManager;
struct FBXModelData;
class DXMatrix;
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
	float mRadius;//当たり判定半径
	std::unique_ptr<DXVector3>mCenter;//中心座標
	//指定したFBXメッシュを使って境界球を作成する
	void CreateCollision(const FBXModelData&pMeshmconst,const DXMatrix&pGlobalPosition);
	//境界球と当たり判定を行う
	bool Collision(MSCollisionSphere&pCollision);
};
