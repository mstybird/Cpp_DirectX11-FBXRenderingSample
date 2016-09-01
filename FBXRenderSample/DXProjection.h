#pragma once
#include<d3dx9.h>

class DXProjection {
public:
	static void SetAspect(float pWidth, float pHeight);

	void SetViewAngle(float pAngle);
	void SetPlaneNear(float pNearPlane);
	void SetPlaneFar(float pFarPlane);
	void AddViewAngle(float pAngle);
	void AddPlaneNear(float pNearPlane);
	void AddPlaneFar(float pFarPlane);

	D3DXMATRIX*GetMatrix();
	float mAngle;//‹–ìŠp
	float mNear;	//‹„‘ä‚Ìè‘O
	float mFar;		//‹„‘ä‚Ìˆê”Ô‰œ
	static float mAspect;

};