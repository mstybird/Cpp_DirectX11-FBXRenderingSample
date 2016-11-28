#pragma once
#include"NBullet.h"
#include"MSFbxManager.h"
#include<memory>
#include<unordered_map>
class NBullet;
class MSFbxManager;

namespace BulletUniqueID {
	static const int NBullet = 0;
}

class BulletManager {
public:
	//�}�l�[�W���̏�����
	void Initialize();

	//�L�����N�^�[��o�^����
	bool RegisterChara(CharacterBase* aChara, const int aActiveID);

	//�e�Ƀ��b�V����o�^����
	void RegisterMesh(MSFbxManager*aDesignMesh, MSFbxManager*aCollisionMesh,const int aID);
	//�V�F�[�_���Z�b�g����
	void RegisterShader(MSBase3DShader*aShader, const int aID);

	//���ˎ�̌��݃A�N�e�B�u�ɂȂ��Ă���e��ID���擾
	int GetActiveBulletID(CharacterBase* aShooter);
	//���ˎ�̌��݃A�N�e�B�u�ɂȂ��Ă���e���擾
	NBullet* GetActiveBullet(CharacterBase* aShooter);

	//�w�肵��ID�̒e�𐶐�����
	void Create(std::vector<std::unique_ptr<NBullet>>&aOutBulletList, CharacterBase* aShoter);
	//���݃A�N�e�B�u�ɂȂ��Ă���o���b�g�̃X�e�[�^�X���擾
	StatusBulletBase* GetActiveStatus(CharacterBase*aChara);
	//�o���b�g�̃X�e�[�^�X�̃��X�g���擾
	std::vector<StatusBulletBase*>GetStatusArray(CharacterBase*aChara);

private:
	//�m�[�}���o���b�g
	std::unique_ptr<NBullet> nBullet;

	/*
		�o���b�g���}�b�v�ɓo�^������B
		�G����������e�𔭎˂ł���悤�ɁB
		�I�𒆂̒e(�A�N�e�B�u�Ȓe�𔭎˂���悤�ɂ���)
	*/

	//�L�����N�^�[���Ƃ̒e�ɑ΂���X�e�[�^�X�Ǘ�

	//�g�p�҃A�h���X,(�o���b�gID,�g�p�҂��ێ����邻�̃o���b�g�X�e�[�^�X)
	std::unordered_map<CharacterBase*, std::unordered_map<int, StatusBulletBase>>mStatusMap;
	//�o���b�g�ŗL��ID,�o���b�g�{�̂��Q�Ƃ���A�h���X
	std::unordered_map<int, NBullet*>mBulletMap;
	//�g�p�҃A�h���X,�g�p�҂̃A�N�e�B�u��Ԃɂ���o���b�gID
	std::unordered_map<CharacterBase*, int>mActiveIDMap;

};