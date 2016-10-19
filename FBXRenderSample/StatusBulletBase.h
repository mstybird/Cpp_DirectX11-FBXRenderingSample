#pragma once
/*
	弾の情報
*/
#include"MSProgress\MSProgressData.hxx"

//ダメージタイプ
enum class BulletDamageType {
	NORMAL,	//通常(防御が実装されれば防御に関係する
	PERCENT,//割合ダメージ
	FIXED	//固定ダメージ
};

struct StatusBulletBase {
	int mCost;
	float mAtk;
	MSProgress mInterval;
	BulletDamageType mType;
};