#pragma once
#include<d3dx9.h>

class DXProjection {
public:
	DXProjection();
	static void SetAspect(float pWidth, float pHeight);

	void SetViewAngle(float pAngle);
	void SetPlaneNear(float pNearPlane);
	void SetPlaneFar(float pFarPlane);
	void AddViewAngle(float pAngle);
	void AddPlaneNear(float pNearPlane);
	void AddPlaneFar(float pFarPlane);

	D3DXMATRIX*GetMatrix();
	D3DXMATRIX* operator*() {
		return GetMatrix();
	}


	float mAngle;//‹–ìŠp
	float mNear;	//‹„‘ä‚Ìè‘O
	float mFar;		//‹„‘ä‚Ìˆê”Ô‰œ
	D3DXMATRIX mMatrix;

	static float mAspect;


};