#pragma once
#include"CharacterBase.h"
#include"DXVector3.h"
#include"StatusPlayer.h"


class Player :public CharacterBase{

public:
	Player();
	~Player();

	void Initialize()override;
	void Update()override;
	void Render()override;
	void AddBullet();
	StatusPlayer*GetStatus();
private:
	void InitStatus()override;
private:
	DXVector3 mCameraLen;
	StatusPlayer mStatus;
};

