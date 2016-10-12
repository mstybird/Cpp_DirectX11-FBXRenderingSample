#include "StatusField.h"
#include"DXWorld.h"
#include"DXMatrix.h"
StatusField::StatusField():
	mBallHoldChara{nullptr},
	mBallIsField{false},
	mBall{nullptr}
{
}

void StatusField::SetBallHolder(GameObjectBase * pBallHolder)
{
	mBallHoldChara = pBallHolder;
	mBallIsField = false;
	mBall->SetActive(false);
}

void StatusField::RespawnBall()
{
	mBall->SetActive(true);
	mBall->GetWorld()->SetT(-10, 0, -8);
	mBallIsField = true;
	mBallHoldChara = nullptr;
}
