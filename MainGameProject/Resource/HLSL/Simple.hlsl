//�O���[�o��
/*
	FBX�����_�����O�p
*/
Texture2D gTexture:register(t0);
SamplerState gSampler:register(s0);

//�A�v���P�[�V�������ƂɓK�p
cbuffer global:register(b0)
{
	matrix g_mW;//���[���h�s��
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_vLightDir;  //���C�g�̕����x�N�g��
	float g_Alpha;	//�����x
};

//�T�u���b�V�����ƂɓK�p
cbuffer global:register(b1) {
	float4 g_Diffuse = float4(1, 0, 0, 0); //�g�U����(�F�j
	float g_ColorPer = 1.0f;
}

//�\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 UV:TEXCOORD;
};

//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Normal : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	output.UV = Tex;
	output.Color = 1.0 * g_Diffuse * dot(Normal, g_vLightDir);//���̎��̓����o�[�g�̗]����

	return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 Color = gTexture.Sample(gSampler,input.UV)*(1.0 - g_ColorPer) + input.Color*(g_ColorPer);
	Color.w = g_Alpha;
	return Color;
}