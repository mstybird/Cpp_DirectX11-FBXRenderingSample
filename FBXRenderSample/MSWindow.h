//警告非表示

//ヘッダーファイルのインクルード
#pragma once
#include <windows.h>
#include<memory>
//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")


class MSDirect;
//MAINクラス　定義
class MSWindow
{

public:
	MSWindow();
	~MSWindow();
	//WinMainからの呼び出し
	void _Run(HINSTANCE, INT, INT, INT, INT, LPSTR);
	//Window破棄
	void Destroy();
	//Windowアプリループ
	HRESULT _Loop();

	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPSTR);
	LRESULT CALLBACK MsgProc(HWND,UINT,WPARAM,LPARAM);
	
	HINSTANCE mHInst;
	LPSTR mWindowName;
	HWND m_hWnd;
	std::weak_ptr<MSDirect>mDirectX;
};

