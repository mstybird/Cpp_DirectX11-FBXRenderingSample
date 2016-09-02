#pragma once
#include<d3dx9.h>

class DXProjection {
public:
	DXProjection();
	static void SetAspect(float pWidth, float pHeight);

	///視野角
	void SetViewAngle(float pAngle);
	void AddViewAngle(float pAngle);

	///視野(ビュープレーン手前)
	void SetPlaneNear(float pNearPlane);
	void AddPlaneNear(float pNearPlane);

	///視野(ビュープレーン奥)
	void SetPlaneFar(float pFarPlane);
	void AddPlaneFar(float pFarPlane);

	D3DXMATRIX*GetMatrix();

	float mAngle;//視野角
	float mNear;	//視推台の手前
	float mFar;		//視推台の一番奥
	D3DXMATRIX mMatrix;

	static float mAspect;


};