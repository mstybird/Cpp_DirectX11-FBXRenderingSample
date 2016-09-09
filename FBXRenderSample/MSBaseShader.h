#pragma once
#include<D3DX11.h>
#include<string>
#include<vector>
#include"MSVertexShader.h"
#include"MSPixelShader.h"
class MSBaseShader abstract {
public:
	static void Init(ID3D11Device*pDevice, ID3D11DeviceContext*pDeviceContext);
	virtual void Init() = 0;
	void InitVertex(
		const std::string&fileName,
		const std::string&EntryPoint = "VS",
		const std::string&Version = "vs_5_0"
	);
	void InitPixel(
		const std::string&fileName,
		const std::string&EntryPointp = "PS",
		const std::string&Version = "ps_5_0"
	);

	virtual void SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout) = 0;
	MSVertexShader mVertexShader;
	MSPixelShader mPixelShader;
	static ID3D11Device*sDevice;				//DirectX11デバイス
	static ID3D11DeviceContext*sDeviceContext;	//DirectX11デバイスコンテキスト

};
