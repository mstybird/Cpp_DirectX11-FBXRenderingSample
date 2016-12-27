#pragma once
#include<memory>

//�����_�[�}�l�[�W���ɓo�^����f�B�X�v���C���
class DXCamera;
class DXProjection;
class DXDisplay {
public:
	void SetRenderTarget( DXCamera* pCamera,  DXProjection* pProjection);
	DXCamera* GetCamera()const;
	DXProjection* GetProjection()const;
private:
	DXCamera* mUseCamera;
	DXProjection* mUseProjection;
};
