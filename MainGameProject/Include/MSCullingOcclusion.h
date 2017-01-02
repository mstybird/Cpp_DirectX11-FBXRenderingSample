#pragma once
#include<d3dx11.h>
#include<functional>
#include"MS3DRender.h"
#include"MSFbxManager.h"
#include"DX11RenderResource.h"
#include"DXProjection.h"
#include<memory>
#include<unordered_map>
class GameObjectBase;
class MSCullingOcculusion abstract{
public:
	//視点リソースから見たターゲットのオクルージョン判定
	/*
		pRender			:レンダリングに使う初期化済みのレンダラー
		pEyeResource	:カメラの位置
		pEyeProjection	:カメラの視野
		pTargetResource	:ターゲットの位置
		pTargetMesh		:ターゲットのメッシュ
		pPixelper		:発見したと判定するピクセルの割合0~1.0f
		pRenderFunc		:障害となるオブジェクトの描画処理
	*/
	static bool IsCullingWorld(
		std::vector<GameObjectBase*>*pHitList,
		MS3DRender*pRender,
		GameObjectBase*pEyeResource,
		std::vector<GameObjectBase*>*pTargetResource,
		float pPixelper,
		std::function<void(void)>pRenderFunc
		);
	static void Initialize(ID3D11Device*pDevice, ID3D11DeviceContext*pDeviceContext,
		IDXGISwapChain*pSwapChain);

	static void Release();
private:

	static void CreateOcclusionQuery();
	static void BeginOcclusionQuery();
	static void EndOcclusionQuery();

	//カリングで使用する
	static ID3D11RenderTargetView*sRTV;
	static ID3D11DepthStencilView*sDSV;
	static ID3D11Texture2D*sDS2D;

	static ID3D11Query* sOcculusionQuery;
	static ID3D11Device*sDevice;
	static ID3D11DeviceContext*sDeviceContext;
	static IDXGISwapChain*sSwapChain;
};