#pragma once
//#include"DXVector3.h"
//ワールド空間座標系クラス
#include<d3dx9.h>
class DXVector3;
class DXMatrix;
class DXWorld {
public:
	//座標移動パターン
	enum TYPEMOVE {
		TYPE_PARALLEL=0,	//平行移動
		TYPE_ROTATE=1	//回転移動
	};

	DXWorld();
	~DXWorld();
	///移動行列操作
	void SetT(float pX, float pY, float pZ);
	void SetT(DXVector3 pPosition);
	void AddT(float pX, float pY, float pZ);
	void AddT(DXVector3 pPosition);
	void AddT(TYPEMOVE pType, float pSpeed, DXVector3 pDirection);
	void AddTRotated(float pSpeed, DXVector3 pDirection);


	void SubT(float pX, float pY, float pZ);
	void SubT(DXVector3 pPosition);
	void MulT(float pX, float pY, float pZ);
	void MulT(DXVector3 pPosition);
	void DivT(float pX, float pY, float pZ);
	void DivT(DXVector3 pPosition);

	///移動前の回転行列
	void SetRC(float pX, float pY, float pZ);
	void SetRC(DXVector3 pPosition);
	void AddRC(float pX, float pY, float pZ);
	void AddRC(DXVector3 pPosition);
	void SubRC(float pX, float pY, float pZ);
	void SubRC(DXVector3 pPosition);
	void MulRC(float pX, float pY, float pZ);
	void MulRC(DXVector3 pPosition);
	void DivRC(float pX, float pY, float pZ);
	void DivRC(DXVector3 pPosition);

	//移動後の回転行列
	void SetRT(float pX, float pY, float pZ);
	void SetRT(DXVector3 pPosition);
	void AddRT(float pX, float pY, float pZ);
	void AddRT(DXVector3 pPosition);
	void SubRT(float pX, float pY, float pZ);
	void SubRT(DXVector3 pPosition);
	void MulRT(float pX, float pY, float pZ);
	void MulRT(DXVector3 pPosition);
	void DivRT(float pX, float pY, float pZ);
	void DivRT(DXVector3 pPosition);

	//拡大行列
	void SetS(float pX, float pY, float pZ);
	void SetS(DXVector3 pScaling);
	void AddS(float pX, float pY, float pZ);
	void AddS(DXVector3 pScaling);
	void SubS(float pX, float pY, float pZ);
	void SubS(DXVector3 pScaling);
	void MulS(float pX, float pY, float pZ);
	void MulS(DXVector3 pScaling);
	void DivS(float pX, float pY, float pZ);
	void DivS(DXVector3 pScaling);

	DXMatrix* GetMatrix();
	DXVector3 *mPosition;			//位置
	DXVector3 *mRotationCenter;	//回転(移動行列適用前)
	DXVector3 *mRotationTransed;	//回転移動行列適用後)
	DXVector3 *mScale;				//拡大率

	DXMatrix *mMatrix;			//計算用


};
