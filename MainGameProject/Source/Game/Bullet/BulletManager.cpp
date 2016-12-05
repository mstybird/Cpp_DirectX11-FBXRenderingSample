#include "BulletManager.h"
#include"MSFbxDatabase.h"

void BulletManager::Initialize(StatusBulletBase* aBullet1)
{
	nBullet = std::make_unique<NBullet>();
	nBullet->Initialize();
	nBullet->InitStatus(aBullet1);

	mBulletMap[BulletUniqueID::NBullet] = nBullet.get();

}

void BulletManager::InitEffect(::Comfort::EfkManager * aManager, ::Comfort::EffectDatabase * aDb, const int aShotID, const int aHitID, const int KillID)
{
	nBullet->SetEffect(aManager, aDb, aHitID, aShotID, KillID);
}

bool BulletManager::RegisterChara(CharacterBase * aChara, const int aActiveID)
{
	//登録済みか調べる
	if (mActiveIDMap.count(aChara) != 0) {
		//登録済み
		return false;
	}
	//アクティブバレットの登録
	mActiveIDMap[aChara] = aActiveID;
	//全ステータスの登録
	for (auto lBullet : mBulletMap) {
		//バレット固有のステータスでたまステータスを初期化
		auto& lStatus = mStatusMap[aChara][lBullet.first];
		lStatus = *lBullet.second->GetStatus();
		auto&b = lStatus;
	}
	return true;
}

void BulletManager::RegisterMesh(MSFbxDatabase*aModelDb, std::unordered_map<int,float>*aScaleMap,const int aDesignID, const int aCollisionID, const int aBulletID)
{
	mBulletMap[aBulletID]->SetMesh(*aModelDb->Get(aDesignID));
	mBulletMap[aBulletID]->SetMeshScale(
		aScaleMap->at(aDesignID),
		aScaleMap->at(aDesignID),
		aScaleMap->at(aDesignID)
		);


	mBulletMap[aBulletID]->SetCollisionMesh(*aModelDb->Get(aCollisionID));
	mBulletMap[aBulletID]->SetCollisionScale(
		aScaleMap->at(aCollisionID),
		aScaleMap->at(aCollisionID),
		aScaleMap->at(aCollisionID)
	);

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

std::vector<StatusBulletBase*> BulletManager::GetStatusArray(CharacterBase * aChara)
{
	std::vector<StatusBulletBase*> lStatusArray;

	lStatusArray.reserve(mStatusMap.count(aChara));
	for (auto&lStatus : mStatusMap[aChara]) {
		lStatusArray.push_back(&lStatus.second);
	}

	return std::move(lStatusArray);
}
