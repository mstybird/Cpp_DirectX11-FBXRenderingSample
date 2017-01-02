//グローバル
/*
	FBXレンダリング用
*/
//メッシュごとに適用
cbuffer global:register(b1)
{
	matrix g_mWVP;//ワールド行列
};

//
//バーテックスシェーダー
//
float4 VS(float4 Pos : POSITION):SV_POSITION
{
	return mul(Pos, g_mWVP);
}

//
//ピクセルシェーダー
//
float4 PS(float4 Pos :SV_POSITION) : SV_Target
{
	return float4(1,1,1,1);
}