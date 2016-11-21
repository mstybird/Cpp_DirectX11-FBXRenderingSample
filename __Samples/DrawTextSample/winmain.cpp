//-----------------------------------------------------
//	DirectX テキスト描画サンプル
//	2010.03.04 TMO
//-----------------------------------------------------

#include "stdafx.h"
#include "Graphic.h"
#include "KeyDownCheck.h"

class MyApp
{
	//テクスチャ画像化したテキストデータ
	CTexture	m_texture;
	TextGraphic m_text;
	
	//キー入力チェック
	KeyDownChk	m_chkNumA;
	KeyDownChk	m_chkNumS;
	KeyDownChk	m_chkNumD;
	KeyDownChk	m_chkNumF;
	KeyDownChk	m_chkNumZ;
	KeyDownChk	m_chkNumX;
	KeyDownChk	m_chkNumC;

public:
	//サンプルアプリの初期化処理
	void Init(HWND hWnd)
	{
		//DirectX初期化
		CreateGraphic( hWnd );

		//背景色黒
		GetGraphic()->m_backColor = 0x22222222;

		//背景画像の作成
		m_texture.Load(TEXT(".\\image\\sakura.jpg"));

		//フォントの準備
		LOGFONT	logFont;
		::ZeroMemory( &logFont, sizeof( logFont ) );
	    logFont.lfHeight = 20;	//フォントサイズ
		logFont.lfWidth = 0;
		logFont.lfEscapement = 0;
		logFont.lfOrientation = 0;
		logFont.lfWeight = FW_EXTRABOLD;
		logFont.lfItalic = 0;
		logFont.lfUnderline = 0;
		logFont.lfStrikeOut = 0;
		logFont.lfCharSet   = SHIFTJIS_CHARSET;
		logFont.lfOutPrecision = OUT_TT_ONLY_PRECIS;
		logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logFont.lfQuality = PROOF_QUALITY;
		logFont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;

#ifdef UNICODE
		//wcscpy_sの第二引数は文字(配列)数(バッファサイズだとデバッガが終了しなかったりする)
		wcscpy_s( logFont.lfFaceName, LF_FACESIZE, _T("ＭＳ 明朝") );
#else
		strcpy_s( logFont.lfFaceName, LF_FACESIZE, _T("ＭＳ 明朝") );
#endif	
		
		//テキスト画像の作成
		m_text.Create(
			_T("こ字表示のテストプログラムです。\nビジュアルノベルみたいにしてみました。\n")
			_T("一文字ごとにテクスチャをつくって描画をします。指定エリアからはみ出ると折り返してますよ。\n")
			_T("\n")
			_T("Zキー:左寄せ\n")
			_T("Xキー:中央寄せ\n")
			_T("Cキー:右寄せ\n")
			_T("\n")
			_T("Aキー:テキスト通常描画\n")
			_T("Sキー:テキスト半透明\n")
			_T("Dキー:テキスト透明（※見えなくなります）\n")
			_T("\n")
			_T("Fキー:文字色を赤にする\n")
			,50,50,640-50,480-50//テキストの描画エリア
			,logFont );


		//キーダウンチェックの登録
		m_chkNumA.SetKey('A');
		m_chkNumS.SetKey('S');
		m_chkNumD.SetKey('D');
		m_chkNumF.SetKey('F');
		m_chkNumZ.SetKey('Z');
		m_chkNumX.SetKey('X');
		m_chkNumC.SetKey('C');

	}

	void Roop()
	{
		//キー入力による処理
		if ( m_chkNumA.Actioned() ) {
			m_text.SetTextAlpha(0xffffffff);
		} else
		if ( m_chkNumS.Actioned() ) {
			m_text.SetTextAlpha(0x88888888);
		} else
		if ( m_chkNumD.Actioned() ) {
			m_text.SetTextAlpha(0x00000000);
		} else
		if ( m_chkNumF.Actioned() ) {
			//文字の色を白で作っているので、アルファ以外のビットをいじって赤いテキストにすることもできます。
			m_text.SetTextAlpha(0xffff0000);
		} else
		if ( m_chkNumZ.Actioned() ) {
			m_text.SetTextAlign(TextAlign::AlignLeft);
		} else
		if ( m_chkNumX.Actioned() ) {
			m_text.SetTextAlign(TextAlign::AlignCenter);
		} else
		if ( m_chkNumC.Actioned() ) {
			m_text.SetTextAlign(TextAlign::AlignRight);
		}

		//以降描画処理------------

		GetGraphic()->ClearAndBegin();

		//背景描画
		GetGraphic()->Draw( 0, 0, 640, 480, &m_texture, 0xffffffff );

		//背景を部分的に暗くするために半透明の黒い板を描画
		GetGraphic()->DrawColorRect(50,50,640-50, 480-50,0xaa000000);
	
		//テキスト描画
		m_text.Draw();

		GetGraphic()->EndAndPresent();
	}

	//サンプルアプリの解放処理
	void Release()
	{
		//テクスチャの解放
		m_text.Release();

		//DirectXの解放
		ReleaseGraphic();
	}

};
static MyApp myApp;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		//WM_PAINTでは描画しません。
		//ValidateRectを使うと次からWM_PAINTが呼ばれなくなる。（フルスクリーン表示の時だけ）
		ValidateRect( hWnd, NULL );
		break;
	case WM_DESTROY:
		myApp.Release();//サンプルアプリ解放処理
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	
	//ウィンドウクラスの登録
	WNDCLASSEX wcex;
	memset( &wcex, 0, sizeof(wcex) );
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= TEXT("wcDrawSample");

	if( !RegisterClassEx(&wcex) ) {
		return FALSE;
	}

	HWND hWnd;
	hWnd = CreateWindow(TEXT("wcDrawSample"), TEXT("描画サンプル"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	CW_USEDEFAULT, 0, 640, 480, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	
	{
		//クライアントサイズを640＊480にする。
		//現在ウィンドウサイズが640＊480なのでクライアントサイズを取得して枠やタイトル幅を求める
		RECT rect;
		GetClientRect( hWnd, &rect );
		int ofsW = 640 - ( rect.right - rect.left );
		int ofsH = 480 - ( rect.bottom - rect.top );
		SetWindowPos( hWnd, NULL, 0, 0, 640+ofsW, 480+ofsH, SWP_NOMOVE | SWP_NOZORDER );
	}

	//サンプルアプリの初期化
	myApp.Init(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	for (;;) {
		if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
			
			if ( GetMessage( &msg, NULL, 0, 0 ) > 0) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} else {
				break;
			}

		} else {
			//ゲームの処理をここでする
			myApp.Roop();
		}
	}

	return (int) msg.wParam;
}
