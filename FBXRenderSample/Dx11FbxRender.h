#pragma once

#include"DX11FbxLoader.h"
#include<D3D11.h>
class DX11FbxManager {

private:
	DX11FbxLoader*mLoader;
	ID3D11Buffer**mVertexBuffer;
	ID3D11Buffer**mIndexBuffer;
};
