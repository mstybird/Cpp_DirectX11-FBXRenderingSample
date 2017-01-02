#pragma once

/*
	���C�������������b�V���̂ݓ��߂�����悤�ɂ���

*/


#include<D3DX9.h>

#include"MSBase3DShader.h"
#include"DX11Resrouce.h"
#include<memory>



struct FBXMesh;
struct FBXModelData;
class DX11RenderResource;
class DXDisplay;
class My3DShader :public MSBase3DShader {
public:
	//�o�b�t�@�T�C�Y�A���_�v�f�T�C�Y��n��
	My3DShader();

	struct MyFBXCONSTANTBUFFER0//�t���[���P�ʂœn����� ���C�g���͂����ɓ����
	{
		D3DXVECTOR4 vDirectLightDir;//�f�B���N�V���i�����C�g
		D3DXVECTOR4 vEye;//�J�����ʒu
	};

	struct MyFBXCONSTANTBUFFER1
	{
		D3DXMATRIX mW;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
		D3DXMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
		//D3DXVECTOR4 LightDir{ 1,0,-1,0 };
		//ALIGN16 float mAlpha{ 1.0f };
	};

	//���b�V������擾����
	struct MyFBXCONSTANTBUFFER2
	{

		D3DXVECTOR4 Ambient;//�A���r�G���g��
		D3DXVECTOR4 Diffuse;//�f�B�t���[�Y�F
		D3DXVECTOR4 Specular;//���ʔ���

		//ALIGN16 float ColorPer;
	};

	virtual void SetConstantBuffer0(CBResource0&aResource)override;

	//���b�V���P�ʂɐݒ�p
	virtual void SetConstantBuffer1(
		FBXMesh& fbxMesh,
		DX11RenderResource* resource, 
		DXDisplay* pDisplay)override;
	virtual void SetConstantBuffer2(std::weak_ptr<FBXModelData>modelData)override;

private:
	void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout)override;
};
