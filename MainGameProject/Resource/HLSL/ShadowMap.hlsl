//�O���[�o��
/*
	FBX�����_�����O�p
*/
//Texture2D gTexture:register(t0);
//SamplerState gSampler:register(s0);

Texture2D g_texDecal:register(t0);
SamplerState g_samLinear:register(s0);

//���ꂩ��`�悷��I�u�W�F�N�g���ׂĂɓK�p
cbuffer per_frame:register(b0) {
	float4 g_vLightDir;	//�f�B���N�V���i�����C�g
	float4 g_vEye;		//�J�������W
};

cbuffer per_mesh:register(b1) {
	matrix g_mW;	//���[���h�s��
	matrix g_mWVP;	//�ϊ��s��
};

cbuffer per_material:register(b2) {
	float4 g_Ambient;	//����
	float4 g_Diffuse;	//�g�U���ˌ�
	float4 g_Specular;	//���ʔ��ˌ�
};

//���_�V�F�[�_�o�͍\����
struct VS_OUTPUT {
	float4 Pos		:	SV_POSITION;
	float3 Light	:	TEXCOORD0;
	float3 Normal	:	TEXCOORD1;
	float3 EyeVector:	TEXCOORD2;
	float4 PosWorld	:	TEXCOORD3;
	float2 Tex		:	TEXCOORD4;
};

//�����̃t�H���V�F�[�f�B���O
VS_OUTPUT VS(float4 Pos:POSITION, float4 Norm :NORMAL, float2 Tex : TEXCOORD) {
	VS_OUTPUT output;
	//�ˉe�ϊ�
	output.Pos = mul(Pos, g_mWVP);
	//�@���̉�]
	output.Normal = mul(Norm, (float3x3)g_mW);
	//���C�g�����̐��K��
	output.Light = normalize(g_vLightDir);
	//�����x�N�g�� ���[���h��ԏ�ł̒��_���王�_�֌������x�N�g��
	output.PosWorld = mul(Pos, g_mW);
	output.EyeVector = normalize(g_vEye - output.PosWorld);
	//�e�N�X�`���[���W
	output.Tex = Tex;
	return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	//�����@��
	float4 ambient = g_Ambient;
	//�g�U���ˌ��@��
	float NL = saturate(dot(input.Normal, input.Light));

	float4 diffuse = 
		g_Diffuse * g_Diffuse.a + 
		g_texDecal.Sample(g_samLinear, input.Tex) * (1.0- g_Diffuse.a);

	//���ʔ��ˌ��@��
	float3 reflect = normalize(2 * NL*input.Normal - input.Light);
	float4 specular = 2*pow(saturate(dot(reflect,input.EyeVector)), 20);
	//�t�H�����f���ŏI�F�@�R�̍��̍��v
	float4 color = diffuse + specular;
	//�A���t�@�u�����h
	color.a = (1.0-g_Diffuse.a);

	return color;
}
