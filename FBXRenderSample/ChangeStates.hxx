#pragma once
#include<vector>
#include<memory>
/*
	様々な状態の変化の処理の関数群
*/
class BulletObject;
class CharacterBase;
class NBullet;
class BulletManager;
namespace ChangeStates {
	/*
		ダメージ処理
		魔法が使えるかどうかの処理
		魔法発射処理




	*/
	//弾、当たったキャラ
	bool Damage(NBullet* aAttackObj, CharacterBase*aDamageChara);
	//発射可能かどうか
	//発射予定キャラ、
	bool IsAttackDo(CharacterBase*lAttackChara, BulletManager*aBulletManager);
	bool BulletShot(std::vector<std::unique_ptr<NBullet>>&aBulletList, CharacterBase*lAttakcer, BulletManager*aBulletManager);

}


