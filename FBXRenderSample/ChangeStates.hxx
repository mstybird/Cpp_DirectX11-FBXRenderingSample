#pragma once
#include<vector>
#include<memory>
/*
	�l�X�ȏ�Ԃ̕ω��̏����̊֐��Q
*/
class BulletObject;
class CharacterBase;
class NBullet;
class BulletManager;
namespace ChangeStates {
	/*
		�_���[�W����
		���@���g���邩�ǂ����̏���
		���@���ˏ���




	*/
	//�e�A���������L����
	bool Damage(NBullet* aAttackObj, CharacterBase*aDamageChara);
	//���ˉ\���ǂ���
	//���˗\��L�����A
	bool IsAttackDo(CharacterBase*lAttackChara, BulletManager*aBulletManager);
	bool BulletShot(std::vector<std::unique_ptr<NBullet>>&aBulletList, CharacterBase*lAttakcer, BulletManager*aBulletManager);

}


