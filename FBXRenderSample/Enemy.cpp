#include "Enemy.h"
#include"NcgLua\NcgLua.hpp"
#include"AI\EnemyAI.h"
#include"DX11RenderResource.h"
#include"MSCollisionRayPicking.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"MSUtility.h"
#include"MSCullingOcclusion.h"
#include"MSCullingFrustum.h"
#include"BulletNormal.h"
#include"StatusField.h"
#include"DXMatrix.h"
#include"Ball.h"
Enemy::Enemy()
{
	mBulletNormal = std::make_unique<BulletNormal>();
}
Enemy::~Enemy()
{
}
void Enemy::Initialize()
{
	CharacterBase::Initialize();

	mAI = std::make_unique<EnemyAI>();
	mAI->CreateNodes();
	

	//ステータスの初期化
	InitStatus();
}
void Enemy::InitFinal()
{
	
	mRayPick->SetFramePosition(*mTransform);
	//一番近いノードを初期座標としておく
	DXVector3 lPosition;
	GetWorld()->GetMatrix().lock()->GetT(lPosition);
	auto lNearNode = mAI->GetNearNodeList(lPosition)[0];
	mAI->SetStartNode(lNearNode->GetID());
}
void Enemy::SetAI(NcgLuaManager * aAI)
{
	mAI->mLuaAI = aAI;
	mAI->mLuaAI->SetFunction("GetPlan", 1, 6);	//関数の設定
}



void Enemy::Update()
{
	UpdateMesh();
	UpdateBullets();

	switch (mAI->GetNowAI())
	{
	case EnemyAIType::eUnKnown:
		UpdateAI();//AIが起動していない(停止している)場合、AI更新
		break;
	case EnemyAIType::eEnergyShot:
		UpdateEnergyShot();
		break;
	case EnemyAIType::eMovingHide:
		UpdateMovingHide();
		break;
	case EnemyAIType::eChargeEnergy:
		UpdateChargeEnergy();
		break;
	case EnemyAIType::eSearchTarget:
		UpdateSearching();
		break;
	case EnemyAIType::eMoveToTarget:
		UpdateMoveToTarget();
		break;
	case EnemyAIType::eMoveToBall:
		UpdateMoveToBall();
		break;
	case EnemyAIType::eMoveToBallTarget:
		UpdateMoveToBallTarget();
		break;
	case EnemyAIType::eMoveToGoal:
		UpdateMoveToGoal();
		break;
	default:
		break;
	}
	//行動処理が全て終わってから衝突判定
	UpdateCollision(true);
}

void Enemy::InitStatus()
{
	mStatus.mEnergy = 1;
	mStatus.mTargetting = false;
}

void Enemy::UpdateAI()
{
	mAI->ClearRoot();
	mAI->Update(
		mStatus.mTargetting,
		mStatus.mTarget!=nullptr, 
		mStatus.mEnergy > 0,
		mStatus.mTarget==mField->mBallHoldChara&&mStatus.mTarget!=nullptr,
		mStatus.mBall!=nullptr,
		mField->mBallIsField
		);
}

void Enemy::Render()
{
	assert(mRender);
	mRender->SetShader(mShader);
	mRender->Render(*mTransform);
	RenderBullets();

}

/*
	大抵は一度隠れたあと(見失ったあとに処理される)
	隠れたあとは移動が完全に終わっているので、移動ルートは空状態。
	必ず空状態であることを利用して、空のときに移動ルートを初期化する
*/
void Enemy::UpdateMoveToTarget()
{
	//最後に敵を見た位置をゴールとする
	if (mAI->IsRootEmpty() == true) {
		//最後に敵を見た位置から一番近いノードをゴールとする
		auto lGoalID=mAI->GetNearNodeList(mStatus.mLastLookPosiion)[0]->GetID();

		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(lGoalID);

	}

	//戻る最中に視界に入れば発見。そのターゲットをロックオンする
	auto lLookTarget = IsCulling();
	if (lLookTarget) {
		//ボール以外の場合
		if (!dynamic_cast<Ball*>(lLookTarget)) {
			//発見したらシーケンスをすすめる
			mAI->NextAI();
			//敵を視認中
			mStatus.mTargetting = true;
			//捉えたターゲットを記憶
			mStatus.mTarget = lLookTarget;
			return;
		}
	}


	//隠れれる位置まで移動
	if (!MoveNode()) {
		//移動完了してしまった場合、発見できなかったので、ロックオンフラグ(ターゲットをnullクリア)を外して
		//次のAIへ
		mAI->ClearAI();
		
		mStatus.mTarget = nullptr;
	}

}


