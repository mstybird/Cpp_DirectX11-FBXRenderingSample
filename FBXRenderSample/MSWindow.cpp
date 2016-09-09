#include "MSWindow.h"
#include"MyScene.h"
#include"Helper.h"
#include"MSDirect.h"
#include"DX11Resrouce.h"
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

