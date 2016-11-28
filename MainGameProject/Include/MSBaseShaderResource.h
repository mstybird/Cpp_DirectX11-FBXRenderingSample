#pragma once
#include<D3DX11.h>
class MSBaseShaderResource abstract {
public:
	static void sInitialize(ID3D11Device*pDevice, ID3D11DeviceContext*pDeviceContext);
	MSBaseShaderResource();
	virtual ~MSBaseShaderResource();

	virtual void SetShader() = 0;

protected:
	ID3DBlob*mCompiledShader;
	ID3DBlob*mErrors;

	static ID3D11Device*sDevice;
	static ID3D11DeviceContext*sDeviceContext;
};
