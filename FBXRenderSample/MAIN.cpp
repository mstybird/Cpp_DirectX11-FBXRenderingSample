#include "MAIN.h"


/*
	メモ：
	次の作業
	GetNodeのアニメーション反映
*/



//グローバル変数
MAIN* g_pMain=NULL;
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

	clock_t start, end;
	start = clock();
	g_pMain=new MAIN;
	if(g_pMain != NULL)
	{
		//FBX初期化
		if(SUCCEEDED(g_pMain->InitWindow(hInstance,0,0,WINDOW_WIDTH,
			WINDOW_HEIGHT,APP_NAME)))
		{


			auto succeeded = g_pMain->InitD3D();
			end = clock();
			printf("Initialize:%dms\n", end - start);
			if(SUCCEEDED(succeeded))
			{
				g_pMain->Loop();
			}
		}

		//アプリ終了
		delete g_pMain;
	}
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
MAIN::MAIN()
{
	//ZeroMemory(this,sizeof(MAIN));
}
//
//
//クラスデストラクター
MAIN::~MAIN()
{
	DestroyD3D();
}
//
//
//ウィンドウ作成
HRESULT MAIN::InitWindow(HINSTANCE hInstance,
		INT iX,INT iY,INT iWidth,INT iHeight,LPSTR WindowName)
{
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
//
//
//ウィンドウプロシージャー
LRESULT MAIN::MsgProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch(iMsg)
	{
		case WM_KEYDOWN:
		switch((char)wParam)
		{
			case VK_ESCAPE://ESCキーで修了
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
//
//
//メッセージループとアプリケーション処理の入り口
void MAIN::Loop()
{
	// メッセージループ
	MSG msg={0};
	ZeroMemory(&msg,sizeof(msg));
	while(msg.message!=WM_QUIT)
	{
		if( PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//アプリケーションの処理はここから飛ぶ。
			App();
		}
	}
}
//
//
//アプリケーション処理。アプリのメイン関数。
void MAIN::App()
{
	Update();
	Render();
	m_pSwapChain->Present(1, 0);//画面更新（バックバッファをフロントバッファに）	

}

//
//
//Direct3D初期化
HRESULT MAIN::InitD3D()
{
	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width=WINDOW_WIDTH;
	sd.BufferDesc.Height=WINDOW_HEIGHT;
	sd.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator=60;
	sd.BufferDesc.RefreshRate.Denominator=1;
	sd.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow=m_hWnd;
	sd.SampleDesc.Count=1;
	sd.SampleDesc.Quality=0;
	sd.Windowed=TRUE;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;
	
	if( FAILED( D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,
		0,&pFeatureLevels,1,D3D11_SDK_VERSION,&sd,&m_pSwapChain,&m_pDevice,
		pFeatureLevel,&m_pDeviceContext )))
	{
		return FALSE;
	}
	//各種テクスチャーと、それに付帯する各種ビューを作成

	//バックバッファーテクスチャーを取得（既にあるので作成ではない）
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ),(LPVOID*)&pBackBuffer_Tex);
	//そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
	m_pDevice->CreateRenderTargetView( pBackBuffer_Tex, NULL, &m_pBackBuffer_TexRTV );
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
	m_pDevice->CreateTexture2D( &descDepth, NULL, &m_pBackBuffer_DSTex );
	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	m_pDevice->CreateDepthStencilView( m_pBackBuffer_DSTex, NULL, &m_pBackBuffer_DSTexDSV);

	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBuffer_TexRTV,m_pBackBuffer_DSTexDSV);
	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports( 1, &vp );
	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc,sizeof(rdc));
	rdc.CullMode=D3D11_CULL_NONE;
	rdc.FillMode=D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = TRUE;
	
	m_pDevice->CreateRasterizerState(&rdc,&m_pRasterizerState);
	m_pDeviceContext->RSSetState(m_pRasterizerState);	

	

	//各ラッパーにDirectX11デバイス等を登録
	DX11BaseShader::Init(m_pDevice, m_pDeviceContext);
	DX11FbxManager::InitDevice(m_pDevice, m_pDeviceContext);

	DX11Render::Initialize(m_pDevice, m_pDeviceContext,m_pBackBuffer_TexRTV,m_pBackBuffer_DSTexDSV);
	DXProjection::SetAspect((FLOAT)WINDOW_WIDTH, (FLOAT)WINDOW_HEIGHT);

	//ポリゴン作成
	if(FAILED(InitPolygon()))
	{
		return E_FAIL;
	}
	return S_OK;
}

