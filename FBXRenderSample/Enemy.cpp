#include "Enemy.h"
#include"NcgLua.hpp"
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
#include"CharacterBase.h"
#include"StatusBulletBase.h"
#include"ChangeStates.hxx"
#include"StatusField.h"
#include"BulletManager.h"
Enemy::Enemy()
{
	//mBulletNormal = std::make_unique<BulletNormal>();
	mStatus = std::make_unique<EnemyStatus>();
}
Enemy::~Enemy()
{
}
void Enemy::Initialize(StatusField&pSetStatus)
{
	CharacterBase::Initialize(pSetStatus);

	mAI = std::make_unique<EnemyAI>();
	mAI->CreateNodes(*mField);

	//ステータスの初期化
	InitStatus();
}
void Enemy::InitFinal()
{
	//ゴールの初期化
	SetGoalIndex(mField->GetTeamAlly(this)->mGoalIndex);
	
	mRayPick->SetFramePosition(*mTransform);
	//一番近いノードを初期座標としておく
	DXVector3 lPosition;
	GetWorld()->GetMatrix().lock()->GetT(lPosition);
	auto lNearNode = mAI->GetNearNodeList(lPosition)[0];
	mAI->SetStartNode(lNearNode->GetID());
	mTransform->mMesh->SetAnimation(0);
}
void Enemy::SetAI(NcgLuaManager * aAI)
{
	mAI->mLuaAI = aAI;
	mAI->mLuaAI->SetFunction("GetPlan", 1, 11);	//関数の設定
}

void Enemy::SetGoalIndex(int aIndex)
{
	
	GetStatus<EnemyStatus>()->mGoalIndex = aIndex;
}



void Enemy::Update()
{
	UpdateMesh();
	UpdateBullets();
	
	if (mStatus->mLive == CharaStateFlag::ALIVE) {

		switch (mAI->GetNowAI())
		{
		case EnemyAIType::eUnKnown:
			UpdateAI();
			break;
		case EnemyAIType::eMoveToBall:
			BetaMoveToBall();
			break;
		case EnemyAIType::eMoveToGoal:
			BetaMoveToGoal();
			break;
		case EnemyAIType::eMoveToHide:
			BetaMoveToHide();
			break;
		case EnemyAIType::eChargeEnergy:
			BetaChargeEnergy();
			break;
		case EnemyAIType::eInSightAttack:
			BetaInSightAttack();
			break;
		case EnemyAIType::eMoveToBallHoldAlly:
			BetaMoveToBallHoldAlly();
			break;
		case EnemyAIType::eSearchEnemyShort:
			BetaSearchEnemyShort();
			break;
		case EnemyAIType::eSerachEnemyAll:
			BetaSearchEnemyAll();
			break;
		case EnemyAIType::eMoveToLookingTarget:
			BetaMoveToLookingTarget();
			break;
		case EnemyAIType::eMoveToBallTarget:
			BetaMoveToBallTarget();
			break;
		case EnemyAIType::eSearchForEnemyArea:
			BetaSearchForEnemyArea();
			break;
		case EnemyAIType::eSearchForAllyArea:
			BetaSearchForAllyArea();
			break;
		default:
			break;
		}
	}


	UpdateAlive();

	//行動処理が全て終わってから衝突判定
	UpdateCollision(true);
}

void Enemy::UpdateAlive()
{

	switch (mStatus->mLive)
	{
	case CharaStateFlag::ALIVE:
		break;
	case CharaStateFlag::DEAD:
		mStatus->mLive = CharaStateFlag::RESPAWNWAIT;
		break;
	case CharaStateFlag::RESPAWNWAIT:
	{
		//スポーン
		//InitStatus();
		mAI->ClearAI();
		Respawn();
		GetWorld()->mPosition;
		printf("%.2f,%.2f,%.2f\n", GetWorld()->mPosition->x, GetWorld()->mPosition->y, GetWorld()->mPosition->z);
		auto lNowNode = mAI->GetNearNodeList(*GetWorld()->mPosition)[0];
		mAI->SetNowNode(lNowNode);
		mRayPick->SetFramePosition(*mTransform);

	}
		break;
	default:
		break;
	}

}

