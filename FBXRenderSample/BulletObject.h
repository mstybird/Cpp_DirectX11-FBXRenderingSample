#pragma once
#include"GameObjectBase.h"
#include"DXVector3.h"
//�e�̊��N���X
class BulletObject :public GameObjectBase {
public:
	virtual ~BulletObject();
	//��ɒe�̃V�F�[�_�̏�����
	virtual void Initialize() = 0;
	//��{�I�ɐ��ʂ���ɒe����
	//�����p�^�[���͒e���ƂɈقȂ�A���������قȂ�Ɖ��肵�āA�x�N�^�ŊǗ����邱�Ƃ�O��ɂ���
	virtual void Create(std::vector<std::unique_ptr<BulletObject>>&aOutBulletList, GameObjectBase& aShoter) = 0;
	void SetBulletMesh(MSFbxManager&aSetMesh);
	//�e�̍X�V
	virtual void Update() = 0;
	virtual void Render() = 0;
protected:
	float mFiringRange;	//�˒�
	float mVelocity;	//���x
	DXVector3 mDirection;	//�����x�N�g��
							//�e�̎�ނ��ƂɎ���(�Q�Ƃ̂݃R�s�[����p)
	std::unique_ptr<MSBase3DShader> mBulletShader;
	MSFbxManager* mBulletMesh;
};
