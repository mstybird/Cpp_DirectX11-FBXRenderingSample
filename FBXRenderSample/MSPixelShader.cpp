#include"MSPixelShader.h"
#include"DX11Resrouce.h"


MSPixelShader::MSPixelShader() :
	MSBaseShaderResource(),
	mPixelShader{ nullptr }
{
}

MSPixelShader::~MSPixelShader()
{
	SAFE_RELEASE(mPixelShader);

}

void MSPixelShader::Create(const std::string & pFileName, const std::string & pEntryPoint, const std::string & pVersion)
{
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile(pFileName.c_str(), NULL, NULL, pEntryPoint.c_str(), pVersion.c_str(), 0, 0, NULL, &mCompiledShader, &mErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		return;
	}
	SAFE_RELEASE(mErrors);

	if (FAILED(sDevice->CreatePixelShader(mCompiledShader->GetBufferPointer(), mCompiledShader->GetBufferSize(), NULL, &mPixelShader)))
	{
		SAFE_RELEASE(mCompiledShader);
		MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
		return;
	}
	//	mPixelShader.reset(lPixelShader);
	SAFE_RELEASE(mCompiledShader);
}

ID3D11PixelShader * MSPixelShader::GetShader() const
{
	return mPixelShader;
}

void MSPixelShader::SetShader()
{
	sDeviceContext->PSSetShader(mPixelShader, nullptr, 0);
}
