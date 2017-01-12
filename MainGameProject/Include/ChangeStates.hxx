#pragma once
#include<vector>
#include<memory>
/*
	�l�X�ȏ�Ԃ̕ω��̏����̊֐��Q
*/
class BulletObject;
class CharacterBase;
class BulletObjectBase;
class BulletManager;
namespace ChangeStates {
	/*
		�_���[�W����
		���@���g���邩�ǂ����̏���
		���@���ˏ���




	*/
	//�e�A���������L����
	bool Damage(BulletObjectBase* aAttackObj, CharacterBase*aDamageChara);
	//���ˉ\���ǂ���
	//���˗\��L�����A
	bool IsAttackDo(CharacterBase*lAttackChara, BulletManager*aBulletManager);

	//�e�̃C���^�[�o�����c���Ă��邩���ׂ�
	bool IsBulletWaiting(CharacterBase*lAttakcer, BulletManager*aBulletManager);
	//�e�̔�����������
	void FirstBulletProc(CharacterBase*aAttakcer, BulletManager*aBulletManager);

	//�e�̔���
	bool BulletShot(std::vector<std::unique_ptr<BulletObjectBase>>&aBulletList, CharacterBase*lAttakcer, BulletManager*aBulletManager);

}


