#pragma once
#include<memory>
#include"DXVector3.h"
#include"DXMatrix.h"
//�Փ˔���֌W
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

//���E������p
class MSCollisionSphere :public MSCollisionBase {
public:
	MSCollisionSphere();
	float mRadius;//�����蔻�蔼�a
	std::unique_ptr<DXVector3>mCenter;//���S���W
	//�w�肵��FBX���b�V�����g���ċ��E�����쐬����
	void CreateCollision(const FBXModelData&pMeshmconst,const DXMatrix&pGlobalPosition);
	//���E���Ɠ����蔻����s��
	bool Collision(MSCollisionSphere&pCollision);
};
