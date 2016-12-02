#pragma once
#include"NBullet.h"
#include"MSFbxManager.h"
#include<memory>
#include<unordered_map>
class NBullet;
class MSFbxManager;
class MSFbxDatabase;
namespace BulletUniqueID {
	static const int NBullet = 1000;
}

class BulletManager {
public:
	//マネージャの初期化
	void Initialize(StatusBulletBase* aBullet1);

	//キャラクターを登録する
	bool RegisterChara(CharacterBase* aChara, const int aActiveID);

	//弾にメッシュを登録する
	void RegisterMesh(MSFbxDatabase*aModelDb,std::unordered_map<int,float>*aScaleMap,const int aDesignID, const int aCollisionID,const int aBulletID);
	//シェーダをセットする
	void RegisterShader(MSBase3DShader*aShader, const int aID);

	//発射主の現在アクティブになっている弾のIDを取得
	int GetActiveBulletID(CharacterBase* aShooter);
	//発射主の現在アクティブになっている弾を取得
	NBullet* GetActiveBullet(CharacterBase* aShooter);

	//指定したIDの弾を生成する
	void Create(std::vector<std::unique_ptr<NBullet>>&aOutBulletList, CharacterBase* aShoter);
	//現在アクティブになっているバレットのステータスを取得
	StatusBulletBase* GetActiveStatus(CharacterBase*aChara);
	//バレットのステータスのリストを取得
	std::vector<StatusBulletBase*>GetStatusArray(CharacterBase*aChara);

private:
	//ノーマルバレット
	std::unique_ptr<NBullet> nBullet;

	/*
		バレットをマップに登録させる。
		敵がしっかり弾を発射できるように。
		選択中の弾(アクティブな弾を発射するようにする)
	*/

	//キャラクターごとの弾に対するステータス管理

	//使用者アドレス,(バレットID,使用者が保持するそのバレットステータス)
	std::unordered_map<CharacterBase*, std::unordered_map<int, StatusBulletBase>>mStatusMap;
	//バレット固有のID,バレット本体を参照するアドレス
	std::unordered_map<int, NBullet*>mBulletMap;
	//使用者アドレス,使用者のアクティブ状態にあるバレットID
	std::unordered_map<CharacterBase*, int>mActiveIDMap;

};