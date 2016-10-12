#pragma once
#include"MSCollisionBase.h"

//境界球判定用
class MSCollisionSphere :public MSCollisionBase {
public:
	MSCollisionSphere();
	//指定したFBXメッシュを使って境界球を作成する
	void CreateCollision(const FBXModelData&pMeshmconst, const DXMatrix&pGlobalPosition);
	//境界球と当たり判定を行う
	static bool Collision(
		MSCollisionSphere&pCollision1,
		const DXWorld&pWorldMatrix1,
		MSCollisionSphere&pCollision2,
		const DXWorld&pWorldMatrix2
		);
	float GetRadius()const;
	void GetCenterPosition(DXVector3&pOutCenter)const;
private:
	float mRadius;//当たり判定半径
	std::shared_ptr<DXVector3>mCenter;//中心座標
};
/*
	A>B
	A<C
	E=A+B
	E=C+D

	A.D<B<A<C<E


	20m-300=n
	15m+400=n
	5m-700=0
	m=140

	A.2500yen


	m=100

	
	x-3000=2y-4200
	x-2y=-1200
	x=2y-1200

	x:y=2:5
	4y-1200

*/