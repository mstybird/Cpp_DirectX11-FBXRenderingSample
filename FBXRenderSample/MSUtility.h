#pragma once
class DX11RenderResource;
class DXVector3;
//�Ǐ]�����B�Ǐ]�p�x���w�肵�A�w��p�x�O�������ꍇ�A�Ǐ]�p�x�����̂܂ܖ߂�l�ƂȂ�
//�߂�l�̓f�B�O���[
float MSHormingY(DX11RenderResource&pEyeResource, DX11RenderResource&pTargetResource, const float pDegree = 180);

//float�̃[��������s��
bool IsZero(float pValue, float Threshold=0.0000001);


//���_�ƃ^�[�Q�b�g�Ƃ̑��Ίp�x���擾����
float MSGetToRotateY(DX11RenderResource&pEyeResource, DX11RenderResource&pTargetResource);

/*
	��邱�ƁB
	�`�F�b�N�|�C���g�𗘗p�����U��(���Ԃɂ��ǂ�)
*/