#pragma once
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

	struct MyFBXCONSTANTBUFFER1
	{
		D3DXMATRIX mW;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
		D3DXMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
		D3DXVECTOR4 LightDir{ 1,0,-1,0 };
	};

	//���b�V������擾����
	struct MyFBXCONSTANTBUFFER2
	{

		D3DXVECTOR4 Diffuse;
		ALIGN16 float ColorPer;
	};



	//���b�V���P�ʂɐݒ�p
	void SetConstantBuffer1(
		FBXMesh& fbxMesh,
		DX11RenderResource* resource, 
		DXDisplay* pDisplay)override;
	void SetConstantBuffer2(std::weak_ptr<FBXModelData>modelData)override;

private:
	void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout)override;
};