void Enemy::InitStatus()
{
	auto lStatus = GetStatus<EnemyStatus>();

	lStatus->mTargetting = false;
	lStatus->mLastLookPosiion = {};
	lStatus->mAllyComplianceCount = 0;
	lStatus->mAllyNear = false;
	lStatus->mBall = nullptr;
	lStatus->mBallAllyNear = false;
	lStatus->mEnergy.Set(5.0f,0.0f,2.0f);
	lStatus->mHp.Set(10.0f, .0f, 10.0f);
	lStatus->mIsTargetingBall = false;
	lStatus->mTargetChara = nullptr;
	lStatus->mTargetting = false;
	lStatus->mAIType = EnemyAIType::Type::eUnKnown;
	lStatus->mLive = CharaStateFlag::ALIVE;
}

void Enemy::UpdateAI()
{
	mAI->ClearRoot();

	auto lStatus = GetStatus<EnemyStatus>();
	auto lBulletStatus = &mBltManager->mStatusMap[this];

	struct tAIStatus {
		//BallHolder
		bool mBallHoldField{ false };
		bool mBallHoldAlly{ false };
		bool mBallHoldEnemy{ false };
		bool mBallHoldMe{ false };

		//EnemyState
		bool mInSigntEnemy{ false };
		bool mLockonEnemy{ false };
		bool mBallHoldTarget{ false };

		//AllyState
		bool mInSightAlly{ false };
		bool mBallAllyNear{ false };
		bool mAllyNear{ false };

		//Other
		bool mChargedEnergy{ false };

		

	}lAIStatus;

	lAIStatus.mBallHoldField = mField->mBallHoldChara == nullptr;


	auto lTeamAlly= mField->GetTeamAlly(mField->mBallHoldChara);
	auto lTeamEnemy = mField->GetTeamEnemy(mField->mBallHoldChara);
	if (lTeamAlly) {
		lAIStatus.mBallHoldAlly = lTeamAlly->IsMember(this);
	}
	if (lTeamEnemy) {
		lAIStatus.mBallHoldEnemy = lTeamEnemy->IsMember(this);
	}
	
	
	
	lAIStatus.mBallHoldMe = lStatus->mBall != nullptr;

	lAIStatus.mInSigntEnemy = lStatus->mTargetting;
	lAIStatus.mLockonEnemy = lStatus->mTargetChara != nullptr;

	lAIStatus.mBallHoldTarget = mField->mBallHoldChara != nullptr&&lStatus->mTargetChara == mField->mBallHoldChara;

	lAIStatus.mInSightAlly = false;


	lAIStatus.mBallAllyNear = false;
	if (lAIStatus.mBallHoldAlly==true) {
		if (GetDistance(mField->mBallHoldChara) < 10.0f) {
			lAIStatus.mBallAllyNear = true;
			lAIStatus.mAllyNear = true;
		}
	}

	//味方全走査
	lAIStatus.mAllyNear = false;
	for (decltype(auto)lAlly : mField->GetTeamAlly(this)->mMembers) {
		if (lAIStatus.mAllyNear == true)break;
		//自身はチェックしない
		if (lAlly == this) continue;
		//一人でも近くにいればtrue
		if (GetDistance(lAlly) < 10.0f) {
			lAIStatus.mAllyNear = true;
		}
	}
	lAIStatus.mChargedEnergy = lStatus->mEnergy.GetNow() > lBulletStatus->mCost;

	mAI->Update(
		lAIStatus.mBallHoldField,
		lAIStatus.mBallHoldAlly,
		lAIStatus.mBallHoldEnemy,
		lAIStatus.mBallHoldMe,

		//EnemyState
		lAIStatus.mInSigntEnemy,
		lAIStatus.mLockonEnemy,
		lAIStatus.mBallHoldTarget,

		//AllyState
		lAIStatus.mInSightAlly,
		lAIStatus.mBallAllyNear,
		lAIStatus.mAllyNear,

		//Other
		lAIStatus.mChargedEnergy
		);
}

