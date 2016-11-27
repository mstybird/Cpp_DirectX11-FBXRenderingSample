#pragma once
/*
	弾の情報
*/
#include"MSProgress\MSProgressData.hxx"
#include<DXMath.hpp>
//ダメージタイプ
enum class BulletDamageType {
	NORMAL,	//通常(防御が実装されれば防御に関係する
	PERCENT,//割合ダメージ
	FIXED	//固定ダメージ
};

//バレット固有の能力の基底クラス
struct StatusBulletBase {
	int mCost;				//使用コスト
	float mAtk;				//攻撃力
	MSProgress mInterval;	//発射間隔
	float mIntervalRecovery;//発射間隔回復量
	BulletDamageType mType;	//ダメージタイプ
	float mFiringRange;		//射程
	float mVelocity;		//速度
	DXVector3 mDirection;	//方向ベクトル


};