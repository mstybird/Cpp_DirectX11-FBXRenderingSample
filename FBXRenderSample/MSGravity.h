#pragma once
#include<memory>
//�d�͊Ǘ��N���X
class MSProgress;
class MSGravity {
public:
	MSGravity();
	~MSGravity();

	void Initialize(
		bool aIsGravity = true,
		float aMaxGravity = 0,
		float aMinGravity = 0,
		float aNowGravity = 0,
		float aRepulsion = 0,
		float aAttraction = 0);

	//������Ɉړ�(�W�����v)����
	void UpdateUpGravity();
	//�������Ɉړ�(�@����)����
	void UpdateDownGravity();
	//�d�͏����X�V
	void UpdateGravity();

	//���݂̏d�͂��擾
	float GetGravity();

private:
	bool mIsGravity;	//�d�͂��L�����ǂ���
	std::unique_ptr<MSProgress>mGravity;
	float mFrameAttraction;	//�d��
	float mRepulsion;	//�˗�
	float mAttraction;	//����

};