//バーテックスバッファー作成

D3DXVECTOR3 pos;

HRESULT MAIN::InitPolygon()
{

	DXVector3 v1 = {0,0,0};
	DXVector3 v2 = {1,1,1};

	float x, y, z;
	x = DXVector3::GetAngleX(DXVector3::TYPE_DEGREE, v1, v2);
	y = DXVector3::GetAngleY(DXVector3::TYPE_DEGREE, v1, v2);
	z = DXVector3::GetAngleZ(DXVector3::TYPE_DEGREE, v1, v2);


	shader.Init();
	shader.InitVertex("Simple.hlsl");
	shader.InitPixel("Simple.hlsl");

	fbx.Initialize();
	fbx.LoadFile("res/meshes.fbx", true);
	mbox.LoadFile("res/box.fbx",true);
	
	//どのシェーダーでレンダリングするか登録
	render.SetShader(&shader);
	//このリソースをレンダリング用に使う
	me.InitRenderMatrix();
	//レンダーにカメラを登録
	render.SetRenderTarget(&me);

	auto lWorld = me.GetWorld();
	auto lView= me.GetCamera();
	auto lProjection = me.GetProjection();

	auto world = me.GetWorld();
	world->SetT(0, 0, 0);
	lView->SetEyeT(0, 0, 10);
	lView->SetLookT(0.0f, 0.0f, 0.0f);
	lView->SetUpV(0.0f, 1.0f, 0.0f);
	
	lProjection->SetViewAngle(45);
	lProjection->SetPlaneNear(0.1f);
	lProjection->SetPlaneFar(2000.0f);

	box.GetWorld()->SetT(0, 0, 0);
	//box.GetWorld()->SetRC(45, 45, 0);

	ground.GetWorld()->SetS(1, 0.001, 1);

	return S_OK;
}

void MAIN::Update()
{
	auto world = box.GetWorld();
	auto camera = me.GetCamera();

	//ボックスの平行移動
	if (GetAsyncKeyState('W')) {
		world->AddT(DXWorld::TYPE_ROTATE, 0.25);
	}
	if (GetAsyncKeyState('S')) {
		world->AddT(DXWorld::TYPE_ROTATE, -0.25);
	}
	//カメラの回転
	if (GetAsyncKeyState('A')) {
		world->AddRC(0, 1, 0);
	}
	if (GetAsyncKeyState('D')) {
		world->AddRC(0, -1, 0);
	}



	//カメラの平行移動
	if (GetAsyncKeyState(VK_LEFT)) {
		camera->AddTranslation(DXCamera::TYPE_ROTATE, -0.25, { 1,0,0 });
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		camera->AddTranslation(DXCamera::TYPE_ROTATE, 0.25, { 1,0,0 });
	}

	//ボックスの回転
	if (GetAsyncKeyState(VK_UP)) {
		if (GetAsyncKeyState(VK_SHIFT)) {
			camera->AddTranslation(DXCamera::TYPE_ROTATE, 0.25, { 0,1,0 });
		}
		else {
			camera->AddTranslation(DXCamera::TYPE_ROTATE, 0.25);
		}
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		if (GetAsyncKeyState(VK_SHIFT)) {
			camera->AddTranslation(DXCamera::TYPE_ROTATE, -0.25, { 0,1,0 });
		}
		else {
			camera->AddTranslation(DXCamera::TYPE_ROTATE, -0.25);
		}
	}

}

//
//
//シーンを画面にレンダリング
void MAIN::Render()
{
	////画面クリア
	DX11Render::Clear({ 0,0,1,1 });

	fbx.Update();
	mbox.Update();
	render.Render(&fbx, &box);
	//render.Render(&mbox, &ground);



}

//
//
//全てのインターフェイスをリリース
void MAIN::DestroyD3D()
{
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}



