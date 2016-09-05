#include "DX11FbxResource.h"

FBXModelData::FBXModelData():
	Index{ nullptr },
	Emissive{ nullptr },
	Ambient{ nullptr },
	Diffuse{ nullptr },
	Specular{ nullptr }
{

}

FBXModelData::~FBXModelData()
{

}

FBXMesh::FBXMesh()
{
	mWorld = nullptr;
}

FBXMesh::~FBXMesh()
{
	if (mWorld != nullptr) {
		delete mWorld;
	}
	for (unsigned int i = 0; i<subMesh.size(); i++) {
		if (subMesh[i] != nullptr) {
			delete subMesh[i];
			subMesh[i] = nullptr;
		}
	}
	subMesh.clear();
}

ColorChannel::ColorChannel():
	mTexture{nullptr},
	Color{0.0f,0.0f,0.0f,1.0f}
{
}

ColorChannel::~ColorChannel()
{
	TextureName.clear();
	if (mTexture != nullptr) {
		delete mTexture;
	}
}

void ColorChannel::CreateTexture()
{
	if (!TextureName.empty() == true) {
		mTexture = new DXTexture;
		mTexture->Create(TextureName);
	}
}
