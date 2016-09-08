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
#include<memory>
//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//�萔��`
#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����
#define APP_NAME "DirectX11 Program"

#define _CRTDBG_MAP_ALLOC #include <stdlib.h> #include <crtdbg.h>

#include"Dx11Manager.h"

//�Q�[���V�[��
class MSSceneBase abstract{
public:
	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Render() {};
	virtual void Destroy() {};
	virtual ~MSSceneBase() {};
};

class MyMSScene :public MSSceneBase {
public:
	~MyMSScene() {
		printf("B\n");
	}
private:
	void Initialize();
	void Update() {}
	void Render();
	void Destroy() {}
private:
	DX11TextureManager textureManager;

	//DX11FbxManager fbx;				//���f���f�[�^
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

};


class MSDirect {
public:
	static const std::weak_ptr<MSDirect>& GetInstance(){ return sMSDirect; }
	HRESULT InitD3D(HWND pHwnd);
	void Loop();
	static void SetScene(std::unique_ptr<MSSceneBase>&&pScene);
	MSDirect();
	~MSDirect();

private:
	
	static std::shared_ptr<MSDirect>sMSDirect;
	HWND mHwnd;

	
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBackBuffer_DSTex;
	ID3D11RasterizerState* m_pRasterizerState;
	
 	std::unique_ptr<MSSceneBase>scene;
//	MSSceneBase scene;
};

//MAIN�N���X�@��`
class MSWindow
{

public:
	MSWindow();
	~MSWindow();
	//WinMain����̌Ăяo��
	void _Run(HINSTANCE, INT, INT, INT, INT, LPSTR);
	//Window�A�v�����[�v
	HRESULT _Loop();

	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPSTR);
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);
	//���A�v���ɂЂƂ�
	HWND m_hWnd;
	std::weak_ptr<MSDirect>mDirectX;
};

