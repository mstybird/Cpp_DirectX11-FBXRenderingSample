#pragma once
#include"DXCamera.h"
#include"DXProjection.h"

//���ۂ̃����_�����O�Ɏg���r���[�s��Ǝˉe�s��͈�ł���
class DXDisplay {
public:
	void SetRenderTarget();
	DXCamera*mUseCameraPtr;
	DXProjection*mUseProjectionPtr;
};
