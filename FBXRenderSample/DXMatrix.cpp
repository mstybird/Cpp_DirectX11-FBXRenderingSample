#include "DXMatrix.h"
#include"DXVector3.h"
#include"DX11Resrouce.h"
DXMatrix::DXMatrix()
{
	D3DXMatrixIdentity(this);
}

void DXMatrix::SetIdentity()
{
	D3DXMatrixIdentity(this);
}

bool DXMatrix::IsIdentity()
{
	return D3DXMatrixIsIdentity(this);
}

void DXMatrix::Inverse()
{
	D3DXMatrixInverse(this, nullptr, this);
}

void DXMatrix::SetLookAtLH(
	const DXVector3&pEyePosition,
	const DXVector3&pLookPosition,
	const DXVector3&pUpVector
	)
{
	D3DXMatrixLookAtLH(this, &pEyePosition, &pLookPosition, &pUpVector);
}

void DXMatrix::SetLookAtRH(
	const DXVector3&pEyePosition,
	const DXVector3&pLookPosition,
	const DXVector3&pUpVector
	)
{
	D3DXMatrixLookAtRH(this, &pEyePosition, &pLookPosition, &pUpVector);

}

void DXMatrix::Multiply(const DXMatrix&pMultipleMatrix)
{
	D3DXMatrixMultiply(this, this, &pMultipleMatrix);
	
}

void DXMatrix::MultiplyTranspose(const DXMatrix&pMultipleMatrix)
{
	D3DXMatrixMultiplyTranspose(this, this, &pMultipleMatrix);
}

void DXMatrix::RotationAxis(const DXVector3&pVector,const float pAngle)
{
	D3DXMatrixRotationAxis(this, &pVector, pAngle);
}

void DXMatrix::RotationQuaternion(const D3DXQUATERNION&pRotateQ)
{
	D3DXMatrixRotationQuaternion(this, &pRotateQ);
}

void DXMatrix::RotationX(float pAngle, TYPE_ANGLE pType)
{
	
	if (pType == TYPE_ANGLE::DEGREE) {
		pAngle = D3DXToRadian(pAngle);
	}
	D3DXMatrixRotationX(this, pAngle);
}

void DXMatrix::RotationY(float pAngle, TYPE_ANGLE pType)
{
	if (pType == TYPE_ANGLE::DEGREE) {
		pAngle = D3DXToRadian(pAngle);
	}
	D3DXMatrixRotationX(this, pAngle);

}

void DXMatrix::RotationZ(float pAngle, TYPE_ANGLE pType)
{
	if (pType == TYPE_ANGLE::DEGREE) {
		pAngle = D3DXToRadian(pAngle);
	}
	D3DXMatrixRotationX(this, pAngle);

}

void DXMatrix::RotationXYZ(const DXVector3&pAngleXYZ, TYPE_ANGLE pType)
{
}

void DXMatrix::Translation(const DXVector3&pVector)
{
	D3DXMatrixTranslation(this, pVector.x, pVector.y, pVector.z);
}

void DXMatrix::Transpose()
{
	D3DXMatrixTranspose(this, this);
}

DXMatrix DXMatrix::Clone()
{

	return DXMatrix(*this);
}

const DXMatrix * DXMatrix::GetPtr()
{
	return this;
}

DXMatrix & DXMatrix::operator=(const D3DXMATRIX & pMatrix)
{
	*this = pMatrix;
	return *this;
	// TODO: return ステートメントをここに挿入します
}

DXMatrix & DXMatrix::operator*(const DXMatrix & pMatrix)
{
	this->Multiply(pMatrix);
	return *this;
}
