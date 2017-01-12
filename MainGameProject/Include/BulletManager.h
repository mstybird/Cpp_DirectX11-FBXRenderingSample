#pragma once
#include"NBullet.h"
#include"MSFbxManager.h"
#include"Effect.hxx"
#include<memory>
#include<unordered_map>
class BulletObjectBase;
class MSFbxManager;
class MSFbxDatabase;
namespace BulletUniqueID {
	static const int BulletNormal = 1000;
	static const int BulletDiffusion = 1001;
}

class BulletManager {
public:
	BulletManager();
	~BulletManager();

	//�}�l�[�W���̏�����
	void Initialize(StatusBulletBase* aBulletNormal, StatusBulletBase* aBulletDiffusion);
	void InitEffect(::Comfort::EfkManager* aManager, ::Comfort::EffectDatabase*aDb, const int aShotID, const int aHitID, const int KillID);
	//�L�����N�^�[��o�^����
	bool RegisterChara(CharacterBase* aChara, const int aActiveID);

	//�e�Ƀ��b�V����o�^����
	void RegisterMesh(MSFbxDatabase*aModelDb,std::unordered_map<int,float>*aScaleMap,const int aDesignID, const int aCollisionID,const int aBulletID);
	//�V�F�[�_���Z�b�g����
	void RegisterShader(MSBase3DShader*aShader, const int aID);

	//���ˎ�̌��݃A�N�e�B�u�ɂȂ��Ă���e��ID���擾
	int GetActiveBulletID(CharacterBase* aShooter);
	//���ˎ�̌��݃A�N�e�B�u�ɂȂ��Ă���e���擾
	BulletObjectBase* GetActiveBullet(CharacterBase* aShooter);
	//�w�肵��ID�̒e���A�N�e�B�u�ɂ���
	void SetActiveBulletID(CharacterBase* aShooter, const int aID);

	//�A�N�e�B�uID�����̒e��ID�ɕύX����
	void NextActiveBulletID(CharacterBase* aShooter);
	//�A�N�e�B�uID��O�̒e��ID�ɕύX����
	void BackActiveBulletID(CharacterBase* aShooter);


	//�w�肵��ID�̒e�𐶐�����
	void Create(std::vector<std::unique_ptr<BulletObjectBase>>&aOutBulletList, CharacterBase* aShoter);
	//���݃A�N�e�B�u�ɂȂ��Ă���o���b�g�̃X�e�[�^�X���擾
	StatusBulletBase* GetActiveStatus(CharacterBase*aChara);
	//�o���b�g�̃X�e�[�^�X�̃��X�g���擾
	std::vector<StatusBulletBase*>GetStatusArray(CharacterBase*aChara);

private:
	//�m�[�}���o���b�g
	std::unique_ptr<BulletObjectBase> mBulletNormal;
	//3way�o���b�g
	std::unique_ptr<BulletObjectBase> mBulletDiffusion;

	/*
		�o���b�g���}�b�v�ɓo�^������B
		�G����������e�𔭎˂ł���悤�ɁB
		�I�𒆂̒e(�A�N�e�B�u�Ȓe�𔭎˂���悤�ɂ���)
	*/

	//�L�����N�^�[���Ƃ̒e�ɑ΂���X�e�[�^�X�Ǘ�

	//�g�p�҃A�h���X,(�o���b�gID,�g�p�҂��ێ����邻�̃o���b�g�X�e�[�^�X)
	std::unordered_map<CharacterBase*, std::unordered_map<int, StatusBulletBase>>mStatusMap;
	//�o���b�g�ŗL��ID,�o���b�g�{�̂��Q�Ƃ���A�h���X
	std::unordered_map<int, BulletObjectBase*>mBulletMap;
	//�g�p�҃A�h���X,�g�p�҂̃A�N�e�B�u��Ԃɂ���o���b�gID
	std::unordered_map<CharacterBase*, int>mActiveIDMap;

	const std::vector<int>mBulletIDArray;

};