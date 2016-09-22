#pragma once
#include<d3dx11.h>
class MSCullingOcculusion {
public:
	bool IsCulling();
	MSCullingOcculusion Create();
private:
	//ビューの作成が必須であるため、
	//コンストラクタでそれを行いたいが、
	//D3D11初期化前にコンストラクタが実行されると
	//ビューの作成ができない(最悪アクセス違反で落ちる)
	MSCullingOcculusion();
	//カリングで使用する
	ID3D11RenderTargetView*mRTV;
	ID3D11DepthStencilView*mDSV;
	ID3D11Texture2D*mDS2D;

};