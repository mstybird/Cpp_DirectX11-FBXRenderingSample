#pragma once
#include"MSBase3DShader.h"
#include<d3dx9.h>
class Collision3DShader :public MSBase3DShader {
public:
	Collision3DShader();

	struct MyFBXCONSTANTBUFFER0//�t���[���P�ʂœn����� ���C�g���͂����ɓ����
	{

	};

	struct MyFBXCONSTANTBUFFER1
	{
		D3DXMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
	};

	//���b�V������擾����
	struct MyFBXCONSTANTBUFFER2
	{

	};

	virtual void SetConstantBuffer0(CBResource0&aResource)override;

	//���b�V���P�ʂɐݒ�p
	virtual void SetConstantBuffer1(
		FBXMesh& fbxMesh,
		DX11RenderResource* resource,
		DXDisplay* pDisplay)override;
	virtual void SetConstantBuffer2(std::weak_ptr<FBXModelData>modelData)override;
	void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout)override;

};