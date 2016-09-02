#include "DX11FbxResource.h"

FBXModelData::FBXModelData():
	GlobalPosition{nullptr},
	Index{ nullptr },
	Emissive{ nullptr },
	Ambient{ nullptr },
	Diffuse{ nullptr },
	Specular{ nullptr }
{

}

FBXModelData::~FBXModelData()
{
	if (GlobalPosition != nullptr) {
		delete GlobalPosition;
	}
}

FBXMesh::~FBXMesh()
{
	for (unsigned int i = 0; subMesh.size(); i++) {
		if (subMesh[i] != nullptr) {
			delete subMesh[i];
			subMesh[i] = nullptr;
		}
	}
	subMesh.clear();
}
