#include"MSDirect.h"
#include"DX11Resrouce.h"
#include"MSSceneBase.h"
//�V�F�[�_�֌W
#include"MSBaseShaderResource.h"
#include"MSBaseShader.h"
//�X�v���C�g�����_���[
#include"MSSprite2DResource.h"
#include"MSSprite2DRender.h"
//3D�����_���[
#include"MS3DRender.h"
//FBX
#include"MSFbxManager.h"
//Other
#include"DXMath.hpp"
#include"DX11TextureManager.hpp"
#include"MSKeyCodeList.h"

//�J�����O
#include"MSCullingOcclusion.h"

//�e�L�X�g
#include"Graphic_Text.h"

#include<D3DX11.h>

std::shared_ptr<MSDirect> MSDirect::sMSDirect{ std::make_shared<MSDirect>() };

void MSDirect::Destroy()
{
	sMSDirect.reset();
}

MSDirect::MSDirect() :
	mHwnd{ nullptr },
	m_pDevice{ nullptr },
	m_pDeviceContext{ nullptr },
	m_pSwapChain{ nullptr },
	m_pBackBuffer_TexRTV{ nullptr },
	m_pBackBuffer_DSTexDSV{ nullptr },
	m_pBackBuffer_DSTex{ nullptr },
	m_pRasterizerState{ nullptr }
{

}

MSDirect::~MSDirect() {
	scene.reset();
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pRasterizerState);

}

LRESULT MSDirect::MessageProcedule(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	


	//1�ł��L�[�����͂���Ă�����true��Ԃ�
	if (scene.get() != nullptr) {

		switch (iMsg)
		{
		case WM_MOVE:
			//�E�B���h�E�ړ���
			//X���W,Y���W
			break;
		case WM_SIZE:
			//�E�B���h�E�T�C�Y�ύX��
			/*
				w:
				SIZE_MAXMIZED
				SIZE_MINIMIZED
				SIZE_MAXHIDE
				SIZE_MAXSHOW
				l:
				LOWORD(lp)��
				HIWORD(lp)����
			*/
			break;
		case WM_ACTIVATE:
			//�E�B���h�E�̃A�N�e�B�u��Ԃ̐؂�ւ���
			break;
		case WM_ENABLE:
			//�E�B���h�E�̗L�������؂�ւ���
			break;
		case WM_SHOWWINDOW:
			//�E�B���h�E�\��/��\���؂�ւ���
			break;
		case WM_SETFOCUS:
			//�E�B���h�E���t�H�[�J�X���擾�����Ƃ�
			break;
		case WM_KILLFOCUS:
			//�E�B���h�E���t�H�[�J�X���������Ƃ�
			break;
		case WM_GETMINMAXINFO:
			//�E�B���h�E�̍ő�ŏ��T�C�Y�A�ő厞�̈ʒu�A�ŏ��g���b�L���O�T�C�Y�Ȃǂ��擾
			break;
		case WM_SETTEXT:
			//�E�B���h�E�̕�����̐ݒ莞
			break;
		case WM_GETTEXT:
			//�E�B���h�E�̕�����̎擾��
			break;
		case WM_KEYDOWN:
			//��V�X�e���L�[���͎�
			/*
				wParam:�L�[�R�[�h
				LOWORD(lp)�������ςȂ����ǂ����B1��true
			*/
			if (KeyList[static_cast<MSKEY>(wParam)] == false) {
				scene->KeyDown(static_cast<MSKEY>(wParam));
				KeyList[static_cast<MSKEY>(wParam)] = true;
			}



			break;
		case WM_KEYUP:
			//�L�[�������ꂽ�Ƃ�
			KeyList[static_cast<MSKEY>(wParam)] = false;
			scene->KeyUp(static_cast<MSKEY>(wParam));
			break;


		case WM_DESTROY:
			//�E�B���h�E�j����
			PostQuitMessage(0);
			break;
		case WM_TIMER:
			//��莞�Ԃ��Ƃɏ���
			//���z�L�[�R�[�h���X�g���牟����Ă���L�[�̂ݑI��ŏ���
			for (auto& x : KeyList) {
				if (x.second)scene->KeyHold(x.first);
			}
	
			break;
		}
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

const std::weak_ptr<MSDirect> MSDirect::GetInstance()
{
	return sMSDirect;
}

HRESULT MSDirect::InitD3D(HWND pHwnd)
{
	mHwnd = pHwnd;
	// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = mHwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	//�^�C�}�[�̃Z�b�g
	SetTimer(mHwnd, 1, 1000 / 60, nullptr);

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice,
		pFeatureLevel, &m_pDeviceContext)))
	{
		return FALSE;
	}


	//�e��e�N�X�`���[�ƁA����ɕt�т���e��r���[���쐬
	//�o�b�N�o�b�t�@�[�e�N�X�`���[���擾�i���ɂ���̂ō쐬�ł͂Ȃ��j
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
	//���̃e�N�X�`���[�ɑ΂������_�[�^�[�Q�b�g�r���[(RTV)���쐬
	m_pDevice->CreateRenderTargetView(pBackBuffer_Tex, NULL, &m_pBackBuffer_TexRTV);
	SAFE_RELEASE(pBackBuffer_Tex);

	//�f�v�X�X�e���V���r���[�p�̃e�N�X�`���[���쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pBackBuffer_DSTex);
	//���̃e�N�X�`���[�ɑ΂��f�v�X�X�e���V���r���[(DSV)���쐬
	m_pDevice->CreateDepthStencilView(m_pBackBuffer_DSTex, NULL, &m_pBackBuffer_DSTexDSV);

	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV);
	//�r���[�|�[�g�̐ݒ�
	//D3D11_VIEWPORT vp;

	mViewPort.Width = WINDOW_WIDTH;
	mViewPort.Height = WINDOW_HEIGHT;
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;
	mViewPort.TopLeftX = 0;
	mViewPort.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &mViewPort);
	//���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = TRUE;

	m_pDevice->CreateRasterizerState(&rdc, &m_pRasterizerState);
	m_pDeviceContext->RSSetState(m_pRasterizerState);

	//�e���b�p�[��DirectX11�f�o�C�X����o�^
	MSBaseShaderResource::sInitialize(m_pDevice, m_pDeviceContext);
	//MSPixelShader::sInitialize(m_pDevice, m_pDeviceContext);
	MSBaseShader::Init(m_pDevice, m_pDeviceContext);
	MSSpriteBaseResource::sInitialize(m_pDevice);
	MSSpriteBaseRender::Initialize(m_pDevice, m_pDeviceContext, m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV);
	MS3DRender::Initialize(m_pDevice, m_pDeviceContext, m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV);

	MSFbxManager::InitDevice(m_pDevice, m_pDeviceContext);
	DXProjection::SetAspect((FLOAT)WINDOW_WIDTH, (FLOAT)WINDOW_HEIGHT);
	DXTexture::Initialize(m_pDevice);

	MSCullingOcculusion::Initialize(m_pDevice, m_pDeviceContext,m_pSwapChain);

	TextGraphic::SetDevice(m_pDevice);
	TextGraphic::SetDeviceContext(m_pDeviceContext);

	TextManager::SetDevice(m_pDevice);
	TextManager::SetDeviceContext(m_pDeviceContext);

	CharDatabase::SetDevice(m_pDevice);
	CharDatabase::SetDeviceContext(m_pDeviceContext);

	this->KeyList = MSKeyList;

	

	return S_OK;
}


