#pragma once
#include<memory>
class DXMatrix;
class DXProjection {
public:
	DXProjection();
	~DXProjection();
	static void SetAspect(float pWidth, float pHeight);

	//すべてセットする
	void SetProjection(float pAngle, float pNearPlane, float pFarPlane);

	///視野角
	void SetViewAngle(float pAngle);
	void AddViewAngle(float pAngle);

	///視野(ビュープレーン手前)
	void SetPlaneNear(float pNearPlane);
	void AddPlaneNear(float pNearPlane);

	///視野(ビュープレーン奥)
	void SetPlaneFar(float pFarPlane);
	void AddPlaneFar(float pFarPlane);

	void Clone(DXProjection&pOutClone);

	std::weak_ptr<DXMatrix>GetMatrix();

	float mAngle;//視野角
	float mNear;	//視推台の手前
	float mFar;		//視推台の一番奥
	std::shared_ptr<DXMatrix>mMatrix;

	static float mAspect;


};