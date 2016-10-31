#pragma once
#include"DX11FbxLoader.h"
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
	void NextFrame();
	//再生速度倍率を変更する
	void SetSpeed(float aSpeed = 1.0f) {}
	//モーションを変更する
	void SetAnimation(int mIndex) { mCurrentAnimation = mIndex; }
	//バッファの取得
	std::vector<std::shared_ptr<FBXMesh>>& GetMeshData();
	ID3D11Buffer*GetVertexBuffer(int i, int j) {}
	ID3D11Buffer*GetIndexBuffer(int i, int j) {}
	unsigned int*GetIndexBufferCount(int i, int j);

	//コリジョンの作成
	void CreateCollision();

	MSFbxManager*mManager;

	int mAnimationCount;
	int mCurrentAnimation = -1;
	FbxTime mStartFrame;
	FbxTime mStopFrame;
	FbxTime mFrameTime;
	float mFrameSpeed;
	FbxTime mCurrentFrame;
	bool mFirstInit{ false };

	std::vector<std::shared_ptr<FBXMesh>> mMeshData;
	std::vector<std::vector<ID3D11Buffer*>>mVertexBuffer;
	std::vector<std::vector<ID3D11Buffer*>>mIndexBuffer;
	std::vector<std::vector<MSCollisionSphere>>mCollisionSphere;
};
