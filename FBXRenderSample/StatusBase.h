#pragma once
class GameObjectBase;
struct StatusBase {

	StatusBase();

	//���ݑ����Ă���G
	GameObjectBase* mTarget;
	//�{�[�����������Ă��邩(�������Ă����炻�̃{�[���̃A�h���X)
	GameObjectBase* mBall;

	//HP

	//�G�l���M�[�c��
	int mEnergy;

};