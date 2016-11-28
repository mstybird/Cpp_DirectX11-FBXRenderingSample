#pragma once

//最大値と最小値、現在値を保持するクラス
/*
現在値の設定に失敗した場合、設定済みの最小値がセットされる
*/

enum class MSPRogressFlag {
	LESSMIN,
	GREATEMAX,
	NOTOVERFLOW
};

class MSProgress
{
public:
	MSProgress();

	float GetMax();
	float GetNow();
	float GetMin();
	//現在の数値が最大値に対して何％分かどうか(100%を1.0fとする)
	float GetNowPer();
	//あと何％で最大値に達するか(100%を1.0fとする)
	float GetMaxRemainPer();
	//あといくらで最大値に達するか
	float GetMaxRemainFix();


	///最小値が最大値より大きい場合、
	///想定してはいけない事例なので強制終了する
	//それぞれの値をセットする
	void Set(float aMax, float aMin, float aNow);
	//現在地となる値を直接セットする
	void SetNowFix(float aNowValue);
	//現在地となる値を割合(1を100%とする)分セットする
	void SetNowPer(float aNowValue);

	//指定した値をそのまま加算する
	//範囲から溢れたかの情報を返す
	MSPRogressFlag AddFix(float aValue);
	//指定した値をそのまま減算する
	//範囲から溢れたかの情報を返す
	MSPRogressFlag SubFix(float aValue);
	//指定した値を割合(1を100%とする)分加算する
	//範囲から溢れたかの情報を返す
	MSPRogressFlag AddPer(float aValue);
	//指定した値を割合(1を100%とする)分減算する
	//範囲から溢れたかの情報を返す
	MSPRogressFlag SubPer(float aValue);

	MSPRogressFlag IsBetweenValue(float aValue);
protected:
	void FitValue();
protected:
	float mMax;
	float mNow;
	float mMin;
};