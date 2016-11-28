#pragma once
#include"CharacterBase.h"
#include"DXMath.hpp"
#include"StatusPlayer.h"
#include"BulletManager.h"

class Player :public CharacterBase{

public:
	Player();
	~Player();

	void Initialize(StatusField&pSetStatus)override;
	void Update()override;
	void UpdateAlive()override;
	void UpdateGravity()override;
	void UpdateCamera();

	void Render()override;
	void AddBullet();
private:
	void InitStatus()override;
private:
	DXVector3 mCameraLen;
	DXVector3 mCameraOffset;
};