void Enemy::Render()
{
	assert(mRender);
	mRender->SetShader(mShader);
	mRender->Render(*mTransform);
	RenderBullets();

}


Ball* Enemy::UtlCollisionBall()
{
	auto lHitTargets = UpdateCollision(false);
	for (auto&lHitTarget : lHitTargets) {
		if (lHitTarget) {
			//ボールに当たった場合、そのボールを回収する
			Ball* lBall = dynamic_cast<Ball*>(lHitTarget);
			if (lBall) {
				return lBall;
			}
		}
	}
	return nullptr;
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

			auto lTmpThis = mTransform;
			DXVector3 lTmpSThis = *GetWorld()->mPosition;
			mCollisionMesh->GetWorld().lock()->SetT(lTmpSThis);
			mCollisionMesh->GetWorld().lock()->SetRC(*GetWorld()->mRotationCenter);
			if (mIsCollisionScaleDefault == true) {
				GetWorld()->GetMatrix().lock()->GetS(lTmpSThis);
				mCollisionMesh->GetWorld().lock()->SetS(lTmpSThis);
			}
			auto& lThisCam = *mTransform->GetCamera().lock();
			auto& lThisProj = *mTransform->GetProjection().lock();
			mCollisionMesh->SetCamera(lThisCam);
			mCollisionMesh->SetProjection(lThisProj);


			mTransform = mCollisionMesh;


			if (MSCullingOcculusion::IsCullingWorld(
				*mRender, *mTransform, *lTarget->GetTransform(), 0.02f,
				[&,this]() {


				for (auto&lCollision : mCollisionTargets) {

					auto lTmpMesh = lCollision->mTransform;
					DXVector3 lTmpS;
					//判定メッシュをコリジョン用に変更
					lCollision->GetWorld()->GetMatrix().lock()->GetT(lTmpS);
					lCollision->mCollisionMesh->GetWorld().lock()->SetT(lTmpS);
					lCollision->mCollisionMesh->GetWorld().lock()->SetRC(*lCollision->GetWorld()->mRotationCenter);


					//コリジョンスケールが別で設定されていない場合は
					//メッシュのコリジョンスケールを使う
					if (lCollision->mIsCollisionScaleDefault == true) {
						lCollision->GetWorld()->GetMatrix().lock()->GetS(lTmpS);
						lCollision->mCollisionMesh->GetWorld().lock()->SetS(lTmpS);
					}

					lCollision->mTransform = lCollision->mCollisionMesh;

					if (lCollision->IsActive() == false) {
						lCollision->mTransform = lTmpMesh;
						continue;
					}
					//登録済みコリジョンがカリングターゲットと同じだった場合は障害物として描画しない
					if (lTarget == lCollision) {
						lCollision->mTransform = lTmpMesh;
						continue;
					}
					mRender->Render(*lCollision->GetTransform());

					lCollision->mTransform = lTmpMesh;


				}
			}
			)) {
				lLookTargetPtr = lTarget;
			}

			mTransform = lTmpThis;

		}
	}
	return lLookTargetPtr;
}
void Enemy::BetaMoveToBall()
{
	//想定できる状況
	/*
		ボールとの衝突
		敵との遭遇
		味方がボールを取得
		敵がボールを取得
	*/
	
	//初期化処理
	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eMoveToBall) {
		printf("Task:%s\n", "MoveToBall");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eMoveToBall;
		//ボールのある場所をゴールとする
		DXVector3 lBallPos;
		mField->mBall->GetWorld()->GetMatrix().lock()->GetT(lBallPos);
		auto lGoalID = mAI->GetNearNodeList(lBallPos)[0]->GetID();
		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(lGoalID);
	}

	//視界処理
	auto lLookTarget = IsCulling();

	//視界に入ったのがキャラクターの場合、
	//攻撃対象としてロックオン
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		//追従対象をキャラクターに移す
		GetStatus<EnemyStatus>()->mIsTargetingBall = false;
		GetStatus<EnemyStatus>()->mTargetting = true;
		GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
		mAI->ClearAI();
		return;
	}
	//視界に入ったのがボールの場合
	else if (dynamic_cast<Ball*>(lLookTarget)) {
		GetStatus<EnemyStatus>()->mIsTargetingBall = true;
	}

	//ボールを補足済みかそうでないかで処理を変更
	if (GetStatus<EnemyStatus>()->mIsTargetingBall == true) {
		//捕捉済みの場合はホーミング処理
		float lRotateY;
		lRotateY = MSHormingY(*mTransform, *mField->mBall->GetTransform(), 6.0f);
		GetWorld()->AddRC(0, lRotateY, 0);
		//移動処理
		if (IsZero(lRotateY, 0.1f)) {
			GetWorld()->AddT(DXWorld::TYPE_ROTATE, 0.1f, { 0,0,1 });
		}
	}

	else {
		//ノード単位での移動
		MoveNode();
	}


	//誰かがボールを取った場合、
	//フィールドにボールがなくなるので、
	//AIの更新
	if (mField->mBallHoldChara != nullptr) {
		mAI->ClearAI();
		return;
	}

	//自身がボールと衝突した場合
	auto lHitBall = UtlCollisionBall();
	//当たればボールを回収
	if (lHitBall) {
		GetStatus()->mBall = lHitBall;
		mField->SetBallHolder(this);
		//AIを進める
		mAI->NextAI();
	}



}

