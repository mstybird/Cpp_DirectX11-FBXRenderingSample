#pragma once
#include<vector>
#include<memory>
/*
	�l�X�ȏ�Ԃ̕ω��̏����̊֐��Q
*/
class BulletObject;
class CharacterBase;
class NBullet;
namespace ChangeStates {
	/*
		�_���[�W����
		���@���g���邩�ǂ����̏���
		���@���ˏ���




	*/

	bool Damage(NBullet* aAttackObj, CharacterBase*aDamageChara);
	bool IsAttackDo(CharacterBase*lAttackChara, NBullet*aBullet);
	bool BulletShot(std::vector<std::unique_ptr<NBullet>>&aBulletList, CharacterBase*lAttakcer, NBullet* lBullet);

}


