#pragma once
#include"MSCollisionBase.h"
#include<DXMath.hpp>
//‹«ŠE‹…”»’è—p
class MSCollisionSphere :public MSCollisionBase {
public:
	MSCollisionSphere();
	//Žw’è‚µ‚½FBXƒƒbƒVƒ…‚ðŽg‚Á‚Ä‹«ŠE‹…‚ðì¬‚·‚é
	void CreateCollision(const FBXModelData&pMeshmconst, const DXMatrix&pGlobalPosition);
	//‹«ŠE‹…‚Æ“–‚½‚è”»’è‚ðs‚¤
	static bool Collision(
		MSCollisionSphere&pCollision1,
		const DXWorld&pWorldMatrix1,
		MSCollisionSphere&pCollision2,
		const DXWorld&pWorldMatrix2
		);
	float GetRadius()const;
	void GetCenterPosition(DXVector3&pOutCenter)const;
private:
	float mRadius;//“–‚½‚è”»’è”¼Œa
	DXVector3 mCenter;//’†SÀ•W
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