void Enemy::BetaMoveToGoal()
{
	/*
		想定できる状況
		敵との遭遇
	*/

	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eMoveToGoal) {
		printf("Task:%s\n", "MoveToGoal");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eMoveToGoal;

		//移動先をゴールにする
		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(mField->GetTeamAlly(this)->mGoalIndex);

	}

	//移動中に敵と遭遇した場合、現在のAIを破棄する
	auto lLookTarget = IsCulling();
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		//追従対象をキャラクターに移す
		GetStatus<EnemyStatus>()->mTargetting = true;
		GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
		mAI->ClearAI();
		return;
	}

	if (!MoveNode()) {

		printf("%.2f\t%.2f\t%.2f\n",
			GetWorld()->mPosition->x,
			GetWorld()->mPosition->y,
			GetWorld()->mPosition->z);

		mField->RespawnBall();
		GetStatus()->mBall = nullptr;
		GetStatus<EnemyStatus>()->mIsTargetingBall = false;

		mAI->ClearAI();
	}

}

void Enemy::BetaMoveToHide()
{
	//隠れる
	//隠れる動作中にボールが当たれば回収する
	//毎フレームレイチェックを行う
	//対象が死んだ場合、隠れる必要はなくなるので隠れる処理完了



	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eMoveToHide) {

		printf("Task:%s\n", "MoveToHide");

		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eMoveToHide;
		//自身の座標を一時退避
		DXVector3 lTempPosition;
		GetWorld()->GetMatrix().lock()->GetT(lTempPosition);
		//近い順にノードを取得
		decltype(auto) lNearNodeList = mAI->GetNearNodeList(lTempPosition);
		//自身から一番近いノードをレイ発射位置とする
		mAI->SetStartNode(lNearNodeList[0]->GetID());


		//レイ発射位置を設定(ターゲット座標)
		mRayPick->SetFramePosition(*GetStatus<EnemyStatus>()->mTargetChara->GetTransform());

		DXVector3 lTmpVec3;
		Dijkstra::Node* lGoalNodePtr{};
		for (auto& lNode : lNearNodeList) {
			//ノードに移動したと仮定して処理
			GetWorld()->SetT(lNode->Position);



			//どれか一つでもヒットしていると隠れた判定
			for (auto&lCollision : mCollisionTargets) {

				auto lTmpMesh = lCollision->mTransform;
				DXVector3 lTmpS;
				//判定メッシュをコリジョン用に変更
				lCollision->GetWorld()->GetMatrix().lock()->GetT(lTmpS);
				lCollision->mCollisionMesh->GetWorld().lock()->SetT(lTmpS);
				lCollision->mCollisionMesh->GetWorld().lock()->SetRC(*lCollision->GetWorld()->mRotationCenter);


				//コリジョンスケールが別で設定されていない場合は
				//メッシュのコリジョンスケールを使う
				if (lCollision->mIsCollisionScaleDefault == true) {
					lCollision->GetWorld()->GetMatrix().lock()->GetS(lTmpS);
					lCollision->mCollisionMesh->GetWorld().lock()->SetS(lTmpS);
				}

				lCollision->mTransform = lCollision->mCollisionMesh;

				if (lCollision->IsActive() == false) {
					lCollision->mTransform = lTmpMesh;
					continue;
				}
				if (mRayPick->Collision(lTmpVec3, *mTransform, *lCollision->GetTransform())) {
					//ゴールノードの確定
					lGoalNodePtr = lNode;
				}
				lCollision->mTransform = lTmpMesh;

			}
			if (lGoalNodePtr) {
				break;
			}
		}

		//隠れることができない場合、一番遠くのノードに移動する
		lGoalNodePtr = lGoalNodePtr ? lGoalNodePtr : lNearNodeList.back();
		//位置を元に戻す
		GetWorld()->SetT(lTempPosition);
		//移動ルートの確定
		mAI->CreateRoot(lGoalNodePtr->GetID());
		//最後にターゲットを見た位置を記憶する
		GetStatus<EnemyStatus>()->mTargetChara->GetTransform()->GetWorld().lock()->GetMatrix().lock()->GetT(GetStatus<EnemyStatus>()->mLastLookPosiion);
		//隠れるため、必然的に視界から見失う
		GetStatus<EnemyStatus>()->mTargetting = false;


	}

	//隠れる最中にターゲットが死んだ場合、隠れる必要がなくなるので
	//その場でチャージを開始する
	if (GetStatus()->mTargetChara == nullptr) {
		mAI->NextAI();
		return;
	}

	bool lMoveNow = MoveNode();
	//移動し終わればチャージ
	if (!lMoveNow) {
		mAI->NextAI();
	}


	//毎フレームレイ判定を行い、
	//敵の範囲内から消えたら即チャージ
	for (decltype(auto)lCollision : mCollisionTargets) {
		
		auto lTmpMesh = lCollision->mTransform;
		DXVector3 lTmpS;
		//判定メッシュをコリジョン用に変更
		lCollision->GetWorld()->GetMatrix().lock()->GetT(lTmpS);
		lCollision->mCollisionMesh->GetWorld().lock()->SetT(lTmpS);
		lCollision->mCollisionMesh->GetWorld().lock()->SetRC(*lCollision->GetWorld()->mRotationCenter);


		//コリジョンスケールが別で設定されていない場合は
		//メッシュのコリジョンスケールを使う
		if (lCollision->mIsCollisionScaleDefault == true) {
			lCollision->GetWorld()->GetMatrix().lock()->GetS(lTmpS);
			lCollision->mCollisionMesh->GetWorld().lock()->SetS(lTmpS);
		}

		lCollision->mTransform = lCollision->mCollisionMesh;

		if (lCollision->IsActive() == false) {
			lCollision->mTransform = lTmpMesh;
			continue;
		}

		//Rayにはエネミー座標が入っている
		DXVector3 lTmpVec3;
		if (mRayPick->Collision(lTmpVec3, *GetStatus<EnemyStatus>()->mTargetChara->mTransform, *lCollision->GetTransform())) {
			//当たった場合、隠れているので次のAIに移行
			mAI->NextAI();
		}

		lCollision->mTransform = lTmpMesh;

	}
	



	//途中ボールに当たった場合、回収する
	auto lHitBall = UtlCollisionBall();
	if (lHitBall) {
		GetStatus()->mBall = lHitBall;
		mField->SetBallHolder(this);
		//回収後AIを進める
		mAI->NextAI();
	}

}

