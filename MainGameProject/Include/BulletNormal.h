#pragma once
#include"NBullet.h"

class BulletNormal :public BulletObjectBase {
public:
	~BulletNormal();

	virtual void Create(std::vector<std::unique_ptr<BulletObjectBase>>&aOutBulletList, CharacterBase* aShoter)override;
	virtual void ShotFirstEffect(CharacterBase* aShoter)override;

//	virtual void Update()override;

	//ˆÚ“®ˆ—
	virtual void UpdateMove()override;
protected:

private:


};