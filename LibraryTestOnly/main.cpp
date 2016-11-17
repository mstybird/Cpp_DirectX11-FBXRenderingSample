#include"TGALoader.hpp"
#include"DXMath.hpp"
#include"DX11TextureManager.hpp"
#include"DX11FBXLoader.hpp"
int main() {
	DX11FbxLoader l;
	l.FbxInit("sample.txt");
	DX11TextureManager m;
	m.RegisterFile("Sample.txt", 0);
/*	Comfort::TGAImage image;
	image.ReadTGA("Sample.tga");
	ID3D11ShaderResourceView*p;
	image.CreateTextureResource2D(nullptr, p);
*/}