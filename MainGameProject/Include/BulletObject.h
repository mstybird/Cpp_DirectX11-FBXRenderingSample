#pragma once
#include"GameObjectBase.h"
#include"DXMath.hpp"
//�e�̊��N���X
class CharacterBase;
struct StatusBulletBase;
class BulletObject :public GameObjectBase {
public:
	virtual ~BulletObject();
	//��ɒe�̃V�F�[�_�̏�����
	virtual void Initialize() = 0;
	//��{�I�ɐ��ʂ���ɒe����
	//�����p�^�[���͒e���ƂɈقȂ�A���������قȂ�Ɖ��肵�āA�x�N�^�ŊǗ����邱�Ƃ�O��ɂ���
	virtual void Create(std::vector<std::unique_ptr<BulletObject>>&aOutBulletList, CharacterBase* aShoter) = 0;
	void SetBulletMesh(MSFbxManager&aSetMesh);
	StatusBulletBase* GetStatus() {
		return mStatus.get();
	}
	template<typename T>
	T* GetStatus() {
		static_assert(std::is_base_of<StatusBulletBase*, T*>::value == false, "Type Error.");
		return static_cast<T*>(mStatus.get());
	}

	virtual void UpdateStatus();

	//�e�̍X�V
	virtual void Update() = 0;
	virtual void Render() = 0;
protected:
	//�˒�
	float mFiringRange;
	//���x
	float mVelocity;
	//�����x�N�g��
	DXVector3 mDirection;	


	//�e�̎�ނ��ƂɎ���(�Q�Ƃ̂݃R�s�[����p)
	std::unique_ptr<MSBase3DShader> mBulletShader;
	std::unique_ptr<StatusBulletBase>mStatus;
	//�e�̃��b�V��
	MSFbxManager* mBulletMesh;
	CharacterBase* mParentPtr;



};
