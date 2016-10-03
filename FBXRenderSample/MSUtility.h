#pragma once
class DX11RenderResource;
class DXVector3;
//追従処理。追従角度を指定し、指定角度外だった場合、追従角度がそのまま戻り値となる
//戻り値はディグリー
float MSHormingY(DX11RenderResource&pEyeResource, DX11RenderResource&pTargetResource, const float pDegree = 180);

//floatのゼロ判定を行う
bool IsZero(float pValue, float Threshold=0.0000001);


//視点とターゲットとの相対角度を取得する
float MSGetToRotateY(DX11RenderResource&pEyeResource, DX11RenderResource&pTargetResource);

/*
	やること。
	チェックポイントを利用した誘導(順番にたどる)
*/