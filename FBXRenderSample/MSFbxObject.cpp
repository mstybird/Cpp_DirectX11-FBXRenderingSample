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
	//初回のメッシュ生成
	Update();
}

void MSFbxObject::Update()
{
	//現在のアニメーションで更新

	//メッシュがなければ取得できないので生成する
	if (mManager->IsCreatedMeshData()==false) {
		if (mManager->Update(mCurrentFrame,mCurrentAnimation) == false) {
			return;
		}
	}
	//生成したメッシュデータを取得

	for (unsigned int i = 0; i < mVertexBuffer.size(); i++) {
		for (unsigned int j = 0; j < mVertexBuffer.at(i).size(); j++) {
			SAFE_RELEASE(mVertexBuffer[i][j]);
			SAFE_RELEASE(mIndexBuffer[i][j]);
		}
	}

	mMeshData = mManager->MoveMeshData();
	mVertexBuffer = mManager->MoveVertexBuffer();
	mIndexBuffer = mManager->MoveIndexBuffer();
	CreateCollision();
}

void MSFbxObject::NextFrame()
{
	mStartFrame = mManager->mLoader->Start;
	mStopFrame = mManager->mLoader->Stop;
	mCurrentFrame += (mFrameTime*mFrameSpeed);
	
	if (mCurrentFrame >= mStopFrame) {
		mCurrentFrame = mStartFrame;
	}
}

std::vector<std::shared_ptr<FBXMesh>>& MSFbxObject::GetMeshData()
{
	return mMeshData;
	// TODO: return ステートメントをここに挿入します
}

unsigned int * MSFbxObject::GetIndexBufferCount(int i, int j)
{
	return &mMeshData.at(i)->subMesh.at(j)->IndexLength;
}

void MSFbxObject::CreateCollision()
{
	if (mCollisionSphere.size() != 0) {
		return;
	}
	mCollisionSphere.clear();
	mCollisionSphere.resize(mMeshData.size());
	mCollisionSphere.resize(mMeshData.size());
	for (unsigned int i = 0; i < mMeshData.size(); i++) {
		mCollisionSphere.at(i).resize(mCollisionSphere.size());
		for (unsigned int j = 0; j < mMeshData.at(i)->subMesh.size(); j++) {
			mCollisionSphere.at(i).at(j).CreateCollision(
				*mMeshData.at(i)->subMesh.at(j),
				*mMeshData.at(i)->mWorld
				);
		}
	}
}
