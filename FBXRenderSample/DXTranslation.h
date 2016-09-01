#pragma once
#include"DXVector3.h"
//ワールド空間座標系クラス
class DXWorld {
public:
	DXWorld();
	void SetT(float pX, float pY, float pZ);
	void SetT(D3DXVECTOR3 pPosition);

	void SetRC(float pX, float pY, float pZ);
	void SetRC(D3DXVECTOR3 pPosition);
	
	void SetRT(float pX, float pY, float pZ);
	void SetRT(D3DXVECTOR3 pPosition);
	
	void SetS(float pX, float pY, float pZ);
	void SetS(D3DXVECTOR3 pPosition);

	D3DXMATRIX* GetMatrix();
	DXVector3 mPosition;			//位置
	DXVector3 mRotationCenter;	//回転(移動行列適用前)
	DXVector3 mRotationTransed;	//回転移動行列適用後)
	DXVector3 mScale;				//拡大率

	D3DXMATRIX mMatrix;			//計算用

	D3DXMATRIX* operator*() {
		return GetMatrix();
	}

};
