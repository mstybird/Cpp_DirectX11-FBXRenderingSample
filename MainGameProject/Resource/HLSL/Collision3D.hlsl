//�O���[�o��
/*
	FBX�����_�����O�p
*/
//���b�V�����ƂɓK�p
cbuffer global:register(b1)
{
	matrix g_mWVP;//���[���h�s��
};

//
//�o�[�e�b�N�X�V�F�[�_�[
//
float4 VS(float4 Pos : POSITION):SV_POSITION
{
	return mul(Pos, g_mWVP);
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(float4 Pos :SV_POSITION) : SV_Target
{
	return float4(1,1,1,1);
}