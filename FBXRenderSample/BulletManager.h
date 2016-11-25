#pragma once
#include"NBullet.h"
#include"MSFbxManager.h"
#include<unordered_map>
class BulletManager {
public:
	//�}�l�[�W���̏�����
	void Initialize();

	//�e�Ƀ��b�V����o�^����
	void RegisterMesh(MSFbxManager*aNormalMesh);
	//���ˎ�̌��݃A�N�e�B�u�ɂȂ��Ă���e��ID���擾
	int GetActiveBulletID(CharacterBase* aShooter);
	//���ˎ�̌��݃A�N�e�B�u�ɂȂ��Ă���e���擾
	NBullet* GetActiveBullet(CharacterBase* aShooter);

	//�w�肵��ID�̒e�𐶐�����
	void Create(std::vector<std::unique_ptr<NBullet>>&aOutBulletList,const int aBulletID, CharacterBase* aShoter);

	NBullet nBullet;

	//�L�����N�^�[���Ƃ̒e�ɑ΂���X�e�[�^�X�Ǘ�
	std::unordered_map<CharacterBase*, NBulletStatus>mStatusMap;
	std::unordered_map<int, NBullet>mBulletMap;
	std::unordered_map<CharacterBase*, int>mActiveIDMap;

};