#pragma once
#include<memory>
#include<D3DX11.h>
#include<unordered_map>
#include"MSLayout.h"
#include<DXMath.hpp>
class MSSceneBase;
enum class MSKEY;

class MSDirect {
public:
	static const std::weak_ptr<MSDirect> GetInstance();
	HRESULT InitD3D(HWND pHwnd);
	void Loop();
	//�V�����V�[���̃Z�b�g
	static void SetScene(std::unique_ptr<MSSceneBase>&&pScene);
	//�r���[�|�[�g���擾����
	static D3D11_VIEWPORT*GetViewPort();
	//static ID3D11RenderTargetView* GetRTV();
	//static ID3D11DepthStencilView* GetDSV();
	static MSView* GetView();
	static MSView* GetActiveView();
	static MSView* SetActiveView(MSView* pView);
	static void SetDefaultView();
	static HWND GetWinHandle();

	static ID3D11Device*GetDevice();
	static ID3D11DeviceContext*GetDeviceContext();
	static IDXGISwapChain*GetSwapChain();
	//�L�[���͏�Ԃ�����������
	static void ResetKeyStateAll();
	static void ResetKeyState(MSKEY pResetKey);
	//�C���X�^���X�̉��(�V�[������͌Ă΂Ȃ�)
	static void Destroy();
	MSDirect();
	~MSDirect();
	//���b�Z�[�W�v���V�[�W��
	//�V�[���ɃL�[���͂Ȃǂ̏�����������p
	LRESULT MessageProcedule(HWND, UINT, WPARAM, LPARAM);

private:

	static std::shared_ptr<MSDirect>sMSDirect;
	HWND mHwnd;


	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;


	MSView mView;	//MSDirect�����r���[���
	MSView* mActiveView;//���݃A�N�e�B�u�ȃr���[

	//ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	//ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	//ID3D11Texture2D* m_pBackBuffer_DSTex;

	ID3D11RasterizerState* m_pRasterizerState;
	D3D11_VIEWPORT mViewPort;

	std::unordered_map<MSKEY, bool> KeyList;
	std::unique_ptr<MSSceneBase>scene;
	LONG sceneCounter;
	//	MSSceneBase scene;
	bool mIsSceneChanging = false;

	//�}�E�X�����ʒu
	POINT mMouseDefaultPosition;
	//���O�t���[���ʒu
	POINT mMouseBeforePosition;

	//�}�E�X���݈ʒu
	POINT mMouseNowPosition;

};