//警告非表示
#pragma warning(disable : 4005)
//ヘッダーファイルのインクルード
#include<stdio.h>
#include <windows.h>
#include<string>
//#include<map>
#include<vector>
#include<array>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>

//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//定数定義
#define WINDOW_WIDTH 640 //ウィンドウ幅
#define WINDOW_HEIGHT 480 //ウィンドウ高さ
#define APP_NAME "三角ポリゴン(最小シェーダー)"
//マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

#define _CRTDBG_MAP_ALLOC #include <stdlib.h> #include <crtdbg.h>



//
//struct SimpleIndex {
//	unsigned int x, y, z;
//	
//};



#include"DX11FbxLoader.h"


struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mW;//ワールド、ビュー、射影の合成変換行列
	D3DXMATRIX mWVP;//ワールド、ビュー、射影の合成変換行列
	D3DXVECTOR4 LightDir;
	D3DXVECTOR4 Diffuse;
};
//MAINクラス　定義
class MAIN
{

public:
	MAIN();
	~MAIN();
	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPSTR);
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);
	HRESULT InitD3D();
	HRESULT InitPolygon();
	HRESULT InitShader();
	void Loop();
	void App();
	void Render();
	void DestroyD3D();
	//↓アプリにひとつ
	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBackBuffer_DSTex;
	ID3D11RasterizerState* m_pRasterizerState;
	//↓モデルの種類ごと(モデルの構造が全て同一ならアプリにひとつ）
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	//↓モデルごと
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer*m_pIndexBuffer;
	DX11FbxLoader fbx;
	std::vector<std::vector<FBXModelData*>>*vert;
};
