#pragma once
#include<d3dx9.h>

class DXProjection {
public:
	DXProjection();
	static void SetAspect(float pWidth, float pHeight);

	///����p
	void SetViewAngle(float pAngle);
	void AddViewAngle(float pAngle);

	///����(�r���[�v���[����O)
	void SetPlaneNear(float pNearPlane);
	void AddPlaneNear(float pNearPlane);

	///����(�r���[�v���[����)
	void SetPlaneFar(float pFarPlane);
	void AddPlaneFar(float pFarPlane);

	D3DXMATRIX*GetMatrix();

	float mAngle;//����p
	float mNear;	//������̎�O
	float mFar;		//������̈�ԉ�
	D3DXMATRIX mMatrix;

	static float mAspect;


};