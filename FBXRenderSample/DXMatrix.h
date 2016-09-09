#pragma once
#include<d3dx9.h>

//class DXCamera;
class DXVector3;
enum class TYPE_ANGLE;
class DXMatrix :public D3DXMATRIX {
public:

	DXMatrix();
	void SetIdentity();
	bool IsIdentity();
	void Inverse();
	void SetLookAtLH(
		const DXVector3&pEyePosition,
		const DXVector3&pLookPosition,
		const DXVector3&pUpVector
		);
	void SetLookAtRH(
		const DXVector3&pEyePosition,
		const DXVector3&pLookPosition,
		const DXVector3&pUpVector

		);
	void Multiply(const DXMatrix&pMultipleMatrix);
	void MultiplyTranspose(const DXMatrix&pMultipleMatrix);
	void RotationAxis(const DXVector3&pVector, const float pAngle);
	void RotationQuaternion(const D3DXQUATERNION&pRotateQ);
	void RotationX(float pAngle, TYPE_ANGLE pType);
	void RotationY(float pAngle, TYPE_ANGLE pType);
	void RotationZ(float pAngle, TYPE_ANGLE pType);
	void RotationXYZ(const DXVector3&pAngleXYZ,TYPE_ANGLE pType);
	void Translation(const DXVector3&pVector);
	void Transpose();
	DXMatrix Clone();
	const DXMatrix*GetPtr();

	DXMatrix&operator=(const D3DXMATRIX&pMatrix);
	DXMatrix&operator*(const DXMatrix&pMatrix);

};