void Enemy::BetaChargeEnergy()
{
	//想定できる事例
	//敵との遭遇
	//ボールと接触

	//初期化
	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eChargeEnergy) {
		printf("Task:%s\n", "ChargeEnergy");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eChargeEnergy;
	}

	//移動中に敵を発見した場合、一定以上エネルギーが溜まっていればAIを破棄する
	auto lLookTarget = IsCulling();
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {

		if (GetStatus<EnemyStatus>()->mEnergy.GetNowPer() >= 0.6f) {
			GetStatus<EnemyStatus>()->mTargetting = true;
			GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
			mAI->ClearAI();
			return;
		}

	}

	//エネルギーチャージ
	if (GetStatus<EnemyStatus>()->mEnergy.GetNowPer() < 1.0f) {
		GetStatus<EnemyStatus>()->mEnergy.AddPer(0.01f);
	}
	//チャージが完了したらAI移行
	else {
		mAI->NextAI();
	}

	//途中ボールに当たった場合、回収する
	auto lHitBall = UtlCollisionBall();
	if (lHitBall) {
		GetStatus()->mBall = lHitBall;
		mField->SetBallHolder(this);
		//回収後AIを進める
		mAI->ClearAI();
	}

}

void Enemy::BetaInSightAttack()
{
	/*
		想定できる状況
		敵を見失う
		敵が倒れる
		ボールと接触
	*/

	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eInSightAttack) {
		printf("Task:%s\n", "InSightAttack");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eInSightAttack;
	}

	//ターゲットが倒れた場合、AIクリア
	{
		CharacterBase* lTargetChara = static_cast<CharacterBase*>(GetStatus<EnemyStatus>()->mTargetChara);
		if (lTargetChara->GetStatus()->mLive != CharaStateFlag::ALIVE) {
			//ターゲティングも初期化する
			GetStatus<EnemyStatus>()->mTargetting = false;
			mAI->ClearAI();
			return;
		}
	}

	//ターゲットホーミング処理
	//ターゲットの方向を向く
	float lRotateY = MSHormingY(*mTransform, *GetStatus<EnemyStatus>()->mTargetChara->GetTransform(), 3.0f);
	if (IsZero(lRotateY, 0.25f)) {
		//振り向ききれば攻撃
		//弾の発射
		auto lActiveBullet = mBltManager->GetActiveBullet(this);

		ChangeStates::BulletShot(mBullets, this, lActiveBullet);
		//エネルギーがなくなった場合、
		//次のAIへ移行する
		if (!ChangeStates::IsAttackDo(this, lActiveBullet)) {
			mAI->NextAI();
		}
	}
	else {
		//振り向き処理
		GetWorld()->AddRC(0, lRotateY, 0);
	}

	////敵が視界にいるか毎フレーム調べる
	auto lLookTarget = IsCulling();
	//if (dynamic_cast<CharacterBase*>(lLookTarget)) {



	//}


	//途中ボールに当たった場合、回収する
	auto lHitBall = UtlCollisionBall();
	if (lHitBall) {
		GetStatus()->mBall = lHitBall;
		mField->SetBallHolder(this);
		//回収後AIを進める
		mAI->ClearAI();
	}

}

