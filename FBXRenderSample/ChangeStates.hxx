#pragma once
#include<vector>
#include<memory>
/*
	様々な状態の変化の処理の関数群
*/
class BulletObject;
class CharacterBase;
class NBullet;
namespace ChangeStates {
	/*
		ダメージ処理
		魔法が使えるかどうかの処理
		魔法発射処理




	*/

	bool Damage(NBullet* aAttackObj, CharacterBase*aDamageChara);
	bool IsAttackDo(CharacterBase*lAttackChara, NBullet*aBullet);
	bool BulletShot(std::vector<std::unique_ptr<NBullet>>&aBulletList, CharacterBase*lAttakcer, NBullet* lBullet);

}


