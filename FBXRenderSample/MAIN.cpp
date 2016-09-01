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
	Render();
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
	DX11BaseShader::Init(m_pDevice, m_pDeviceContext);
	shader.Init();
	shader.InitVertex("Simple.hlsl");
	shader.InitPixel("Simple.hlsl");

	DX11FbxManager::InitDevice(m_pDevice, m_pDeviceContext);
	fbx.Initialize();
	fbx.LoadFile("res/humanoid2.fbx",true);
	
	//どのシェーダーでレンダリングするか登録
	DX11Render::Initialize(m_pDevice, m_pDeviceContext);
	render.shader = &shader;

	return S_OK;
}
//
//
//シーンを画面にレンダリング
void MAIN::Render()
{


	//画面クリア（実際は単色で画面を塗りつぶす処理）
	float ClearColor[4] = { 0,0,1,1 };// クリア色作成　RGBAの順
	m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer_TexRTV, ClearColor);//画面クリア
	m_pDeviceContext->ClearDepthStencilView(m_pBackBuffer_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア

	//D3DXMATRIX mWorld, mRotate;
	//D3DXMATRIX mView;
	//D3DXMATRIX mProj;
	////ワールドトランスフォーム（絶対座標変換）
	//D3DXMatrixTranslation(&mWorld, 0, 0, 0);
	//D3DXMatrixRotationY(&mRotate, /*timeGetTime()/1000.0f*/0);//単純にyaw回転させる
	//// ビュートランスフォーム（視点座標変換）
	//D3DXVECTOR3 vEyePt(0.0f, 0, -100.0f); //カメラ（視点）位置
	//D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	//D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置

	//D3DXMatrixLookAtLH(&mView, &vEyePt, &vLookatPt, &vUpVec);
	//// プロジェクショントランスフォーム（射影変換）
	//D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 2000.0f);

	//fbx.Update();
	////fbx.SetMatrix(mRotate*mWorld, mView, mProj);
	//resource.mWorld = mRotate;
	//resource.mView = mView;
	//resource.mProj = mProj;

	resource.smWorld.SetT(30, 0, 0);
	resource.smWorld.SetRT(0, 0, 90);
	resource.mWorld = *resource.smWorld.GetMatrix();
	resource.mView = *resource.smView.GetMatrix();
	resource.mProj = *resource.smProj.GetMatrix();
	render.Render(&fbx, &resource);
	//D3DXMatrixTranslation(&mWorld, 30.5, 0, 0);
	//resource.mWorld = mRotate*mWorld;
	//render.Render(&fbx, &resource);

	m_pSwapChain->Present(0, 0);//画面更新（バックバッファをフロントバッファに）	

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