void MSDirect::Loop()
{

	scene->Update();
	scene->Render();
	m_pSwapChain->Present(1, 0);
}

void MSDirect::SetScene(std::unique_ptr<MSSceneBase>&& pScene)
{
	pScene->Initialize();
	std::unique_ptr<MSSceneBase> lBufferPtr = std::move(sMSDirect->scene);
	sMSDirect->scene = std::move(pScene);
	lBufferPtr.reset();
}

D3D11_VIEWPORT * MSDirect::GetViewPort()
{
	return &sMSDirect->mViewPort;
}

ID3D11RenderTargetView * MSDirect::GetRTV()
{
	return sMSDirect->m_pBackBuffer_TexRTV;
}

ID3D11DepthStencilView * MSDirect::GetDSV()
{
	return sMSDirect->m_pBackBuffer_DSTexDSV;
}

ID3D11Device * MSDirect::GetDevice()
{
	return sMSDirect->m_pDevice;
}

ID3D11DeviceContext * MSDirect::GetDeviceContext()
{
	return sMSDirect->m_pDeviceContext;
}

IDXGISwapChain * MSDirect::GetSwapChain()
{
	return sMSDirect->m_pSwapChain;
}

void MSDirect::ResetKeyStateAll()
{
	sMSDirect->KeyList = MSKeyList;
}

void MSDirect::ResetKeyState(MSKEY pResetKey)
{
	sMSDirect->KeyList[pResetKey] = false;
}

