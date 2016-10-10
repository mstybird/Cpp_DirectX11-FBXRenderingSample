#pragma once
#include"GameObjectBase.h"
#include"DXVector3.h"
//弾の基底クラス
class BulletObject :public GameObjectBase {
public:
	virtual ~BulletObject();
	//主に弾のシェーダの初期化
	virtual void Initialize() = 0;
	//基本的に正面を基準に弾生成
	//生成パターンは弾ごとに異なり、正整数も異なると仮定して、ベクタで管理することを前提にする
	virtual void Create(std::vector<std::unique_ptr<BulletObject>>&aOutBulletList, GameObjectBase& aShoter) = 0;
	void SetBulletMesh(MSFbxManager&aSetMesh);
	//弾の更新
	virtual void Update() = 0;
	virtual void Render() = 0;
protected:
	float mFiringRange;	//射程
	float mVelocity;	//速度
	DXVector3 mDirection;	//方向ベクトル
							//弾の種類ごとに持つ(参照のみコピーする用)
	std::unique_ptr<MSBase3DShader> mBulletShader;
	MSFbxManager* mBulletMesh;
};
