#pragma once
#include"MSCollisionBase.h"

//���E������p
class MSCollisionSphere :public MSCollisionBase {
public:
	MSCollisionSphere();
	//�w�肵��FBX���b�V�����g���ċ��E�����쐬����
	void CreateCollision(const FBXModelData&pMeshmconst, const DXMatrix&pGlobalPosition);
	//���E���Ɠ����蔻����s��
	static bool Collision(
		MSCollisionSphere&pCollision1,
		const DXWorld&pWorldMatrix1,
		MSCollisionSphere&pCollision2,
		const DXWorld&pWorldMatrix2
		);
	float GetRadius()const;
	void GetCenterPosition(DXVector3&pOutCenter)const;
private:
	float mRadius;//�����蔻�蔼�a
	std::shared_ptr<DXVector3>mCenter;//���S���W
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