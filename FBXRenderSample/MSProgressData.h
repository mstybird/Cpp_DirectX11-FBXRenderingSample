#pragma once

//最大値と最小値、現在値を保持するクラス
/*
	現在値の設定に失敗した場合、設定済みの最小値がセットされる
*/

enum class MSPRogressFlag{
	LESSMIN,
	GREATEMAX,
	NOTOVERFLOW
};

class MSProgress
{

	MSProgress();

	float GetMax();
	float GetNow();
	float GetMin();

	///最小値が最大値より大きい場合、
	///想定してはいけない事例なので強制終了する
	//それぞれの値をセットする
	void Set(float aMax, float aMin, float aNow);
	//現在地となる値を直接セットする
	bool SetNowFix(float aNowValue);
	//現在地となる値を割合(1を100%とする)分セットする
	bool SetNowPer(float aNowValue);
	
	//指定した値をそのまま加算する
	//最大値を超えたらtrueを返し、
	//最大値に満たない場合はfalseを返す
	bool AddFix(float aValue);
	//指定した値をそのまま減算する
	//最小値を超えたらtrueを返し、
	//最小値に満たない場合はfalseを返す
	bool SubFix(float aValue);
	//指定した値を割合(1を100%とする)分加算する
	//最大値を超えたらtrueを返し、
	//最大値に満たない場合はfalseを返す
	bool AddPer(float aValue);
	//指定した値を割合(1を100%とする)分減算する
	//最小値を超えたらtrueを返し、
	//最小値に満たない場合はfalseを返す
	bool SubPer(float aValue);

	bool IsBetweenValue(float aValue);
protected:
	float mMax;
	float mNow;
	float mMin;
};