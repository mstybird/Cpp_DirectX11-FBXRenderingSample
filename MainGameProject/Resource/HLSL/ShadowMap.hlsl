//グローバル
/*
	FBXレンダリング用
*/
//Texture2D gTexture:register(t0);
//SamplerState gSampler:register(s0);

Texture2D g_texDecal:register(t0);
SamplerState g_samLinear:register(s0);

//これから描画するオブジェクトすべてに適用
cbuffer per_frame:register(b0) {
	float4 g_vLightDir;	//ディレクショナルライト
	float4 g_vEye;		//カメラ座標
};

cbuffer per_mesh:register(b1) {
	matrix g_mW;	//ワールド行列
	matrix g_mWVP;	//変換行列
};

cbuffer per_material:register(b2) {
	float4 g_Ambient;	//環境光
	float4 g_Diffuse;	//拡散反射光
	float4 g_Specular;	//鏡面反射光
};

//頂点シェーダ出力構造体
struct VS_OUTPUT {
	float4 Pos		:	SV_POSITION;
	float3 Light	:	TEXCOORD0;
	float3 Normal	:	TEXCOORD1;
	float3 EyeVector:	TEXCOORD2;
	float4 PosWorld	:	TEXCOORD3;
	float2 Tex		:	TEXCOORD4;
};

//ただのフォンシェーディング
VS_OUTPUT VS(float4 Pos:POSITION, float4 Norm :NORMAL, float2 Tex : TEXCOORD) {
	VS_OUTPUT output;
	//射影変換
	output.Pos = mul(Pos, g_mWVP);
	//法線の回転
	output.Normal = mul(Norm, (float3x3)g_mW);
	//ライト方向の正規化
	output.Light = normalize(float4(1,1,0,0));
	//視線ベクトル ワールド空間上での頂点から視点へ向かうベクトル
	output.PosWorld = mul(Pos, g_mW);
	output.EyeVector = normalize(g_vEye - output.PosWorld);
	//テクスチャー座標
	output.Tex = Tex;
	return output;
}

//
//ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	//環境光　項
	float4 ambient = g_Ambient;
	//拡散反射光　項
	float NL = saturate(dot(input.Normal, input.Light));

	float4 diffuse = 
		g_Diffuse * g_Diffuse.a + 
		g_texDecal.Sample(g_samLinear, input.Tex) * (1.0- g_Diffuse.a);

	//鏡面反射光　項
	float3 reflect = normalize(2 * NL*input.Normal - input.Light);
	float4 specular = 2*pow(saturate(dot(reflect,input.EyeVector)), 20);
	//フォンモデル最終色　３つの項の合計
	float4 color = diffuse + specular;
	//アルファブレンド
	color.a = (1.0-g_Diffuse.a);

	return color;
}
