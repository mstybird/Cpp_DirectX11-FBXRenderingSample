#pragma once
#include"CharacterBase.h"
#include"DXVector3.h"
#include"StatusPlayer.h"


class Player :public CharacterBase{

public:
	Player();
	~Player();

	void Initialize(StatusField&pSetStatus)override;
	void Update()override;
	void UpdateAlive()override;
	void UpdateGravity()override;

	void Render()override;
	void AddBullet();
private:
	void InitStatus()override;
private:
	DXVector3 mCameraLen;
};

