#pragma once
#include<vector>
#include<memory>
/*
	�l�X�ȏ�Ԃ̕ω��̏����̊֐��Q
*/
class BulletObject;
class CharacterBase;
namespace ChangeStates {
	/*
		�_���[�W����
		���@���g���邩�ǂ����̏���
		���@���ˏ���




	*/

	bool Damage(BulletObject* aAttackObj, CharacterBase*aDamageChara);
	bool IsAttackDo(CharacterBase*lAttackChara, BulletObject*aBullet);
	bool BulletShot(std::vector<std::unique_ptr<BulletObject>>&aBulletList, CharacterBase*lAttakcer, BulletObject* lBullet);

}


