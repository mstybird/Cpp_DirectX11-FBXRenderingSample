//�x����\��
#pragma warning(disable : 4005)
//�w�b�_�[�t�@�C���̃C���N���[�h
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

//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//�萔��`
#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����
#define APP_NAME "�O�p�|���S��(�ŏ��V�F�[�_�[)"
//�}�N��
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
	D3DXMATRIX mW;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
	D3DXMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
	D3DXVECTOR4 LightDir;
	D3DXVECTOR4 Diffuse;
};
//MAIN�N���X�@��`
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
	//���A�v���ɂЂƂ�
	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBackBuffer_DSTex;
	ID3D11RasterizerState* m_pRasterizerState;
	//�����f���̎�ނ���(���f���̍\�����S�ē���Ȃ�A�v���ɂЂƂj
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	//�����f������
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer*m_pIndexBuffer;
	DX11FbxLoader fbx;
	std::vector<std::vector<FBXModelData*>>*vert;
};