void Enemy::BetaMoveToBallHoldAlly()
{
	/*
		想定できる状況

		ボールを持っている味方が倒される
		敵を発見
		ボールと接触
	*/
	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eMoveToBallHoldAlly) {
		printf("Task:%s\n", "MoveToBallHoldAlly");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eMoveToBallHoldAlly;

		//移動先をボール所持者にする

		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();

		DXVector3& lPosition = *mField->mBallHoldChara->GetWorld()->mPosition;
		auto lNearNode = mAI->GetNearNodeList(lPosition)[0];

		mAI->CreateRoot(lNearNode->GetID());

		GetStatus<EnemyStatus>()->mAllyComplianceCount = 0;
	}

	//ボールを持った味方がやられた場合(落とした場合、
	//AIクリア
	if (mField->mBallHoldChara == nullptr) {
		mAI->ClearAI();
	}



	//60フレーム毎にゴール更新
	if (!MoveNode()||GetStatus<EnemyStatus>()->mAllyComplianceCount >= 60) {
		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();

		DXVector3& lPosition = *mField->mBallHoldChara->GetWorld()->mPosition;
		auto lNearNode = mAI->GetNearNodeList(lPosition)[0];

		mAI->CreateRoot(lNearNode->GetID());

		GetStatus<EnemyStatus>()->mAllyComplianceCount = 0;
	}

	//移動中に敵を発見した場合、現在のAIを破棄する
	auto lLookTarget = IsCulling();
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		//敵発見処理
		GetStatus<EnemyStatus>()->mTargetting = true;
		GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
		mAI->ClearAI();
	}


	//途中ボールに当たった場合、回収する
	auto lHitBall = UtlCollisionBall();
	if (lHitBall) {
		GetStatus()->mBall = lHitBall;
		mField->SetBallHolder(this);
		//回収後AIを進める
		mAI->ClearAI();
	}

	++GetStatus<EnemyStatus>()->mAllyComplianceCount;



}

