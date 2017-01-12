#pragma once
#include<vector>
#include<memory>
/*
	様々な状態の変化の処理の関数群
*/
class BulletObject;
class CharacterBase;
class BulletObjectBase;
class BulletManager;
namespace ChangeStates {
	/*
		ダメージ処理
		魔法が使えるかどうかの処理
		魔法発射処理




	*/
	//弾、当たったキャラ
	bool Damage(BulletObjectBase* aAttackObj, CharacterBase*aDamageChara);
	//発射可能かどうか
	//発射予定キャラ、
	bool IsAttackDo(CharacterBase*lAttackChara, BulletManager*aBulletManager);

	//弾のインターバルが残っているか調べる
	bool IsBulletWaiting(CharacterBase*lAttakcer, BulletManager*aBulletManager);
	//弾の発動準備処理
	void FirstBulletProc(CharacterBase*aAttakcer, BulletManager*aBulletManager);

	//弾の発射
	bool BulletShot(std::vector<std::unique_ptr<BulletObjectBase>>&aBulletList, CharacterBase*lAttakcer, BulletManager*aBulletManager);

}


