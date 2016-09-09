#include "MSWindow.h"

#include"Helper.h"
#include<cassert>
#include<memory>
/*
	メモ：
	次の作業
	GetNodeのアニメーション反映
*/



//グローバル変数
MSWindow* g_pMain=NULL;
//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
//
//
//アプリケーションのエントリー関数 
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
//OSから見たウィンドウプロシージャー（実際の処理はMAINクラスのプロシージャーで処理）
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return g_pMain->MsgProc(hWnd,uMsg,wParam,lParam);
}
//
//
//クラスコンストラクター
MSWindow::MSWindow()
{
	//ZeroMemory(this,sizeof(MAIN));
}
//
//
//クラスデストラクター
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
	
	//std::unique_ptr<MSSceneBase> lScene = std::make_unique<MSSceneBase>();
	
	
	//mDirectX.lock()->InitD3D(m_hWnd);


	mDirectX = MSDirect::GetInstance();
	if (FAILED(mDirectX.lock()->InitD3D(m_hWnd))) {
		assert(0);
		return;
	}
	
	MSDirect::SetScene(std::make_unique<MyMSScene>());
	_Loop();
	Destroy();


}
void MSWindow::Destroy()
{
	MSDirect::Destroy();
	UnregisterClass(mWindowName, mHInst);
}
HRESULT MSWindow::_Loop()
{
	// メッセージループ
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
			//アプリケーションの処理はここから飛ぶ。
			//App();
			mDirectX.lock()->Loop();

		}
	}
	return S_OK;
}
//
//
//ウィンドウ作成
HRESULT MSWindow::InitWindow(HINSTANCE hInstance,
		INT iX,INT iY,INT iWidth,INT iHeight,LPSTR WindowName)
{
	mWindowName = WindowName;
	// ウィンドウの定義
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
	//ウィンドウの作成
	m_hWnd=CreateWindow(WindowName,WindowName,WS_OVERLAPPEDWINDOW,
	 0,0,iWidth,iHeight,0,0,hInstance,0);
	if(!m_hWnd)
	{
	 return E_FAIL;
	}
	//ウインドウの表示
	ShowWindow(m_hWnd,SW_SHOW);
	UpdateWindow(m_hWnd) ;

	return S_OK;
}

//ウィンドウプロシージャー
LRESULT MSWindow::MsgProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{

	if (!mDirectX.expired()) {
		return mDirectX.lock()->MessageProcedule(hWnd, iMsg, wParam, lParam);
	}
	else {
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}


}

std::shared_ptr<MSDirect> MSDirect::sMSDirect{std::make_shared<MSDirect>()};

void MSDirect::Destroy()
{
	sMSDirect.reset();
}

MSDirect::MSDirect():
	mHwnd{nullptr},
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

LRESULT MSDirect::MessageProcedule(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	if (scene.get() != nullptr) {

	switch (iMsg)
	{
	case WM_KEYDOWN:
		scene->KeyDown(wParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		//		break;
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
	// デバイスとスワップチェーンの作成
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


	//各種テクスチャーと、それに付帯する各種ビューを作成
	//バックバッファーテクスチャーを取得（既にあるので作成ではない）
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
	//そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
	m_pDevice->CreateRenderTargetView(pBackBuffer_Tex, NULL, &m_pBackBuffer_TexRTV);
	SAFE_RELEASE(pBackBuffer_Tex);

	//デプスステンシルビュー用のテクスチャーを作成
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
	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	m_pDevice->CreateDepthStencilView(m_pBackBuffer_DSTex, NULL, &m_pBackBuffer_DSTexDSV);

	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBuffer_TexRTV, m_pBackBuffer_DSTexDSV);
	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH / 2;
	vp.Height = WINDOW_HEIGHT / 2;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);
	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = TRUE;

	m_pDevice->CreateRasterizerState(&rdc, &m_pRasterizerState);
	m_pDeviceContext->RSSetState(m_pRasterizerState);

	//各ラッパーにDirectX11デバイス等を登録
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
	//テクスチャ登録
	textureManager.RegisterFile("res/Chips_Cover.jpg", 0);

	shader.Init();
	shader.InitVertex("Simple.hlsl");
	shader.InitPixel("Simple.hlsl");

	mbox.LoadFile("res/Chips.fbx", true);

	//どのシェーダーでレンダリングするか登録
	render.SetShader(&shader);
	//このリソースをレンダリング用に使う
	me.InitRenderMatrix();
	//レンダーにカメラを登録
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
	////画面クリア
	DX11Render::Clear({ 1,0,0,1 });
	mbox.Update();
	ground.GetWorld()->SetS(1, 1, 1);
	render.Render(&mbox, &ground);

}
