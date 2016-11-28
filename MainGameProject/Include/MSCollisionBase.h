#pragma once
#include<memory>
//è’ìÀîªíËä÷åW
class DXVector3;
class MSFbxManager;
struct FBXModelData;
class DXMatrix;
class DXWorld;
class DXCamera;
class DX11RenderResource;

class MSCollisionBase {
public:
	MSCollisionBase();
	MSCollisionBase(const MSCollisionBase& _co);
	~MSCollisionBase();
protected:
	std::unique_ptr<DXMatrix>mGlobalPosition;
};



