//�x����\��

//�w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include <windows.h>
#include<memory>
//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")


class MSDirect;
//MAIN�N���X�@��`
class MSWindow
{

public:
	MSWindow();
	~MSWindow();
	//WinMain����̌Ăяo��
	void _Run(HINSTANCE, INT, INT, INT, INT, LPSTR);
	//Window�j��
	void Destroy();
	//Window�A�v�����[�v
	HRESULT _Loop();

	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPSTR);
	LRESULT CALLBACK MsgProc(HWND,UINT,WPARAM,LPARAM);
	
	HINSTANCE mHInst;
	LPSTR mWindowName;
	HWND m_hWnd;
	std::weak_ptr<MSDirect>mDirectX;
};

