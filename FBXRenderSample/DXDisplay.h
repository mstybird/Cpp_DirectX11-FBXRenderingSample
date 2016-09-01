#pragma once
#include"DXCamera.h"
#include"DXProjection.h"

//実際のレンダリングに使うビュー行列と射影行列は一つでいい
class DXDisplay {
public:
	void SetRenderTarget();
	DXCamera*mUseCameraPtr;
	DXProjection*mUseProjectionPtr;
};
