#pragma once
#include"NBullet.h"
#include"MSFbxManager.h"
#include<unordered_map>
class BulletManager {
public:
	//マネージャの初期化
	void Initialize();

	//弾にメッシュを登録する
	void RegisterMesh(MSFbxManager*aNormalMesh);
	//発射主の現在アクティブになっている弾のIDを取得
	int GetActiveBulletID(CharacterBase* aShooter);
	//発射主の現在アクティブになっている弾を取得
	NBullet* GetActiveBullet(CharacterBase* aShooter);

	//指定したIDの弾を生成する
	void Create(std::vector<std::unique_ptr<NBullet>>&aOutBulletList,const int aBulletID, CharacterBase* aShoter);

	NBullet nBullet;

	//キャラクターごとの弾に対するステータス管理
	std::unordered_map<CharacterBase*, NBulletStatus>mStatusMap;
	std::unordered_map<int, NBullet>mBulletMap;
	std::unordered_map<CharacterBase*, int>mActiveIDMap;

};