/*
	最初に移動先が決定しているか調べる。
	決定していなければ、次に移動するノードの決定。(後戻りはしない)

	移動処理。移動先ノードに向かって移動する
	この移動の際、毎フレームターゲット捜査する。
	ターゲットを確実に視界に捉えた時、
	次のルーチンに進む。
*/
void Enemy::UpdateSearching()
{
	//ランダム移動処理
	{
		if (mAI->IsRootEmpty() == true) {
			//ルートを生成する
			mAI->CreateNextRoot(false);
		}
		MoveNode();
		
	}


	//登録している走査ターゲット全てでループさせる
	auto lLookTarget = IsCulling();
	if (lLookTarget) {
		//発見したらシーケンスをすすめる
		mAI->NextAI();
		//敵を視認中
		mStatus.mTargetting = true;
		//捉えたターゲットを記憶
		mStatus.mTarget = lLookTarget;				
	}







}

//エネルギーのチャージ
//チャージ後AI移行
void Enemy::UpdateChargeEnergy()
{
	mAI->NextAI();
	mStatus.mEnergy = 100;
}

//敵から隠れるため、近い順にノードを取得する
//各ノードからレイを発射し、レイが当たらないノードをゴールとする
void Enemy::UpdateMovingHide()
{
	//このフローに来る場合は必ずAI初期化後とする
	if (mAI->IsRootEmpty() == true) {
		//自分自身の座標
		DXVector3 lPosition;
		GetWorld()->GetMatrix().lock()->GetT(lPosition);
		auto lNearNodeList = mAI->GetNearNodeList(lPosition);

		//レイ発射位置を設定(ターゲット座標)
		mRayPick->SetFramePosition(*mStatus.mTarget->GetTransform());

		//一番近いノードをレイ発射位置とする
		mAI->SetStartNode(lNearNodeList[0]->GetID());
		//ターゲットから各ノードの座標からレイを飛ばしてヒットすれば見つかったままなのでアウト
		DXVector3 lTmpVec3;
		Dijkstra::Node* lGoalNodePtr{};
		for (auto& lNode : lNearNodeList) {
			//ノードに移動したと仮定して処理
			GetWorld()->SetT(lNode->Position);

			//どれか一つでもヒットしていると隠れた判定
			for (auto&lCollision : mCollisionTargets) {
				if (mRayPick->Collision(lTmpVec3, *mTransform, *lCollision->GetTransform())) {
					//ゴールノードの確定
					lGoalNodePtr = lNode;
				}
			}
			if (lGoalNodePtr) {
				break;
			}
		}
		//隠れることができない場合、一番遠くのノードに移動する
		lGoalNodePtr = lGoalNodePtr ? lGoalNodePtr : lNearNodeList.back();


		//位置を元に戻す
		GetWorld()->SetT(lPosition);
		//移動ルートの確定
		mAI->CreateRoot(lGoalNodePtr->GetID());
		//最後にターゲットを見た位置を記憶する
		mStatus.mTarget->GetTransform()->GetWorld().lock()->GetMatrix().lock()->GetT(mStatus.mLastLookPosiion);
		//隠れるため、必然的に視界から見失う
		mStatus.mTargetting = false;
	}
	
	//隠れれる位置まで移動
	if (!MoveNode()) {
		//移動完了したら次のAIへ
		mAI->NextAI();
	}

}

void Enemy::UpdateEnergyShot()
{
	//ターゲットの方向を向く
	float lRotateY = MSHormingY(*mTransform, *mStatus.mTarget->GetTransform(), 3.0f);
	if (IsZero(lRotateY, 0.001f)) {
		//振り向ききれば攻撃
		//弾の発射
		mBulletNormal->Create(mBullets, *this);
		//エネルギーを枯渇させる
		mStatus.mEnergy = 0;
		//次のAIへ移行する
		mAI->NextAI();
	}
	else {
		//振り向き処理
		GetWorld()->AddRC(0, lRotateY, 0);
	}

}

