//グローバル
/*
	FBXレンダリング用
*/
Texture2D gTexture:register(t0);
SamplerState gSampler:register(s0);

//アプリケーションごとに適用
cbuffer global:register(b0)
{
	matrix g_mW;//ワールド行列
	matrix g_mWVP; //ワールドから射影までの変換行列
	float4 g_vLightDir;  //ライトの方向ベクトル
	float g_Alpha;	//透明度
};

//サブメッシュごとに適用
cbuffer global:register(b1) {
	float4 g_Diffuse = float4(1, 0, 0, 0); //拡散反射(色）
	float g_ColorPer = 1.0f;
}

//構造体
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 UV:TEXCOORD;
};

//
//バーテックスシェーダー
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Normal : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	output.UV = Tex;
	Normal.w = 0;
	Normal = mul(Normal, g_mW);
	Normal = normalize(Normal);
	output.Color = 1.0 * g_Diffuse * dot(Normal, g_vLightDir);//この式はランバートの余弦則

	return output;
}

//
//ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 Color = gTexture.Sample(gSampler,input.UV)*(1.0 - g_ColorPer) + input.Color*(g_ColorPer);
	Color.w = g_Alpha;
	return Color;
}