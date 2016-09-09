#include"MSBaseShader.h"


ID3D11Device*MSBaseShader::sDevice;				//DirectX11デバイス
ID3D11DeviceContext*MSBaseShader::sDeviceContext;	//DirectX11デバイスコンテキスト


void MSBaseShader::Init(ID3D11Device * pDevice, ID3D11DeviceContext*pDeviceContext)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
	


}


void MSBaseShader::InitVertex(const std::string & fileName, const std::string & EntryPoint, const std::string & Version)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC>layout;

	SetLayout(layout);

	mVertexShader.Create(
		fileName,
		EntryPoint,
		Version,
		layout
		);

}

void MSBaseShader::InitPixel(const std::string & fileName, const std::string & EntryPointp, const std::string & Version)
{
	mPixelShader.Create(fileName,EntryPointp,Version);
}
