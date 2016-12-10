#pragma once
#include<DX11FBXLoader.hpp>
#include"MSFbxManager.h"
//FBX���b�V�����Ǘ�����N���X
#include"MSCollisionSphere.h"
class MSFbxObject {
public:
	//FBX�}�l�[�W�����g���ď�����
	void Initialize(MSFbxManager& aFbxMesh);
	//�t���[���̍X�V
	void Update();
	//�t���[�����w��X�s�[�h�Ői�߂�
	bool NextFrame();
	//�Đ����x�{����ύX����
	void SetSpeed(float aSpeed = 1.0f) {}
	//���[�V������ύX����
	void SetAnimation(int mIndex) {
		mCurrentAnimation = mIndex;
		mManager->mLoader->SetAnimation(mCurrentAnimation);
	}
	//�ŏ��̃t���[���Ɉړ�����
	void SetFrontFrame();
	//���[�v�Đ��̐ݒ�
	void SetLoopFlag(bool aFlag);

	//�o�b�t�@�̎擾
	std::vector<std::shared_ptr<FBXMesh>>* GetCurrentMeshData();
	std::vector<std::vector<ID3D11Buffer*>>*GetCurrentVertexBuffer();
	std::vector<std::vector<ID3D11Buffer*>>*GetCurrentIndexBuffer();
	std::vector<std::vector<uint32_t>>*GetCurrentIndexBufferCount();
	std::vector<std::vector<MSCollisionSphere>>*GetCurrentCollisionSpheres();
	MSFbxManager*GetManager();

private:
	MSFbxManager*mManager;

	int mAnimationCount;
	int mCurrentAnimation = -1;
	FbxTime mStartFrame;
	FbxTime mStopFrame;
	FbxTime mFrameTime;
	float mFrameSpeed;
	FbxTime mCurrentFrame;
	bool mFirstInit{ false };
	bool mIsLooping{ false };

	std::vector<std::shared_ptr<FBXMesh>>* mMeshData;
	std::vector<std::vector<ID3D11Buffer*>>* mVertexBuffer;
	std::vector<std::vector<ID3D11Buffer*>>* mIndexBuffer;
	std::vector<std::vector<uint32_t>>* mIndexBufferLength;
	std::vector<std::vector<MSCollisionSphere>>* mCollisionSphere;
};
