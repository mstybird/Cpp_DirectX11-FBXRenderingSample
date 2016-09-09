#pragma once
#include"MSBaseShaderResource.h"
#include<string>

//�s�N�Z���V�F�[�_�[�N���X
class MSPixelShader: public MSBaseShaderResource {
public:
	MSPixelShader();
	virtual ~MSPixelShader();
	void Create(const std::string & pFileName,
		const std::string & pEntryPoint,
		const std::string & pVersion);
	ID3D11PixelShader* GetShader()const;
	void SetShader()override;

private:
	ID3D11PixelShader*mPixelShader;
};
