#pragma once
#include"DX11FbxLoader.h"
#include"MSFbxManager.h"
//FBX���b�V�����Ǘ�����N���X
class MSCollisionSphere;
class MSFbxObject {
public:
	//FBX�}�l�[�W�����g���ď�����
	void Initialize(MSFbxManager& aFbxMesh);
	//�t���[���̍X�V
	void Update();
	//�t���[�����w��X�s�[�h�Ői�߂�
	void NextFrame();
	//�Đ����x�{����ύX����
	void SetSpeed(float aSpeed = 1.0f) {}
	//���[�V������ύX����
	void SetAnimation(int mIndex) {}
	//�o�b�t�@�̎擾
	std::vector<std::shared_ptr<FBXMesh>>& GetMeshData();
	ID3D11Buffer*GetVertexBuffer(int i, int j) {}
	ID3D11Buffer*GetIndexBuffer(int i, int j) {}
	unsigned int*GetIndexBufferCount(int i, int j);

	//�R���W�����̍쐬
	void CreateCollision();

	MSFbxManager*mManager;

	int mAnimationCount;
	int mCurrentAnimation;
	FbxTime mStartFrame;
	FbxTime mStopFrame;
	FbxTime mFrameTime;
	float mFrameSpeed;
	FbxTime mCurrentFrame;

	std::vector<std::shared_ptr<FBXMesh>> mMeshData;
	std::vector<std::vector<ID3D11Buffer*>>mVertexBuffer;
	std::vector<std::vector<ID3D11Buffer*>>mIndexBuffer;
	std::vector<std::vector<MSCollisionSphere>>mCollisionSphere;
};
