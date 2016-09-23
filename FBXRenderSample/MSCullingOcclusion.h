#pragma once
#include<d3dx11.h>
#include<functional>
#include"MS3DRender.h"
#include"MSFbxManager.h"
#include"DX11RenderResource.h"
#include"DXProjection.h"
#include<memory>
class MSCullingOcculusion abstract{
public:
	//���_���\�[�X���猩���^�[�Q�b�g�̃I�N���[�W��������
	/*
		pRender			:�����_�����O�Ɏg���������ς݂̃����_���[
		pEyeResource	:�J�����̈ʒu
		pEyeProjection	:�J�����̎���
		pTargetResource	:�^�[�Q�b�g�̈ʒu
		pTargetMesh		:�^�[�Q�b�g�̃��b�V��
		pPixelper		:���������Ɣ��肷��s�N�Z���̊���0~1.0f
		pRenderFunc		:��Q�ƂȂ�I�u�W�F�N�g�̕`�揈��
	*/
	static bool IsCullingWorld(
		MS3DRender&pRender,
		DX11RenderResource&pEyeResource,
		DXProjection&pEyeProjection,
		const std::weak_ptr<DX11RenderResource>&pTargetResource,
		const std::weak_ptr<MSFbxManager>&pTargetMesh,
		float pPixelper,
		std::function<void(void)>pRenderFunc
		);
	static void Initialize(ID3D11Device*pDevice, ID3D11DeviceContext*pDeviceContext,
		IDXGISwapChain*pSwapChain);

	static void Release();
private:

	static void CreateOcclusionQuery();
	static void BeginOcclusionQuery();
	static void EndOcclusionQuery();

	//�J�����O�Ŏg�p����
	static ID3D11RenderTargetView*sRTV;
	static ID3D11DepthStencilView*sDSV;
	static ID3D11Texture2D*sDS2D;

	static ID3D11Query* sOcculusionQuery;
	static ID3D11Device*sDevice;
	static ID3D11DeviceContext*sDeviceContext;
	static IDXGISwapChain*sSwapChain;
};