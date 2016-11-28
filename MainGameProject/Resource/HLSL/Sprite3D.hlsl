//�O���[�o��

Texture2D g_Texture: register(t0);
SamplerState g_Sampler : register(s0);

cbuffer global
{
	matrix g_WVP; //���[���h����ˉe�܂ł̕ϊ��s��
};
//�\����
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
};

//
//
//�o�[�e�b�N�X�V�F�[�_�[
PS_INPUT VS( float4 Pos : POSITION ,float2 UV : TEXCOORD) 
{
	PS_INPUT Out;
	Out.Pos=mul(Pos,g_WVP);
	Out.UV=UV;

	return Out;
}
//
//
//�s�N�Z���V�F�[�_�[
float4 PS( PS_INPUT Input ) : SV_Target
{
	return g_Texture.Sample( g_Sampler, Input.UV );
}