#pragma once
#include<memory>

//レンダーマネージャに登録するディスプレイ情報
class DXCamera;
class DXProjection;
class DXDisplay {
public:
	void SetRenderTarget(DXCamera*pCamera, DXProjection*pProjection);
	DXCamera* GetCamera()const;
	DXProjection*GetProjection()const;
private:
	//std::weak_ptr<DXCamera>mUseCameraPtr;
	//std::weak_ptr<DXCamera>mUseProjectoPtr;
	DXCamera*mUseCameraPtr;
	DXProjection*mUseProjectionPtr;
};