void Enemy::BetaSearchEnemyShort()
{
	//想定できる状況
	/*
		味方と離れる
		ボールがフィールドに落ちる
		敵との遭遇
	*/

	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eSerachEnemyAll) {
		printf("Task:%s\n", "SearchEnemyShort");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eSerachEnemyAll;
		GetStatus<EnemyStatus>()->mAllyComplianceCount = 0;

		//移動先を適当な場所にする
		mAI->ClearRoot();
	}

	if (mAI->IsRootEmpty() == true) {
		//ルートを生成する
		mAI->CreateNextRoot(false);
	}
	//移動に失敗すればAI更新
	if (!MoveNode()) {
		mAI->ClearAI();
	}

	//敵との遭遇
	auto lLookTarget = IsCulling();
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		//発見したらシーケンスをすすめる
		mAI->NextAI();
		//敵を視認中
		GetStatus<EnemyStatus>()->mTargetting = true;
		//捉えたターゲットを記憶
		GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
		return;
	}


	//ボール所持者がいなくなった場合
	if (mField->mBallHoldChara == nullptr) {
		mAI->ClearAI();
	}


//	//60フレーム毎に(現在未実装)
	//毎フレーム
	//味方との距離計算
	//一定距離離れたらAIクリア(更新)
	{
		DXVector3 lBallHolderPos;
		DXVector3 lThisPos;
		float lDistance;
		lBallHolderPos = *mField->mBallHoldChara->GetWorld()->mPosition;
		lThisPos = *GetWorld()->mPosition;
		lDistance = (lBallHolderPos - lThisPos).GetDistance();
		if (lDistance < 10.0f) {
			mAI->ClearAI();
		}
	}

	if (GetDistance(mField->mBallHoldChara) > 10.0f) {
		mAI->ClearAI();
	}




}

void Enemy::BetaSearchEnemyAll()
{
	/*
		想定できる状況
		//誰かがボールを取得
		敵を捉える
	*/

	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eSerachEnemyAll) {
		printf("Task:%s\n", "SearchEnemyAll");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eSerachEnemyAll;
	}

	//ランダム移動処理
	{
		if (mAI->IsRootEmpty() == true) {
			//ルートを生成する
			mAI->CreateNextRoot(false);
		}
		//移動に失敗すればAI更新
		if (!MoveNode()) {
			mAI->ClearAI();
		}

	}

	//auto lLookTarget = IsCulling();
	//if (lLookTarget) {
	//	//ボール以外の場合
	//	if (!dynamic_cast<Ball*>(lLookTarget)) {
	//		//発見したらシーケンスをすすめる
	//		mAI->NextAI();
	//		//敵を視認中
	//		GetStatus<EnemyStatus>()->mTargetting = true;
	//		//捉えたターゲットを記憶
	//		GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
	//		return;
	//	}
	//}


}

