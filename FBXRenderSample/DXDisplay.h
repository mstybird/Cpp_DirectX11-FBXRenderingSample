#pragma once
#include<memory>

//�����_�[�}�l�[�W���ɓo�^����f�B�X�v���C���
class DXCamera;
class DXProjection;
class DXDisplay {
public:
	void SetRenderTarget(const std::weak_ptr<DXCamera>pCamera, const std::weak_ptr<DXProjection>pProjection);
	std::weak_ptr<DXCamera> GetCamera()const;
	std::weak_ptr<DXProjection>GetProjection()const;
private:
	std::weak_ptr<DXCamera>mUseCamera;
	std::weak_ptr<DXProjection>mUseProjection;
};
