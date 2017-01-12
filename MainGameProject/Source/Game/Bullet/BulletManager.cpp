#include "BulletManager.h"
#include"MSFbxDatabase.h"
#include<BulletNormal.h>
#include<BulletDiffusion.h>
#include<cassert>
BulletManager::BulletManager():
	mBulletIDArray{ BulletUniqueID::BulletNormal, BulletUniqueID::BulletDiffusion }
{

}
BulletManager::~BulletManager()
{

}
void BulletManager::Initialize(StatusBulletBase* aBulletNormal, StatusBulletBase* aBulletDiffusion)
{
	//通常弾
	mBulletNormal = std::make_unique<BulletNormal>();
	mBulletNormal->Initialize();
	mBulletNormal->InitStatus(aBulletNormal);
	mBulletMap[BulletUniqueID::BulletNormal] = mBulletNormal.get();
	//3way弾
	mBulletDiffusion = std::make_unique<BulletDiffusion>();
	mBulletDiffusion->Initialize();
	mBulletDiffusion->InitStatus(aBulletNormal);
	mBulletMap[BulletUniqueID::BulletDiffusion] = mBulletDiffusion.get();

}

void BulletManager::InitEffect(::Comfort::EfkManager * aManager, ::Comfort::EffectDatabase * aDb, const int aShotID, const int aHitID, const int KillID)
{
	mBulletNormal->SetEffect(aManager, aDb, aHitID, aShotID, KillID);
	mBulletDiffusion->SetEffect(aManager, aDb, aHitID, aShotID, KillID);
}

bool BulletManager::RegisterChara(CharacterBase * aChara, const int aActiveID)
{
	//登録済みか調べる
	if (mActiveIDMap.count(aChara) != 0) {
		//登録済み
		return false;
	}
	//アクティブバレットの登録
	SetActiveBulletID(aChara, aActiveID);
	//全ステータスの登録
	for (auto lBullet : mBulletMap) {
		//バレット固有のステータスで弾ステータスを初期化
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

BulletObjectBase * BulletManager::GetActiveBullet(CharacterBase * aShooter)
{
	return mBulletMap[mActiveIDMap[aShooter]];
}

void BulletManager::SetActiveBulletID(CharacterBase * aShooter, const int aID)
{
	//不正な値が入力された場合はデバッグ時に弾く
	auto v = std::find(mBulletIDArray.begin(), mBulletIDArray.end(), aID) != mBulletIDArray.end();
	assert(v);
	mActiveIDMap[aShooter] = aID;

	return;
}

void BulletManager::NextActiveBulletID(CharacterBase * aShooter)
{
	if (mActiveIDMap[aShooter] == mBulletIDArray.back()) {
		mActiveIDMap[aShooter] = mBulletIDArray.front();
	}
	else {
		auto index = std::find(mBulletIDArray.begin(), mBulletIDArray.end(), mActiveIDMap[aShooter]);
		mActiveIDMap[aShooter] = *(++index);
	}
}

void BulletManager::BackActiveBulletID(CharacterBase * aShooter)
{
	if (mActiveIDMap[aShooter] == mBulletIDArray.front()) {
		mActiveIDMap[aShooter] = mBulletIDArray.back();
	}
	else {
		auto index = std::find(mBulletIDArray.begin(), mBulletIDArray.end(), mActiveIDMap[aShooter]);
		mActiveIDMap[aShooter] = *(--index);
	}
}

void BulletManager::Create(std::vector<std::unique_ptr<BulletObjectBase>>& aOutBulletList, CharacterBase * aShoter)
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
