#pragma once
#include<memory>

//レンダーマネージャに登録するディスプレイ情報
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
