#pragma once
#include<DX11FBXLoader.hpp>
#include"MSFbxManager.h"
//FBXメッシュを管理するクラス
#include"MSCollisionSphere.h"
class MSFbxObject {
public:
	//FBXマネージャを使って初期化
	void Initialize(MSFbxManager& aFbxMesh);
	//フレームの更新
	void Update();
	//フレームを指定スピードで進める
	//再生が終了した場合、trueを返す
	bool NextFrame();
	//再生速度倍率を変更する
	void SetSpeed(float aSpeed = 1.0f) {}
	//モーションを変更する
	void SetAnimation(int mIndex) {
		mCurrentAnimation = mIndex;
		mManager->mLoader->SetAnimation(mCurrentAnimation);
	}
	//現在のモーションを取得する
	int GetAnimation();
	//最初のフレームに移動する
	void SetFrontFrame();
	//ループ再生の設定
	void SetLoopFlag(bool aFlag);

	//バッファの取得
	std::vector<std::shared_ptr<FBXMesh>>* GetCurrentMeshData();
	std::vector<std::vector<ID3D11Buffer*>>*GetCurrentVertexBuffer();
	std::vector<std::vector<ID3D11Buffer*>>*GetCurrentVertexCollisionBuffer();
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
	std::vector<std::vector<ID3D11Buffer*>>* mVertexCollisionBuffer;
	std::vector<std::vector<ID3D11Buffer*>>* mIndexBuffer;
	std::vector<std::vector<uint32_t>>* mIndexBufferLength;
	std::vector<std::vector<MSCollisionSphere>>* mCollisionSphere;
};
