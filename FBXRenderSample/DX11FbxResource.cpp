#include "DX11FbxResource.h"
#include"DX11Texture.h"
#include"DX11TextureManager.h"
#include"FbxMaterialCache.h"
FBXModelData::FBXModelData():
	Index{ nullptr }
{

}

FBXModelData::~FBXModelData()
{
	//delete Index;
}

void FBXModelData::GetPolygon(std::array<FbxVertex*, 3>&pOutPolygon,unsigned int pPolygonIndex)
{

	pOutPolygon[0] = &Data[pPolygonIndex * 3 + 0];
	pOutPolygon[1] = &Data[pPolygonIndex * 3 + 1];
	pOutPolygon[2] = &Data[pPolygonIndex * 3 + 2];
}

FBXMesh::FBXMesh()
{
	mWorld = nullptr;
}

FBXMesh::~FBXMesh()
{
	subMesh.clear();
}

std::unique_ptr<DX11TextureManager>ColorChannel::sTextureManager{ std::make_unique<DX11TextureManager>() };
int ColorChannel::sTextureCounter{ 0 };

ColorChannel::ColorChannel() :
	Color{ 0.0f,0.0f,0.0f,1.0f },
	mTextureID{ -1 }
{
}

ColorChannel::~ColorChannel()
{
	TextureName.clear();
	if (mTextureID != -1) {
		sTextureManager->UnRegisterFile(mTextureID);
	}
}

void ColorChannel::CreateTexture()
{
	if (!TextureName.empty() == true) {
		if (sTextureManager->RegisterFile(TextureName, sTextureCounter)==true) {
			//テクスチャIDを記憶
			mTextureID = sTextureCounter;
			//テクスチャカウンタが重複しないようにインクリメント
			sTextureCounter++;
			//テクスチャを設定
			sTextureManager->Load(mTexture, mTextureID);
		}
	}
}
