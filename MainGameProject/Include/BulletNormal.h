#pragma once
#include"BulletObject.h"

//�ʏ�e
class BulletNormal :public BulletObject {
public:
	BulletNormal();
	~BulletNormal();

	void Initialize()override;
	void Create(std::vector<std::unique_ptr<BulletObject>>&aOutBulletList, CharacterBase* aShoter)override;
	//�e�̍X�V
	void Update()override;
	void Render()override;

};