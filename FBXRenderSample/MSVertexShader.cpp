#include"MSVertexShader.h"
#include"DX11Resrouce.h"
#include<cassert>
#include<vector>
MSVertexShader::MSVertexShader() :
	mVertexShader{ nullptr },
	mVertexLayout{ nullptr }
{

}

MSVertexShader::~MSVertexShader()
{
	SAFE_RELEASE(mVertexShader);
	SAFE_RELEASE(mVertexLayout);
}

void MSVertexShader::Create(const std::string & fileName, const std::string & EntryPoint, const std::string & Version, std::vector<D3D11_INPUT_ELEMENT_DESC> &pLayout)
{
	//���_�V�F�[�_�쐬
	{
		if (FAILED(D3DX11CompileFromFile(fileName.c_str(), NULL, NULL, EntryPoint.c_str(), Version.c_str(), 0, 0, NULL, &mCompiledShader, &mErrors, NULL)))
		{
			//MessageBox(0, "hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
			assert(0);
			return;
		}
		if (FAILED(sDevice->CreateVertexShader(mCompiledShader->GetBufferPointer(), mCompiledShader->GetBufferSize(), NULL, &mVertexShader)))
		{
			SAFE_RELEASE(mCompiledShader);
			//MessageBox(0, "�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
			assert(0);
			return;
		}

	}

	//���_�C���v�b�g���C�A�E�g�쐬
	{
		if (FAILED(sDevice->CreateInputLayout(pLayout.data(), pLayout.size(), mCompiledShader->GetBufferPointer(), mCompiledShader->GetBufferSize(), &mVertexLayout)))
		{
			assert(0);
		}

	}



	SAFE_RELEASE(mErrors);
	SAFE_RELEASE(mCompiledShader);

}

ID3D11VertexShader* MSVertexShader::GetShader()const
{
	return mVertexShader;
}

ID3D11InputLayout * MSVertexShader::GetLayout() const
{
	return mVertexLayout;
}

void MSVertexShader::SetShader()
{
	sDeviceContext->VSSetShader(mVertexShader, nullptr, 0);
	sDeviceContext->IASetInputLayout(mVertexLayout);
}
