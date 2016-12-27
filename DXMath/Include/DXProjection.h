#pragma once
#include<memory>
#include"DXMatrix.h"
class DXProjection {
public:
	DXProjection();
	~DXProjection();
	static void SetAspect(float pWidth, float pHeight);

	//���ׂăZ�b�g����
	void SetProjection(float pAngle, float pNearPlane, float pFarPlane);

	///����p
	void SetViewAngle(float pAngle);
	void AddViewAngle(float pAngle);

	///����(�r���[�v���[����O)
	void SetPlaneNear(float pNearPlane);
	void AddPlaneNear(float pNearPlane);

	///����(�r���[�v���[����)
	void SetPlaneFar(float pFarPlane);
	void AddPlaneFar(float pFarPlane);

	void Clone(DXProjection&pOutClone);

	DXMatrix* GetMatrix();

	float mAngle;//����p
	float mNear;	//������̎�O
	float mFar;		//������̈�ԉ�
	DXMatrix mMatrix;	//�v�Z�p

	static float mAspect;


};