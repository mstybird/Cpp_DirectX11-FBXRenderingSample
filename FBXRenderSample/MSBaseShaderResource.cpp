#include"MSBaseShaderResource.h"
#include"DX11Resrouce.h"

ID3D11Device*MSBaseShaderResource::sDevice;
ID3D11DeviceContext*MSBaseShaderResource::sDeviceContext;

MSBaseShaderResource::MSBaseShaderResource() :
	mCompiledShader{ nullptr },
	mErrors{ nullptr }
{
}

MSBaseShaderResource::~MSBaseShaderResource()
{
	SAFE_RELEASE(mCompiledShader);
	SAFE_RELEASE(mErrors);

}


void MSBaseShaderResource::sInitialize(ID3D11Device * pDevice, ID3D11DeviceContext*pDeviceContext)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
}
