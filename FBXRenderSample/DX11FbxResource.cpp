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

ColorChannel::~ColorChannel()
{
	TextureName.clear();
}
