#pragma once

class DXMatrix;
class DXProjection {
public:
	DXProjection();
	~DXProjection();
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

	DXMatrix*GetMatrix();

	float mAngle;//����p
	float mNear;	//������̎�O
	float mFar;		//������̈�ԉ�
	DXMatrix*mMatrix;

	static float mAspect;


};