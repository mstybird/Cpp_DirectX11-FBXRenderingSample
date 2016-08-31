#pragma once
//DirectX11で描画するクラス
#include<D3DX11.h>
#include"DX11RenderResource.h"
#include"DX11FbxManager.h"
class DX11BaseShader;
class DX11Render {

public:
	static void Initialize(
		ID3D11Device*pDevice,
		ID3D11DeviceContext*pDeviceContext);
	void Render(DX11FbxManager*fbxManager,DX11RenderResource*resource);
	DX11BaseShader*shader;
private:
	//どのシェーダを使うか
private:
	 static ID3D11Device*sDevice;				//DirectX11デバイス
	 static ID3D11DeviceContext*sDeviceContext;	//DirectX11デバイスコンテキスト

};

#include"DX11BaseShader.h"