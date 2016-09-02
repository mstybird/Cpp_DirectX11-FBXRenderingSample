#pragma once
#include"DXVector3.h"
//ワールド空間座標系クラス
class DXWorld {
public:
	DXWorld();

	///移動行列操作
	void SetT(float pX, float pY, float pZ);
	void SetT(D3DXVECTOR3 pPosition);
	void AddT(float pX, float pY, float pZ);
	void AddT(D3DXVECTOR3 pPosition);
	void SubT(float pX, float pY, float pZ);
	void SubT(D3DXVECTOR3 pPosition);
	void MulT(float pX, float pY, float pZ);
	void MulT(D3DXVECTOR3 pPosition);
	void DivT(float pX, float pY, float pZ);
	void DivT(D3DXVECTOR3 pPosition);

	///移動前の回転行列
	void SetRC(float pX, float pY, float pZ);
	void SetRC(D3DXVECTOR3 pPosition);
	void AddRC(float pX, float pY, float pZ);
	void AddRC(D3DXVECTOR3 pPosition);
	void SubRC(float pX, float pY, float pZ);
	void SubRC(D3DXVECTOR3 pPosition);
	void MulRC(float pX, float pY, float pZ);
	void MulRC(D3DXVECTOR3 pPosition);
	void DivRC(float pX, float pY, float pZ);
	void DivRC(D3DXVECTOR3 pPosition);

	//移動後の回転行列
	void SetRT(float pX, float pY, float pZ);
	void SetRT(D3DXVECTOR3 pPosition);
	void AddRT(float pX, float pY, float pZ);
	void AddRT(D3DXVECTOR3 pPosition);
	void SubRT(float pX, float pY, float pZ);
	void SubRT(D3DXVECTOR3 pPosition);
	void MulRT(float pX, float pY, float pZ);
	void MulRT(D3DXVECTOR3 pPosition);
	void DivRT(float pX, float pY, float pZ);
	void DivRT(D3DXVECTOR3 pPosition);

	//拡大行列
	void SetS(float pX, float pY, float pZ);
	void SetS(D3DXVECTOR3 pScaling);
	void AddS(float pX, float pY, float pZ);
	void AddS(D3DXVECTOR3 pScaling);
	void SubS(float pX, float pY, float pZ);
	void SubS(D3DXVECTOR3 pScaling);
	void MulS(float pX, float pY, float pZ);
	void MulS(D3DXVECTOR3 pScaling);
	void DivS(float pX, float pY, float pZ);
	void DivS(D3DXVECTOR3 pScaling);

	D3DXMATRIX* GetMatrix();
	DXVector3 mPosition;			//位置
	DXVector3 mRotationCenter;	//回転(移動行列適用前)
	DXVector3 mRotationTransed;	//回転移動行列適用後)
	DXVector3 mScale;				//拡大率

	D3DXMATRIX mMatrix;			//計算用


};
