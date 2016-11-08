#pragma once
#include"BulletObject.h"

//’Êí’e
class BulletNormal :public BulletObject {
public:
	BulletNormal();
	~BulletNormal();

	void Initialize()override;
	void Create(std::vector<std::unique_ptr<BulletObject>>&aOutBulletList, CharacterBase* aShoter)override;
	//’e‚ÌXV
	void Update()override;
	void Render()override;

};