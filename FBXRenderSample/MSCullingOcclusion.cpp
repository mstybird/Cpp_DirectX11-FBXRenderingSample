#include "MSCullingOcclusion.h"
#include"MSDirect.h"
#include"DX11Resrouce.h"
MSCullingOcculusion MSCullingOcculusion::Create()
{
	//生成するオクルージョンカリングインスタンス
	MSCullingOcculusion lResultCO;

	//カリングで使うレンダーターゲットビューの作成
	{
		ID3D11Texture2D*lBackBuffer;
		MSDirect::GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&lBackBuffer);
		MSDirect::GetDevice()->CreateRenderTargetView(lBackBuffer, nullptr, &lResultCO.mRTV);
		SAFE_RELEASE(lBackBuffer);
	}

	//カリングで使うステンシルビューの作成
	{
		D3D11_TEXTURE2D_DESC lDescDepth;
		lDescDepth.Width = 640;
		lDescDepth.Height = 480;
		lDescDepth.MipLevels = 1;
		lDescDepth.ArraySize = 1;
		lDescDepth.Format = DXGI_FORMAT_D32_FLOAT;
		lDescDepth.SampleDesc.Count = 1;
		lDescDepth.SampleDesc.Quality = 0;
		lDescDepth.Usage = D3D11_USAGE_DEFAULT;
		lDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		lDescDepth.CPUAccessFlags = 0;
		lDescDepth.MiscFlags = 0;

		MSDirect::GetDevice()->CreateTexture2D(&lDescDepth, nullptr, &lResultCO.mDS2D);
		MSDirect::GetDevice()->CreateDepthStencilView(lResultCO.mDS2D, nullptr, &lResultCO.mDSV);

	}

	//実際にカリングする処理から続き


}