//ボールを手に入れているのでゴールまで移動する
void Enemy::UpdateMoveToGoal()
{
	if (mStatus.mInitMoveToGoal == false) {
		mStatus.mInitMoveToGoal = true;
		//移動先をゴールにする
		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(19);
	}

	//移動中に敵を発見した場合、現在のAIを破棄する
	auto lLookTarget = IsCulling();
	if (lLookTarget) {
		mAI->ClearAI();
		mStatus.mInitMoveToGoal = false;
	}

	//ゴールに到着したらボールを手放す
	if (!MoveNode()) {
		mField->RespawnBall();
		mStatus.mBall = nullptr;
		mStatus.mInitMoveToGoal = false;
		//AI再起動
		mAI->ClearAI();
	}

}
//ボールの位置まで移動する(拾いに行く)
void Enemy::UpdateMoveToBall()
{
	//ボールの位置を取得する
	if (mStatus.mInitMoveToBall == false) {
		mStatus.mInitMoveToBall = true;
		DXVector3 lBallPos;
		mField->mBall->GetWorld()->GetMatrix().lock()->GetT(lBallPos);
		auto lGoalID = mAI->GetNearNodeList(lBallPos)[0]->GetID();
		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(lGoalID);
	}

	auto lLookTarget = IsCulling();
	if (dynamic_cast<Ball*>(lLookTarget)) {
		//ボールだった場合は追跡する
		float lRotateY;
		lRotateY = MSHormingY(*mTransform, *lLookTarget->GetTransform(), 5.0f);
		GetWorld()->AddRC(0, lRotateY, 0);
		//移動処理
		if (IsZero(lRotateY, 0.1f)) {
			GetWorld()->AddT(DXWorld::TYPE_ROTATE, 0.1f, { 0,0,1 });
		}
	}
	else {
		MoveNode();
		//誰かがボールを取った場合、AIクリア
		mAI->ClearAI();
		mStatus.mInitMoveToBall = false;
	}

	//ボールと衝突したか調べる
	auto lHitTargets = UpdateCollision(false);
	for (auto&lHitTarget : lHitTargets) {
		if (lHitTarget) {
			//ボールに当たった場合、そのボールを回収する
			Ball* lBall = dynamic_cast<Ball*>(lHitTarget);
			if (lBall) {
				mStatus.mBall = lBall;
				mField->SetBallHolder(this);
				//回収後、AIを進める
				mAI->NextAI();
			}
		}
	}



}

void Enemy::UpdateMoveToBallTarget()
{
}

bool Enemy::MoveNode()
{
	MyNode*lAINode;
	static int count = 0;
	mAI->GetFrontNode(lAINode);
	//移動先ノードがなかったときは処理しない
	if (!lAINode)return false;
	float lAng = MSHormingY(*mTransform, lAINode->Position, 6.0f);
	GetWorld()->AddRC(0, lAng, 0);
	if (IsZero(lAng, 5.1f)) {
		GetWorld()->AddT(DXWorld::TYPE_ROTATE, 0.1f, { 0,0,1 });
		DXVector3 lLength;
		GetWorld()->GetMatrix().lock()->GetT(lLength);
		lLength = lLength - lAINode->Position;
		if (lLength.GetDistance() < 0.5f) {
			printf("%d\n", lAINode->GetID());
			if (lAINode->GetID() == 1) {
				++count;
			}
			return mAI->SetNextNode();

		}
	}
	return true;
}


GameObjectBase * Enemy::IsCulling()
{
	GameObjectBase* lLookTargetPtr{};
	//視界処理
	//MSCullingFrustum cf;
	for (auto&lTarget : mSearchTargets) {
		if (/*cf.IsCullingWorld(*mTransform, *lTarget)*/true) {
			if (MSCullingOcculusion::IsCullingWorld(
				*mRender, *mTransform, *lTarget->GetTransform(), 0.02f,
				[&,this]() {
				for (auto&lCollision : mCollisionTargets) {
					//登録済みコリジョンがカリングターゲットと同じだった場合は障害物として描画しない
					if (lTarget == lCollision)continue;
					mRender->Render(*lCollision->GetTransform());
				}
			}
			)) {
				lLookTargetPtr = lTarget;
			}
		}
	}
	return lLookTargetPtr;
}

