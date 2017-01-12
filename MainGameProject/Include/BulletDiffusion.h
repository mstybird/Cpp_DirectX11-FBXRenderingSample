#pragma once
#include"NBullet.h"
class BulletDiffusion :public BulletObjectBase {
public:
	~BulletDiffusion();

	virtual void Create(std::vector<std::unique_ptr<BulletObjectBase>>&aOutBulletList, CharacterBase* aShoter)override;
	virtual void ShotFirstEffect(CharacterBase* aShoter)override;

	//ˆÚ“®ˆ—
	virtual void UpdateMove()override;

};






