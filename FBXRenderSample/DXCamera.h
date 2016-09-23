#pragma once
#include<memory>
//#include<d3dx9.h>
class DXVector3;
class DXMatrix;
class DXWorld;
class DXCamera {
public:
	static const DXVector3 sUpVector;
	//カメラの移動タイプ
	enum TYPEMOVE {
		TYPE_NORMAL = 0,	//向きに関係なくXYZ値で直接移動
		TYPE_PARALLEL,		//現在向いている向きを基準に移動
	};
	//移動方向
	class DIRECTION_TYPE {
	public:
		static const DXVector3 LEFTRIGHT;	//左右
		static const DXVector3 UPDOWN;		//上下
		static const DXVector3 FRONTBACK;	//手前奥
	};

	//コンストラクタ
	DXCamera();
	~DXCamera();

	//視点とターゲット(注視点)の設定
	void SetCamera(const std::weak_ptr<DXWorld>&pEyePosition, const std::weak_ptr<DXWorld>&pLookAtPosition);
	//ワールド座標から指定数ずらす場合
	void SetCamera(const std::weak_ptr<DXWorld>&pEyePosition, const DXVector3&pDistance);
	

	void CreateRay(DXVector3&pOutRay, const DXVector3&pRayDirection)const;

	//指定した分移動する
	void Translation(TYPEMOVE pType, float pSpeed, const DXVector3&pDirection, bool pLockoned = false);

	void Clone(DXCamera&pOutClone);

	//カメラの回転
	void Rotate(float pX, float pY, float pZ);

	std::weak_ptr<DXMatrix>GetMatrix();
	void GetEyeT(DXVector3 &pOutTranslate);
	void GetLookT(DXVector3 &pOutTranslate);

	std::shared_ptr<DXVector3>mEyePosition;	//カメラの位置
	std::shared_ptr<DXVector3>mLookPosition;//注視点
	std::shared_ptr<DXVector3>mUpVector;	//頭上方向
	std::shared_ptr<DXVector3>mRotate;		//カメラの角度
	std::shared_ptr<DXMatrix>mMatrix;
	

	std::weak_ptr<DXMatrix> operator*() {
		return GetMatrix();
	}
private:
	
};