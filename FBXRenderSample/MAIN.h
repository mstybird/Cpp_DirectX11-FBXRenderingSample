//�x����\��

//�w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include<stdio.h>
#include <windows.h>
#include<string>
//#include<map>
#include<vector>
#include<array>
#include <d3dx9.h>
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



#include"Dx11Manager.h"


struct SIMPLESHADER_CONSTANT_BUFFER1
{
	D3DXMATRIX mW;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
	D3DXMATRIX mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
	D3DXVECTOR4 LightDir;
};

struct SIMPLESHADER_CONSTANT_BUFFER2
{
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
	void Loop();
	void App();
	void Update();
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

	DX11TextureManager textureManager;

	DX11FbxManager fbx;				//���f���f�[�^
	DX11FbxManager mbox;				//���f���f�[�^
	DX11RenderResource box;	//�{�b�N�X�ړ�
	DX11RenderResource ground;
	DX11RenderResource me;	//���g

	DX11SpriteResource spRes;	//�X�v���C�g���\�[�X
	DX11BaseSprite sprite;	//�X�v���C�g�V�F�[�_
	DX11Sprite2D render2D;//�X�v���C�g�����_���[

	MyDX11Shader shader;				//�`��Ɏg���V�F�[�_
	//�V�[���Ɉ��
	DX11Render render;
//	DX11BaseRender render;				//�`�惂�f��
};
