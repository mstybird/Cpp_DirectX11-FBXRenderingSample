#include "BulletManager.h"

void BulletManager::Initialize()
{
	nBullet = std::make_unique<NBullet>();
	nBullet->Initialize();
	nBullet->InitStatus();

	mBulletMap[BulletUniqueID::NBullet] = nBullet.get();

}

bool BulletManager::RegisterChara(CharacterBase * aChara, const int aActiveID)
{
	//�o�^�ς݂����ׂ�
	if (mActiveIDMap.count(aChara) != 0) {
		//�o�^�ς�
		return false;
	}
	//�A�N�e�B�u�o���b�g�̓o�^
	mActiveIDMap[aChara] = aActiveID;
	//�S�X�e�[�^�X�̓o�^
	for (auto lBullet : mBulletMap) {
		//�o���b�g�ŗL�̃X�e�[�^�X�ł��܃X�e�[�^�X��������
		auto& lStatus = mStatusMap[aChara][lBullet.first];
		lStatus = *lBullet.second->GetStatus();
		auto&b = lStatus;
	}
	return true;
}

void BulletManager::RegisterMesh(MSFbxManager * aMesh, const int aID)
{
	mBulletMap[aID]->SetMesh(*aMesh);
	mBulletMap[aID]->SetCollisionMesh(*aMesh);
}

void BulletManager::RegisterShader(MSBase3DShader * aShader, const int aID)
{
	mBulletMap[aID]->SetShader(aShader);
}

int BulletManager::GetActiveBulletID(CharacterBase * aShooter)
{
	return mActiveIDMap[aShooter];
}

NBullet * BulletManager::GetActiveBullet(CharacterBase * aShooter)
{
	return mBulletMap[mActiveIDMap[aShooter]];
}

void BulletManager::Create(std::vector<std::unique_ptr<NBullet>>& aOutBulletList, CharacterBase * aShoter)
{
	mBulletMap[mActiveIDMap[aShoter]]->Create(aOutBulletList, aShoter);
	return;
}

StatusBulletBase * BulletManager::GetActiveStatus(CharacterBase * aChara)
{
	return &mStatusMap[aChara][mActiveIDMap[aChara]];
}
