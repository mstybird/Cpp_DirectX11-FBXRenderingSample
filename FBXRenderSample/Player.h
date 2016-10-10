#pragma once
#include"CharacterBase.h"
#include"DXVector3.h"
struct PlayerStatus {
	int mEnergy;
	PlayerStatus() :
		mEnergy{ 100 } {};
};

class Player :public CharacterBase{
public:
	Player();
	~Player();

	void Initialize()override;
	void Update()override;
	void Render()override;
	void AddBullet();
private:
	void InitStatus()override;
private:
	DXVector3 mCameraLen;
};

