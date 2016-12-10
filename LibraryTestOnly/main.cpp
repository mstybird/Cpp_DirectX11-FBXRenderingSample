#include"TGALoader.hpp"
#include"DXMath.hpp"
#include"DX11TextureManager.hpp"
#include"DX11FBXLoader.hpp"
int main() {
	D3DXVECTOR3 v{ 1,2,2 };
	D3DXVec3Normalize(&v, &v);
	float len=D3DXVec3Length(&v);
}