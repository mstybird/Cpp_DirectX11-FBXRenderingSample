#include "MSUtility.h"
#include"DX11RenderResource.h"
#include"DXWorld.h"
#include"DXMatrix.h"
#include"DXVector3.h"
#include<memory>

//YŽ²‰ñ“]ŠpŒvŽZ
//‰ñ“]Šp‚Í•K‚¸ApDegree>ÅI‰ñ“]Šp‚Æ‚È‚é
float MSHormingY(DX11RenderResource & pEyeResource, DX11RenderResource & pTargetResource, const float pDegree)
{
	//Ž‹“_ƒŠƒ\[ƒX‚ÌŠp“x‚ð‹‚ß‚é
	DXVector3 lEyeDir;
	float lEyeRot;
	D3DXVec3TransformNormal(&lEyeDir, &DXVector3{ 0,0,1 }, pEyeResource.GetWorld().lock()->GetMatrix().lock().get());
	lEyeDir.Normalize();
	lEyeRot = D3DXVec3Dot(&DXVector3(0, 0, 1), &lEyeDir);
	lEyeRot = D3DXToDegree(acos(lEyeRot));
	if (lEyeDir.x < 0) { lEyeRot *= -1; }

	//Ž‹“_‚©‚çƒ^[ƒQƒbƒg‚Ö‚ÌŠp“x‚ð‹‚ß‚é
	DXVector3 lTargetPos, lEyePos;
	//ƒ^[ƒQ‚Æ
	pTargetResource.GetWorld().lock()->GetMatrix().lock()->GetT(lTargetPos);
	//Ž©g
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

//YŽ²‰ñ“]ŠpŒvŽZ
//‰ñ“]Šp‚Í•K‚¸ApDegree>ÅI‰ñ“]Šp‚Æ‚È‚é
float MSHormingY(DX11RenderResource & pEyeResource, DXVector3 & pTargetPos, const float pDegree)
{
	//Ž‹“_ƒŠƒ\[ƒX‚ÌŠp“x‚ð‹‚ß‚é
	DXVector3 lEyeDir;
	float lEyeRot;
	D3DXVec3TransformNormal(&lEyeDir, &DXVector3{ 0,0,1 }, pEyeResource.GetWorld().lock()->GetMatrix().lock().get());
	lEyeDir.Normalize();
	lEyeRot = D3DXVec3Dot(&DXVector3(0, 0, 1), &lEyeDir);
	lEyeRot = D3DXToDegree(acos(lEyeRot));
	if (lEyeDir.x < 0) { lEyeRot *= -1; }

	//Ž‹“_‚©‚çƒ^[ƒQƒbƒg‚Ö‚ÌŠp“x‚ð‹‚ß‚é
	DXVector3 lTargetPos, lEyePos;
	//ƒ^[ƒQ‚Æ
	lTargetPos = pTargetPos;
	//Ž©g
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

//Threshold–¢–ž‚Ì’l‚ð0‚Æ‚µ‚ÄŒvŽZ‚·‚é
bool IsZero(float pValue, float Threshold)
{
	Threshold = fabsf(Threshold);
	pValue = fabsf(pValue);
	return pValue<Threshold;
}

//pRyr‚©‚çŒ©‚½‘Š‘Î“I‚ÈŠp“x·‚ð‹‚ß‚é
float MSGetToRotateY(DX11RenderResource & pEyeResource, DX11RenderResource & pTargetResource)
{
	//Ž‹“_ƒŠƒ\[ƒX‚ÌŠp“x‚ð‹‚ß‚é
	DXVector3 lEyeDir;
	float lEyeRot;
	D3DXVec3TransformNormal(&lEyeDir, &DXVector3{ 0,0,1 }, pEyeResource.GetWorld().lock()->GetMatrix().lock().get());
	lEyeDir.Normalize();
	lEyeRot = D3DXVec3Dot(&DXVector3(0, 0, 1), &lEyeDir);
	lEyeRot = D3DXToDegree(acos(lEyeRot));
	if (lEyeDir.x < 0) { lEyeRot *= -1; }

	//Ž‹“_‚©‚çƒ^[ƒQƒbƒg‚Ö‚ÌŠp“x‚ð‹‚ß‚é
	DXVector3 lTargetPos, lEyePos;
	//ƒ^[ƒQ‚Æ
	pTargetResource.GetWorld().lock()->GetMatrix().lock()->GetT(lTargetPos);
	//Ž©g
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

