#include "MSFbxObject.h"
#include"MSCollisionSphere.h"
#include"DXMatrix.h"
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

void MSFbxObject::Initialize(MSFbxManager & aFbxMesh)
{
	mManager = &aFbxMesh;
	mAnimationCount= aFbxMesh.mLoader->AnimStackNameArray.Size();
	mStartFrame = aFbxMesh.mLoader->Start;
	mStopFrame = aFbxMesh.mLoader->Stop;
	mCurrentFrame = mStartFrame;
	mFrameTime = aFbxMesh.mLoader->FrameTime;
	mFrameSpeed = 1.0f;
	//����̃��b�V������
	Update();
}

void MSFbxObject::Update()
{
	//���݂̃A�j���[�V�����ōX�V
	mManager->Update(mCurrentFrame, mCurrentAnimation);	//�����������b�V���f�[�^���擾

	mMeshData = mManager->GetMeshData(mCurrentAnimation, mCurrentFrame);
	mVertexBuffer = mManager->GetVertexBuffer(mCurrentAnimation, mCurrentFrame);
	mIndexBuffer = mManager->GetIndexBuffer(mCurrentAnimation, mCurrentFrame);
	mIndexBufferLength = mManager->GetIndexBufferCount(mCurrentAnimation, mCurrentFrame);
	mCollisionSphere = mManager->GetCollisionSphere(mCurrentAnimation, mCurrentFrame);
}

bool MSFbxObject::NextFrame()
{
	
	mStartFrame = mManager->mLoader->Start;
	mStopFrame = mManager->mLoader->Stop;
	mCurrentFrame += (mFrameTime*mFrameSpeed);
	bool lResult{ false };
	if (mCurrentFrame >= mStopFrame) {
		if (mIsLooping == true) {
			mCurrentFrame = mStartFrame;
		}
		else {
			mCurrentFrame = mStopFrame;
			lResult = true;
		}
	}
	return lResult;
}

int MSFbxObject::GetAnimation()
{
	return mCurrentAnimation;
}


void MSFbxObject::SetFrontFrame()
{
	mCurrentFrame = mStartFrame;
}

void MSFbxObject::SetLoopFlag(bool aFlag)
{
	mIsLooping = aFlag;
}

std::vector<std::shared_ptr<FBXMesh>>* MSFbxObject::GetCurrentMeshData()
{
	return mMeshData;
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
}

std::vector<std::vector<ID3D11Buffer*>> * MSFbxObject::GetCurrentVertexBuffer()
{
	return mVertexBuffer;
}

std::vector<std::vector<ID3D11Buffer*>> * MSFbxObject::GetCurrentIndexBuffer()
{
	return mIndexBuffer;
}

std::vector<std::vector<uint32_t>> * MSFbxObject::GetCurrentIndexBufferCount()
{
	return mIndexBufferLength;
}

std::vector<std::vector<MSCollisionSphere>>* MSFbxObject::GetCurrentCollisionSpheres()
{
	return mCollisionSphere;
}

MSFbxManager * MSFbxObject::GetManager()
{
	return mManager;
}

//void MSFbxObject::CreateCollision()
//{
//
//	if (mCollisionSphere.size() != 0) {
//		return;
//	}
//	mCollisionSphere.clear();
//	mCollisionSphere.resize(mMeshData.size());
//	mCollisionSphere.resize(mMeshData.size());
//	for (unsigned int i = 0; i < mMeshData.size(); i++) {
//		mCollisionSphere.at(i).resize(mCollisionSphere.size());
//		for (unsigned int j = 0; j < mMeshData.at(i)->subMesh.size(); j++) {
//			mCollisionSphere.at(i).at(j).CreateCollision(
//				*mMeshData.at(i)->subMesh.at(j),
//				*mMeshData.at(i)->mWorld
//				);
//		}
//	}
//}
