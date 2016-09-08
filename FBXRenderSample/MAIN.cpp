#include "MAIN.h"

#include"Helper.h"
#include<cassert>
#include<memory>
/*
	�����F
	���̍��
	GetNode�̃A�j���[�V�������f
*/



//�O���[�o���ϐ�
MSWindow* g_pMain=NULL;
//�֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
//
//
//�A�v���P�[�V�����̃G���g���[�֐� 
INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,INT)
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#ifdef _DEBUG
	AllocConsole();
	FILE *fp;
	AllocConsole();
	freopen_s(&fp, "CON", "w", stdout);
//#endif
	g_pMain=new MSWindow;
	g_pMain->_Run(hInstance, 0, 0, WINDOW_WIDTH,
		WINDOW_HEIGHT, APP_NAME);
	delete g_pMain;

//#ifdef _DEBUG
	fclose(fp);
	FreeConsole();
//#endif
	int a = 0;

	return 0;
}
//
//
//OS���猩���E�B���h�E�v���V�[�W���[�i���ۂ̏�����MAIN�N���X�̃v���V�[�W���[�ŏ����j
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return g_pMain->MsgProc(hWnd,uMsg,wParam,lParam);
}
//
//
//�N���X�R���X�g���N�^�[
MSWindow::MSWindow()
{
	//ZeroMemory(this,sizeof(MAIN));
}
//
//
//�N���X�f�X�g���N�^�[
MSWindow::~MSWindow()
{
	
}
void MSWindow::_Run(HINSTANCE hInstance,
	INT iX, INT iY, INT iWidth, INT iHeight, LPSTR WindowName)
{
	if (FAILED(g_pMain->InitWindow(hInstance, iX, iY, iWidth,
		iHeight, WindowName)))
	{
		assert(0);
		return;
	}
	
	std::unique_ptr<MSSceneBase> lScene = std::make_unique<MSSceneBase>();
	
	
	//mDirectX.lock()->InitD3D(m_hWnd);


	mDirectX = MSDirect::GetInstance();
	if (FAILED(mDirectX.lock()->InitD3D(m_hWnd))) {
		assert(0);
		return;
	}
	
	MSDirect::SetScene(std::make_unique<MyMSScene>());
	_Loop();


}
HRESULT MSWindow::_Loop()
{
	// ���b�Z�[�W���[�v
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//�A�v���P�[�V�����̏����͂��������ԁB
			//App();
			mDirectX.lock()->Loop();

		}
	}
	return S_OK;
}
//
//
//�E�B���h�E�쐬
HRESULT MSWindow::InitWindow(HINSTANCE hInstance,
		INT iX,INT iY,INT iWidth,INT iHeight,LPSTR WindowName)
{
	// �E�B���h�E�̒�`
	WNDCLASSEX  wc;
	ZeroMemory(&wc,sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = WindowName;
	wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	RegisterClassEx(&wc);
	//�E�B���h�E�̍쐬
	m_hWnd=CreateWindow(WindowName,WindowName,WS_OVERLAPPEDWINDOW,
	 0,0,iWidth,iHeight,0,0,hInstance,0);
	if(!m_hWnd)
	{
	 return E_FAIL;
	}
	//�E�C���h�E�̕\��
	ShowWindow(m_hWnd,SW_SHOW);
	UpdateWindow(m_hWnd) ;

	return S_OK;
}
//
//
//�E�B���h�E�v���V�[�W���[
LRESULT MSWindow::MsgProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch(iMsg)
	{
		case WM_KEYDOWN:
		switch((char)wParam)
		{
			case VK_ESCAPE://ESC�L�[�ŏC��
			PostQuitMessage(0);
			break;
		}
		break;
		case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc (hWnd, iMsg, wParam, lParam);
}

std::shared_ptr<MSDirect> MSDirect::sMSDirect{std::make_shared<MSDirect>()};

MSDirect::MSDirect():
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
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pRasterizerState);
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
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH / 2;
	vp.Height = WINDOW_HEIGHT / 2;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);
	//���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = TRUE;

	m_pDevice->CreateRasterizerState(&rdc, &m_pRasterizerState);
	m_pDeviceContext->RSSetState(m_pRasterizerState);

	//�e���b�p�[��DirectX11�f�o�C�X����o�^
	MSVertexShader::sInitialize(m_pDevice, m_pDeviceContext);
	MSPixelShader::sInitialize(m_pDevice, m_pDeviceContext);
	DX11BaseShader::Init(m_pDevice, m_pDeviceContext);
	DX11BaseSprite::Init(m_pDevice, m_pDeviceContext);
	DX11Sprite2D::Initialize(m_pDevice, m_pDeviceContext, m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV);
	DX11Render::Initialize(m_pDevice, m_pDeviceContext, m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV);

	DX11FbxManager::InitDevice(m_pDevice, m_pDeviceContext);
	DXProjection::SetAspect((FLOAT)WINDOW_WIDTH, (FLOAT)WINDOW_HEIGHT);
	DXTexture::Initialize(m_pDevice);


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
	sMSDirect->scene= std::move(pScene);
	lBufferPtr.reset();
}

void MyMSScene::Initialize()
{
	//�e�N�X�`���o�^
	textureManager.RegisterFile("res/Chips_Cover.jpg", 0);

	shader.Init();
	shader.InitVertex("Simple.hlsl");
	shader.InitPixel("Simple.hlsl");

	mbox.LoadFile("res/Chips.fbx", true);

	//�ǂ̃V�F�[�_�[�Ń����_�����O���邩�o�^
	render.SetShader(&shader);
	//���̃��\�[�X�������_�����O�p�Ɏg��
	me.InitRenderMatrix();
	//�����_�[�ɃJ������o�^
	render.SetRenderTarget(&me);

	auto lWorld = me.GetWorld();
	auto lView = me.GetCamera();
	auto lProjection = me.GetProjection();

	auto world = me.GetWorld();
	world->SetT(70, 20, -80);
	lView->SetCamera(*me.GetWorld(), *box.GetWorld());

	lProjection->SetViewAngle(45);
	lProjection->SetPlaneNear(0.1f);
	lProjection->SetPlaneFar(2000.0f);

	box.GetWorld()->SetT(0, 0, 0);
	//box.GetWorld()->SetRC(45, 45, 0);

	ground.GetWorld()->SetS(1.0f, 0.001f, 1.0f);

}

void MyMSScene::Render()
{
	////��ʃN���A
	DX11Render::Clear({ 1,0,0,1 });
	mbox.Update();
	ground.GetWorld()->SetS(1, 1, 1);
	render.Render(&mbox, &ground);

}
