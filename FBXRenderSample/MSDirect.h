#pragma once
#include<memory>
#include<D3DX11.h>
class MSSceneBase;
class MSDirect {
public:
	static const std::weak_ptr<MSDirect> GetInstance();
	HRESULT InitD3D(HWND pHwnd);
	void Loop();
	//新しいシーンのセット
	static void SetScene(std::unique_ptr<MSSceneBase>&&pScene);
	//ビューポートを取得する
	static D3D11_VIEWPORT*GetViewPort();
	//インスタンスの解放
	static void Destroy();
	MSDirect();
	~MSDirect();
	//メッセージプロシージャ
	//シーンにキー入力などの処理をさせる用
	LRESULT MessageProcedule(HWND, UINT, WPARAM, LPARAM);

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
	D3D11_VIEWPORT mViewPort;

	std::unique_ptr<MSSceneBase>scene;
	//	MSSceneBase scene;
};