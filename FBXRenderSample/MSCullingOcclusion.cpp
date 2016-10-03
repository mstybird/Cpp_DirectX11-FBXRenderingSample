#include "MSCullingOcclusion.h"
#include"DX11Resrouce.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"DXCamera.h"
#include"DXVector3.h"
#include"MSDirect.h"
#include"DXDisplay.h"
ID3D11RenderTargetView* MSCullingOcculusion::sRTV;
ID3D11DepthStencilView* MSCullingOcculusion::sDSV;
ID3D11Texture2D* MSCullingOcculusion::sDS2D;

ID3D11Query* MSCullingOcculusion::sOcculusionQuery;
ID3D11Device*MSCullingOcculusion::sDevice;
ID3D11DeviceContext*MSCullingOcculusion::sDeviceContext;
IDXGISwapChain*MSCullingOcculusion::sSwapChain;

bool MSCullingOcculusion::IsCullingWorld(
	MS3DRender&pRender,
	DX11RenderResource&pEyeResource,
	const std::weak_ptr<DX11RenderResource>&pTargetResource,
	const std::weak_ptr<MSFbxManager>&pTargetMesh,
	float pPixelper,
	std::function<void(void)>pRenderFunc
	)
{
	//���[���h�s�񂩂王�_�𐶐�

	//���ɖ߂����߂̃J�����̃R�s�[�𐶐�
	DXCamera lEyeCameraCopy;
	pEyeResource.GetCamera().lock()->Clone(lEyeCameraCopy);
	//���ɖ߂����߂̎���̃R�s�[���쐬
	auto lEyeProjCopy = *pEyeResource.GetProjection().lock();

	//�J���������[���h�s����g���Đ��ʕ����ɍ쐬����
	pEyeResource.GetCamera().lock()->SetCamera(pEyeResource.GetWorld(), { 0,0,-1 });
	//�����ݒ�
	//pEyeResource.SetProjection(pEyeProjection);

	//�N�G���̊J�n
	{
		//�`�����N�G�������p�ɕύX

		ID3D11RenderTargetView* lRTVCopy;
		ID3D11DepthStencilView* lDSVCopy;
		D3D11_VIEWPORT* lMainViewPort = MSDirect::GetViewPort();
		sDeviceContext->OMGetRenderTargets(1, &lRTVCopy, &lDSVCopy);


		D3D11_VIEWPORT vp;
		vp.Width = 640;
		vp.Height = 480;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		sDeviceContext->RSSetViewports(1, &vp);
		sDeviceContext->OMSetRenderTargets(1, &sRTV, sDSV);

		float ClearColor[4] = { 0,0,0,1 };
		sDeviceContext->ClearRenderTargetView(sRTV, ClearColor);//�����_�[�^�[�Q�b�g�N���A
		sDeviceContext->ClearDepthStencilView(sDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�X�e���V���o�b�t�@�N���A

		//��Q����`�悷�鏈��
		DXDisplay lTmpDisplay;
		pRender.GetDisplay(lTmpDisplay);
		pRender.SetRenderTarget(pEyeResource);
		pRenderFunc();


		//�����_�����O���������`��
		BeginOcclusionQuery();
		pRender.Render(pTargetMesh, pTargetResource);
		EndOcclusionQuery();
		pRender.SetRenderTarget(lTmpDisplay);
		//�J�����Ǝ�������Ƃɖ߂�
		pEyeResource.SetCamera(lEyeCameraCopy);
		//pEyeResource.SetProjection(lEyeProjCopy);
		//�`�������ɖ߂�
		sDeviceContext->OMSetRenderTargets(1, &lRTVCopy, lDSVCopy);
		sDeviceContext->RSSetViewports(1, lMainViewPort);


	}
	UINT64 lDrawPixels{};
	//�s�N�Z�������擾
	while (S_OK != sDeviceContext->GetData(sOcculusionQuery, (void*)&lDrawPixels, sizeof(UINT64), 0));

	//�������Ɣ��肷��K�v�ȃs�N�Z�����̌v�Z
	UINT64 lCheckPixels = (float)(640 * 480)*pPixelper;

	return lDrawPixels > lCheckPixels;
}

void MSCullingOcculusion::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, IDXGISwapChain*pSwapChain)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
	sSwapChain = pSwapChain;

	//�J�����O�Ŏg�������_�[�^�[�Q�b�g�r���[�̍쐬
	{
		ID3D11Texture2D*lBackBuffer;
		sSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&lBackBuffer);
		sDevice->CreateRenderTargetView(lBackBuffer, nullptr, &sRTV);
		SAFE_RELEASE(lBackBuffer);
	}

	//�J�����O�Ŏg���X�e���V���r���[�̍쐬
	{
		D3D11_TEXTURE2D_DESC lDescDepth;
		lDescDepth.Width = 640;
		lDescDepth.Height = 480;
		lDescDepth.MipLevels = 1;
		lDescDepth.ArraySize = 1;
		lDescDepth.Format = DXGI_FORMAT_D32_FLOAT;
		lDescDepth.SampleDesc.Count = 1;
		lDescDepth.SampleDesc.Quality = 0;
		lDescDepth.Usage = D3D11_USAGE_DEFAULT;
		lDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		lDescDepth.CPUAccessFlags = 0;
		lDescDepth.MiscFlags = 0;

		sDevice->CreateTexture2D(&lDescDepth, nullptr, &sDS2D);
		sDevice->CreateDepthStencilView(sDS2D, nullptr, &sDSV);

	}

	//���ۂɃJ�����O���鏈�����瑱��
	CreateOcclusionQuery();


}

void MSCullingOcculusion::CreateOcclusionQuery()
{
	D3D11_QUERY_DESC lQueryDesc{};
	lQueryDesc.Query = D3D11_QUERY_OCCLUSION;
	sDevice->CreateQuery(&lQueryDesc, &sOcculusionQuery);
}

void MSCullingOcculusion::BeginOcclusionQuery()
{
	sDeviceContext->Begin(sOcculusionQuery);
}

void MSCullingOcculusion::EndOcclusionQuery()
{
	sDeviceContext->End(sOcculusionQuery);
}
