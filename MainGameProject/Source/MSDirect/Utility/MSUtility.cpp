#include "MSUtility.h"
#include"DX11RenderResource.h"
#include"DXWorld.h"
#include"DXMatrix.h"
#include"DXVector3.h"
#include<memory>

//Y����]�p�v�Z
//��]�p�͕K���ApDegree>�ŏI��]�p�ƂȂ�
float MSHormingY(DX11RenderResource & pEyeResource, DX11RenderResource & pTargetResource, const float pDegree)
{
	//���_���\�[�X�̊p�x�����߂�
	DXVector3 lEyeDir;
	float lEyeRot;
	D3DXVec3TransformNormal(&lEyeDir, &DXVector3{ 0,0,1 }, pEyeResource.GetWorld().lock()->GetMatrix().lock().get());
	lEyeDir.Normalize();
	lEyeRot = D3DXVec3Dot(&DXVector3(0, 0, 1), &lEyeDir);
	lEyeRot = D3DXToDegree(acos(lEyeRot));
	if (lEyeDir.x < 0) { lEyeRot *= -1; }

	//���_����^�[�Q�b�g�ւ̊p�x�����߂�
	DXVector3 lTargetPos, lEyePos;
	//�^�[�Q��
	pTargetResource.GetWorld().lock()->GetMatrix().lock()->GetT(lTargetPos);
	//���g
	pEyeResource.GetWorld().lock()->GetMatrix().lock()->GetT(lEyePos);
	lEyePos = lEyePos - lTargetPos;
	lEyePos.Normalize();
	float lToTargetRot = D3DXVec3Dot(&DXVector3(0, 0, 1), &lEyePos);
	lToTargetRot = D3DXToDegree(acos(lToTargetRot));
	if (lEyePos.x < 0) { lToTargetRot *= -1; }

	float lDiffAngle = lToTargetRot - lEyeRot;

	if (lDiffAngle < -180) {
		lDiffAngle += 360;
	}
	if (lDiffAngle > 180) {
		lDiffAngle -= 360;
	}

	float lResultDegree;
	if (lDiffAngle > 0) {
		if (180 - lDiffAngle < pDegree && 180 - lDiffAngle>-pDegree) {
			lResultDegree = -fabsf(lDiffAngle - 180);
		}
		else {
			lResultDegree = -pDegree;
		}
	}
	else {
		if (180 + lDiffAngle > -pDegree && 180 + lDiffAngle < pDegree) {
			lResultDegree = fabsf(lDiffAngle + 180);
		}
		else {
			lResultDegree = pDegree;
		}

	}

	return lResultDegree;
}

//Y����]�p�v�Z
//��]�p�͕K���ApDegree>�ŏI��]�p�ƂȂ�
float MSHormingY(DX11RenderResource & pEyeResource, DXVector3 & pTargetPos, const float pDegree)
{
	//���_���\�[�X�̊p�x�����߂�
	DXVector3 lEyeDir;
	float lEyeRot;
	D3DXVec3TransformNormal(&lEyeDir, &DXVector3{ 0,0,1 }, pEyeResource.GetWorld().lock()->GetMatrix().lock().get());
	lEyeDir.Normalize();
	lEyeRot = D3DXVec3Dot(&DXVector3(0, 0, 1), &lEyeDir);
	lEyeRot = D3DXToDegree(acos(lEyeRot));
	if (lEyeDir.x < 0) { lEyeRot *= -1; }

	//���_����^�[�Q�b�g�ւ̊p�x�����߂�
	DXVector3 lTargetPos, lEyePos;
	//�^�[�Q��
	lTargetPos = pTargetPos;
	//���g
	pEyeResource.GetWorld().lock()->GetMatrix().lock()->GetT(lEyePos);
	lEyePos = lEyePos - lTargetPos;
	lEyePos.Normalize();
	float lToTargetRot = D3DXVec3Dot(&DXVector3(0, 0, 1), &lEyePos);
	lToTargetRot = D3DXToDegree(acos(lToTargetRot));
	if (lEyePos.x < 0) { lToTargetRot *= -1; }

	float lDiffAngle = lToTargetRot - lEyeRot;

	if (lDiffAngle < -180) {
		lDiffAngle += 360;
	}
	if (lDiffAngle > 180) {
		lDiffAngle -= 360;
	}

	float lResultDegree;
	if (lDiffAngle > 0) {
		if (180 - lDiffAngle < pDegree && 180 - lDiffAngle>-pDegree) {
			lResultDegree = -fabsf(lDiffAngle - 180);
		}
		else {
			lResultDegree = -pDegree;
		}
	}
	else {
		if (180 + lDiffAngle > -pDegree && 180 + lDiffAngle < pDegree) {
			lResultDegree = fabsf(lDiffAngle + 180);
		}
		else {
			lResultDegree = pDegree;
		}

	}

	return lResultDegree;
}

//Threshold�����̒l��0�Ƃ��Čv�Z����
bool IsZero(float pValue, float Threshold)
{
	Threshold = fabsf(Threshold);
	pValue = fabsf(pValue);
	return pValue<Threshold;
}

//pRyr���猩�����ΓI�Ȋp�x�������߂�
float MSGetToRotateY(DX11RenderResource & pEyeResource, DX11RenderResource & pTargetResource)
{
	//���_���\�[�X�̊p�x�����߂�
	DXVector3 lEyeDir;
	float lEyeRot;
	D3DXVec3TransformNormal(&lEyeDir, &DXVector3{ 0,0,1 }, pEyeResource.GetWorld().lock()->GetMatrix().lock().get());
	lEyeDir.Normalize();
	lEyeRot = D3DXVec3Dot(&DXVector3(0, 0, 1), &lEyeDir);
	lEyeRot = D3DXToDegree(acos(lEyeRot));
	if (lEyeDir.x < 0) { lEyeRot *= -1; }

	//���_����^�[�Q�b�g�ւ̊p�x�����߂�
	DXVector3 lTargetPos, lEyePos;
	//�^�[�Q��
	pTargetResource.GetWorld().lock()->GetMatrix().lock()->GetT(lTargetPos);
	//���g
	pEyeResource.GetWorld().lock()->GetMatrix().lock()->GetT(lEyePos);
	lEyePos = lEyePos - lTargetPos;
	lEyePos.Normalize();
	float lToTargetRot = D3DXVec3Dot(&DXVector3(0, 0, 1), &lEyePos);
	lToTargetRot = D3DXToDegree(acos(lToTargetRot));
	if (lEyePos.x < 0) { lToTargetRot *= -1; }

	float lDiffAngle = lToTargetRot - lEyeRot;

	if (lDiffAngle < -180) {
		lDiffAngle += 360;
	}
	if (lDiffAngle > 180) {
		lDiffAngle -= 360;
	}

	float lResultDegree;
	if (lDiffAngle > 0) {
			lResultDegree = -fabsf(lDiffAngle - 180);
	}
	else {
			lResultDegree = fabsf(lDiffAngle + 180);
	}

	return lResultDegree;
}