void Enemy::BetaMoveToLookingTarget()
{
	/*
		想定できる状況
		味方がボールを取る
		敵がボールを捕る
		敵との遭遇
		ボール所持者が死亡
	*/

	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eSerachEnemyAll) {
		printf("Task:%s\n", "MoveToLookingTarget");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eSerachEnemyAll;
	}

	//最後に敵を見た位置をゴールとする
	if (mAI->IsRootEmpty() == true) {
		//最後に敵を見た位置から一番近いノードをゴールとする
		auto lGoalID=mAI->GetNearNodeList(GetStatus<EnemyStatus>()->mLastLookPosiion)[0]->GetID();
	
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
			GetStatus<EnemyStatus>()->mTargetting = true;
			//捉えたターゲットを記憶
			GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
			return;
		}
	}
	
	
	if (!MoveNode()) {
		//移動完了してしまった場合、発見できなかったので、ロックオンフラグ(ターゲットをnullクリア)を外して
		//次のAIへ
		mAI->ClearAI();
			
		GetStatus<EnemyStatus>()->mTargetChara = nullptr;
	}
}

void Enemy::BetaMoveToBallTarget()
{
	//想定可能な状況
	/*
		敵との遭遇
		ボールが落ちる
	*/
	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eMoveToBallTarget) {
		printf("Task:%s\n", "MoveToBallTarget");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eMoveToBallTarget;
		DXVector3 lBallPos;
		mField->mBallHoldChara->GetWorld()->GetMatrix().lock()->GetT(lBallPos);
		auto lGoalID = mAI->GetNearNodeList(lBallPos)[0]->GetID();
		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(lGoalID);
	}

	auto lLookTarget = IsCulling();
	//キャラクターであればロックオン
	//キャラクターであればロックオン
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		GetStatus<EnemyStatus>()->mTargetting = true;
		GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
		mAI->ClearAI();
		return;
	}
	
	bool lMoveEnd = MoveNode();
	//ボールを落とした(フィールドに出現した場合)AIクリア
	if (mField->mBallHoldChara == nullptr) {
		mAI->ClearAI();
		return;
	}

	//移動し終わった場合(敵を発見できなかった場合)
	if (!lMoveEnd) {
		//再度敵を探す
		DXVector3 lBallPos;
		mField->mBallHoldChara->GetWorld()->GetMatrix().lock()->GetT(lBallPos);
		auto lGoalID = mAI->GetNearNodeList(lBallPos)[0]->GetID();
		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(lGoalID);
	}
}

void Enemy::BetaSearchForEnemyArea()
{
	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eSearchForEnemyArea) {
		printf("Task:%s\n", "SearchForEnemyArea");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eSearchForEnemyArea;
	}

	assert(0);
}

void Enemy::BetaSearchForAllyArea()
{
	/*
		想定できる状況

		敵との遭遇
	*/
	
	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eSearchForAllyArea) {
		printf("Task:%s\n", "SearchForAllyArea");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eSearchForAllyArea;

		GetStatus<EnemyStatus>()->mAllyComplianceCount = 0;

		//相手チームを取得
		auto lEnemyTeam = mField->GetTeamEnemy(this);
		

		//味方エリアのどこかをゴールとする
		MyNode*lGoalNode;
		//味方チームのチームタイプを取得する
		eTeamType lEnemyTeamType = lEnemyTeam->mTeamType;
		do {

			lGoalNode = mAI->GetNodeRandom();
			//異なっていれば移動可能なノード
		} while (lGoalNode->mTeamType != lEnemyTeamType);
		//auto lGoalID = mAI->GetNearNodeList(*mField->mBallHoldChara->mTransform->GetWorld().lock()->mPosition)[0]->GetID();

		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(lGoalNode->GetID());

	}

	//ボールを持った味方がやられた場合(落とした場合、
	//AIクリア
	if (mField->mBallHoldChara == nullptr) {
		mAI->ClearAI();
	}

	//移動できなくなったらルート再構築する
	if (!MoveNode()) {
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eUnKnown;
		return;
	}
	//移動中に敵を発見した場合、現在のAIを破棄する
	auto lLookTarget = IsCulling();
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		//敵発見処理
		GetStatus<EnemyStatus>()->mTargetting = true;
		GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
		mAI->ClearAI();
	}
}
