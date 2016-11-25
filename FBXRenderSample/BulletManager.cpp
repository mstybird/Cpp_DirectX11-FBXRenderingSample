#include "BulletManager.h"

void BulletManager::Initialize()
{
	nBullet.Initialize();
	nBullet.InitShader();
}

void BulletManager::RegisterMesh(MSFbxManager * aNormalMesh)
{
	nBullet.SetMesh(*aNormalMesh);
	nBullet.SetCollisionMesh(*aNormalMesh);
}

int BulletManager::GetActiveBulletID(CharacterBase * aShooter)
{
	return mActiveIDMap[aShooter];
}

NBullet * BulletManager::GetActiveBullet(CharacterBase * aShooter)
{
	return &mBulletMap[mActiveIDMap[aShooter]];
}

void BulletManager::Create(std::vector<std::unique_ptr<NBullet>>& aOutBulletList, const int aBulletID, CharacterBase * aShoter)
{
	switch (aBulletID)
	{
	case 0:
		nBullet.Create(aOutBulletList, aShoter);
		break;
	default:
		break;
	}
	return;
}
