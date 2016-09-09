#pragma once
#include"MSBaseShaderResource.h"
#include<string>
#include<vector>
//頂点シェーダークラス

class MSVertexShader :public MSBaseShaderResource {
public:
	MSVertexShader();
	~MSVertexShader();
	//頂点シェーダの作成
	void Create(
		const std::string & fileName,
		const std::string & EntryPoint,
		const std::string & Version,
		std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout
	);
	void SetShader();

	ID3D11VertexShader* GetShader()const;
	ID3D11InputLayout* GetLayout()const;
private:
	ID3D11VertexShader*mVertexShader;
	ID3D11InputLayout*mVertexLayout;
};
