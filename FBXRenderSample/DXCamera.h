#pragma once
#include"DXVector3.h"
#include"DXWorld.h"
//class DX11RenderResource;
class DXCamera {
public:
	//カメラの移動タイプ
	enum TYPEMOVE {
		TYPE_PARALLEL = 0,	//平行移動
		TYPE_TARGET = 1,	//回転移動
		TYPE_ROTATE = 2		//現在向いている向きに移動
	};

	
	///カメラの位置
	void SetEyeT(float pX, float pY, float pZ);
	void SetEyeT(D3DXVECTOR3 pPosition);
	//ワールド行列から位置取得
	void SetEyeT(DXWorld pEyeTarget);
	
	///カメラの注視点
	void SetLookT(float pX, float pY, float pZ);
	void SetLookT(D3DXVECTOR3 pLookAt);
	//ワールド行列から注視点取得
	void SetLookT(DXWorld pEyeTarget);

	//カメラの移動
	void AddEyeT(float pSpeed, DXVector3 pDirection = { 0,0,1 });
	void AddLookT(float pSpeed, DXVector3 pDirection = { 0,0,1 });
	//平行移動
	void AddTranslation(TYPEMOVE pType,float pSpeed, DXVector3 pDirection = { 0,0,1 });
	//カメラの回転
	void Rotate(float pX, float pY, float pZ);

	///カメラの向き
	void SetUpV(float pX, float pY, float pZ);
	void SetUpV(D3DXVECTOR3 pVector);

	D3DXMATRIX*GetMatrix();

	DXVector3 mEyePosition;	//カメラの位置
	DXVector3 mLookPosition;//注視点
	DXVector3 mUpVector;	//頭上方向
	DXVector3 mRotate;		//カメラの角度
	D3DXMATRIX mMatrix;

	D3DXMATRIX* operator*() {
		return GetMatrix();
	}
private:
	
};