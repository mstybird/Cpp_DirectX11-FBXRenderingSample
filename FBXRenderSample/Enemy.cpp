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
Enemy::Enemy()
{
	mBulletNormal = std::make_unique<BulletNormal>();
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

	//�X�e�[�^�X�̏�����
	InitStatus();
}
void Enemy::InitFinal()
{
	//�S�[���̏�����
	SetGoalIndex(mField->GetTeamAlly(this)->mGoalIndex);
	
	mRayPick->SetFramePosition(*mTransform);
	//��ԋ߂��m�[�h���������W�Ƃ��Ă���
	DXVector3 lPosition;
	GetWorld()->GetMatrix().lock()->GetT(lPosition);
	auto lNearNode = mAI->GetNearNodeList(lPosition)[0];
	mAI->SetStartNode(lNearNode->GetID());
	mTransform->mMesh->SetAnimation(0);
}
void Enemy::SetAI(NcgLuaManager * aAI)
{
	mAI->mLuaAI = aAI;
	mAI->mLuaAI->SetFunction("GetPlan", 1, 11);	//�֐��̐ݒ�
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

	//�s���������S�ďI����Ă���Փ˔���
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
		//�X�|�[��
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
	auto lBulletStatus = mBulletNormal->GetStatus();

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

	//�����S����
	lAIStatus.mAllyNear = false;
	for (decltype(auto)lAlly : mField->GetTeamAlly(this)->mMembers) {
		if (lAIStatus.mAllyNear == true)break;
		//���g�̓`�F�b�N���Ȃ�
		if (lAlly == this) continue;
		//��l�ł��߂��ɂ����true
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
			//�{�[���ɓ��������ꍇ�A���̃{�[�����������
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
	//�ړ���m�[�h���Ȃ������Ƃ��͏������Ȃ�
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
	//���E����
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
					//���胁�b�V�����R���W�����p�ɕύX
					lCollision->GetWorld()->GetMatrix().lock()->GetT(lTmpS);
					lCollision->mCollisionMesh->GetWorld().lock()->SetT(lTmpS);
					lCollision->mCollisionMesh->GetWorld().lock()->SetRC(*lCollision->GetWorld()->mRotationCenter);


					//�R���W�����X�P�[�����ʂŐݒ肳��Ă��Ȃ��ꍇ��
					//���b�V���̃R���W�����X�P�[�����g��
					if (lCollision->mIsCollisionScaleDefault == true) {
						lCollision->GetWorld()->GetMatrix().lock()->GetS(lTmpS);
						lCollision->mCollisionMesh->GetWorld().lock()->SetS(lTmpS);
					}

					lCollision->mTransform = lCollision->mCollisionMesh;

					if (lCollision->IsActive() == false) {
						lCollision->mTransform = lTmpMesh;
						continue;
					}
					//�o�^�ς݃R���W�������J�����O�^�[�Q�b�g�Ɠ����������ꍇ�͏�Q���Ƃ��ĕ`�悵�Ȃ�
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
	//�z��ł����
	/*
		�{�[���Ƃ̏Փ�
		�G�Ƃ̑���
		�������{�[�����擾
		�G���{�[�����擾
	*/
	
	//����������
	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eMoveToBall) {
		printf("Task:%s\n", "MoveToBall");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eMoveToBall;
		//�{�[���̂���ꏊ���S�[���Ƃ���
		DXVector3 lBallPos;
		mField->mBall->GetWorld()->GetMatrix().lock()->GetT(lBallPos);
		auto lGoalID = mAI->GetNearNodeList(lBallPos)[0]->GetID();
		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(lGoalID);
	}

	//���E����
	auto lLookTarget = IsCulling();

	//���E�ɓ������̂��L�����N�^�[�̏ꍇ�A
	//�U���ΏۂƂ��ă��b�N�I��
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		//�Ǐ]�Ώۂ��L�����N�^�[�Ɉڂ�
		GetStatus<EnemyStatus>()->mIsTargetingBall = false;
		GetStatus<EnemyStatus>()->mTargetting = true;
		GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
		mAI->ClearAI();
		return;
	}
	//���E�ɓ������̂��{�[���̏ꍇ
	else if (dynamic_cast<Ball*>(lLookTarget)) {
		GetStatus<EnemyStatus>()->mIsTargetingBall = true;
	}

	//�{�[����⑫�ς݂������łȂ����ŏ�����ύX
	if (GetStatus<EnemyStatus>()->mIsTargetingBall == true) {
		//�ߑ��ς݂̏ꍇ�̓z�[�~���O����
		float lRotateY;
		lRotateY = MSHormingY(*mTransform, *mField->mBall->GetTransform(), 6.0f);
		GetWorld()->AddRC(0, lRotateY, 0);
		//�ړ�����
		if (IsZero(lRotateY, 0.1f)) {
			GetWorld()->AddT(DXWorld::TYPE_ROTATE, 0.1f, { 0,0,1 });
		}
	}

	else {
		//�m�[�h�P�ʂł̈ړ�
		MoveNode();
	}


	//�N�����{�[����������ꍇ�A
	//�t�B�[���h�Ƀ{�[�����Ȃ��Ȃ�̂ŁA
	//AI�̍X�V
	if (mField->mBallHoldChara != nullptr) {
		mAI->ClearAI();
		return;
	}

	//���g���{�[���ƏՓ˂����ꍇ
	auto lHitBall = UtlCollisionBall();
	//������΃{�[�������
	if (lHitBall) {
		GetStatus()->mBall = lHitBall;
		mField->SetBallHolder(this);
		//AI��i�߂�
		mAI->NextAI();
	}



}

void Enemy::BetaMoveToGoal()
{
	/*
		�z��ł����
		�G�Ƃ̑���
	*/

	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eMoveToGoal) {
		printf("Task:%s\n", "MoveToGoal");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eMoveToGoal;

		//�ړ�����S�[���ɂ���
		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(mField->GetTeamAlly(this)->mGoalIndex);

	}

	//�ړ����ɓG�Ƒ��������ꍇ�A���݂�AI��j������
	auto lLookTarget = IsCulling();
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		//�Ǐ]�Ώۂ��L�����N�^�[�Ɉڂ�
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
	//�B���
	//�B��铮�쒆�Ƀ{�[����������Ή������
	//���t���[�����C�`�F�b�N���s��
	//�Ώۂ����񂾏ꍇ�A�B���K�v�͂Ȃ��Ȃ�̂ŉB��鏈������



	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eMoveToHide) {

		printf("Task:%s\n", "MoveToHide");

		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eMoveToHide;
		//���g�̍��W���ꎞ�ޔ�
		DXVector3 lTempPosition;
		GetWorld()->GetMatrix().lock()->GetT(lTempPosition);
		//�߂����Ƀm�[�h���擾
		decltype(auto) lNearNodeList = mAI->GetNearNodeList(lTempPosition);
		//���g�����ԋ߂��m�[�h�����C���ˈʒu�Ƃ���
		mAI->SetStartNode(lNearNodeList[0]->GetID());


		//���C���ˈʒu��ݒ�(�^�[�Q�b�g���W)
		mRayPick->SetFramePosition(*GetStatus<EnemyStatus>()->mTargetChara->GetTransform());

		DXVector3 lTmpVec3;
		Dijkstra::Node* lGoalNodePtr{};
		for (auto& lNode : lNearNodeList) {
			//�m�[�h�Ɉړ������Ɖ��肵�ď���
			GetWorld()->SetT(lNode->Position);



			//�ǂꂩ��ł��q�b�g���Ă���ƉB�ꂽ����
			for (auto&lCollision : mCollisionTargets) {

				auto lTmpMesh = lCollision->mTransform;
				DXVector3 lTmpS;
				//���胁�b�V�����R���W�����p�ɕύX
				lCollision->GetWorld()->GetMatrix().lock()->GetT(lTmpS);
				lCollision->mCollisionMesh->GetWorld().lock()->SetT(lTmpS);
				lCollision->mCollisionMesh->GetWorld().lock()->SetRC(*lCollision->GetWorld()->mRotationCenter);


				//�R���W�����X�P�[�����ʂŐݒ肳��Ă��Ȃ��ꍇ��
				//���b�V���̃R���W�����X�P�[�����g��
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
					//�S�[���m�[�h�̊m��
					lGoalNodePtr = lNode;
				}
				lCollision->mTransform = lTmpMesh;

			}
			if (lGoalNodePtr) {
				break;
			}
		}

		//�B��邱�Ƃ��ł��Ȃ��ꍇ�A��ԉ����̃m�[�h�Ɉړ�����
		lGoalNodePtr = lGoalNodePtr ? lGoalNodePtr : lNearNodeList.back();
		//�ʒu�����ɖ߂�
		GetWorld()->SetT(lTempPosition);
		//�ړ����[�g�̊m��
		mAI->CreateRoot(lGoalNodePtr->GetID());
		//�Ō�Ƀ^�[�Q�b�g�������ʒu���L������
		GetStatus<EnemyStatus>()->mTargetChara->GetTransform()->GetWorld().lock()->GetMatrix().lock()->GetT(GetStatus<EnemyStatus>()->mLastLookPosiion);
		//�B��邽�߁A�K�R�I�Ɏ��E���猩����
		GetStatus<EnemyStatus>()->mTargetting = false;


	}

	//�B���Œ��Ƀ^�[�Q�b�g�����񂾏ꍇ�A�B���K�v���Ȃ��Ȃ�̂�
	//���̏�Ń`���[�W���J�n����
	if (GetStatus()->mTargetChara == nullptr) {
		mAI->NextAI();
		return;
	}

	bool lMoveNow = MoveNode();
	//�ړ����I���΃`���[�W
	if (!lMoveNow) {
		mAI->NextAI();
	}


	//���t���[�����C������s���A
	//�G�͈͓̔�����������瑦�`���[�W
	for (decltype(auto)lCollision : mCollisionTargets) {
		
		auto lTmpMesh = lCollision->mTransform;
		DXVector3 lTmpS;
		//���胁�b�V�����R���W�����p�ɕύX
		lCollision->GetWorld()->GetMatrix().lock()->GetT(lTmpS);
		lCollision->mCollisionMesh->GetWorld().lock()->SetT(lTmpS);
		lCollision->mCollisionMesh->GetWorld().lock()->SetRC(*lCollision->GetWorld()->mRotationCenter);


		//�R���W�����X�P�[�����ʂŐݒ肳��Ă��Ȃ��ꍇ��
		//���b�V���̃R���W�����X�P�[�����g��
		if (lCollision->mIsCollisionScaleDefault == true) {
			lCollision->GetWorld()->GetMatrix().lock()->GetS(lTmpS);
			lCollision->mCollisionMesh->GetWorld().lock()->SetS(lTmpS);
		}

		lCollision->mTransform = lCollision->mCollisionMesh;

		if (lCollision->IsActive() == false) {
			lCollision->mTransform = lTmpMesh;
			continue;
		}

		//Ray�ɂ̓G�l�~�[���W�������Ă���
		DXVector3 lTmpVec3;
		if (mRayPick->Collision(lTmpVec3, *GetStatus<EnemyStatus>()->mTargetChara->mTransform, *lCollision->GetTransform())) {
			//���������ꍇ�A�B��Ă���̂Ŏ���AI�Ɉڍs
			mAI->NextAI();
		}

		lCollision->mTransform = lTmpMesh;

	}
	



	//�r���{�[���ɓ��������ꍇ�A�������
	auto lHitBall = UtlCollisionBall();
	if (lHitBall) {
		GetStatus()->mBall = lHitBall;
		mField->SetBallHolder(this);
		//�����AI��i�߂�
		mAI->NextAI();
	}

}

void Enemy::BetaChargeEnergy()
{
	//�z��ł��鎖��
	//�G�Ƃ̑���
	//�{�[���ƐڐG

	//������
	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eChargeEnergy) {
		printf("Task:%s\n", "ChargeEnergy");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eChargeEnergy;
	}

	//�ړ����ɓG�𔭌������ꍇ�A���ȏ�G�l���M�[�����܂��Ă����AI��j������
	auto lLookTarget = IsCulling();
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {

		if (GetStatus<EnemyStatus>()->mEnergy.GetNowPer() >= 0.6f) {
			GetStatus<EnemyStatus>()->mTargetting = true;
			GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
			mAI->ClearAI();
			return;
		}

	}

	//�G�l���M�[�`���[�W
	if (GetStatus<EnemyStatus>()->mEnergy.GetNowPer() < 1.0f) {
		GetStatus<EnemyStatus>()->mEnergy.AddPer(0.01f);
	}
	//�`���[�W������������AI�ڍs
	else {
		mAI->NextAI();
	}

	//�r���{�[���ɓ��������ꍇ�A�������
	auto lHitBall = UtlCollisionBall();
	if (lHitBall) {
		GetStatus()->mBall = lHitBall;
		mField->SetBallHolder(this);
		//�����AI��i�߂�
		mAI->ClearAI();
	}

}

void Enemy::BetaInSightAttack()
{
	/*
		�z��ł����
		�G��������
		�G���|���
		�{�[���ƐڐG
	*/

	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eInSightAttack) {
		printf("Task:%s\n", "InSightAttack");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eInSightAttack;
	}

	//�^�[�Q�b�g���|�ꂽ�ꍇ�AAI�N���A
	{
		CharacterBase* lTargetChara = static_cast<CharacterBase*>(GetStatus<EnemyStatus>()->mTargetChara);
		if (lTargetChara->GetStatus()->mLive != CharaStateFlag::ALIVE) {
			//�^�[�Q�e�B���O������������
			GetStatus<EnemyStatus>()->mTargetting = false;
			mAI->ClearAI();
			return;
		}
	}

	//�^�[�Q�b�g�z�[�~���O����
	//�^�[�Q�b�g�̕���������
	float lRotateY = MSHormingY(*mTransform, *GetStatus<EnemyStatus>()->mTargetChara->GetTransform(), 3.0f);
	if (IsZero(lRotateY, 0.25f)) {
		//�U���������΍U��
		//�e�̔���
		ChangeStates::BulletShot(mBullets, this, mBulletNormal.get());
		//�G�l���M�[���Ȃ��Ȃ����ꍇ�A
		//����AI�ֈڍs����
		if (!ChangeStates::IsAttackDo(this, mBulletNormal.get())) {
			mAI->NextAI();
		}
	}
	else {
		//�U���������
		GetWorld()->AddRC(0, lRotateY, 0);
	}

	////�G�����E�ɂ��邩���t���[�����ׂ�
	auto lLookTarget = IsCulling();
	//if (dynamic_cast<CharacterBase*>(lLookTarget)) {



	//}


	//�r���{�[���ɓ��������ꍇ�A�������
	auto lHitBall = UtlCollisionBall();
	if (lHitBall) {
		GetStatus()->mBall = lHitBall;
		mField->SetBallHolder(this);
		//�����AI��i�߂�
		mAI->ClearAI();
	}

}

void Enemy::BetaMoveToBallHoldAlly()
{
	/*
		�z��ł����

		�{�[���������Ă��閡�����|�����
		�G�𔭌�
		�{�[���ƐڐG
	*/
	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eMoveToBallHoldAlly) {
		printf("Task:%s\n", "MoveToBallHoldAlly");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eMoveToBallHoldAlly;

		//�ړ�����{�[�������҂ɂ���

		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();

		DXVector3& lPosition = *mField->mBallHoldChara->GetWorld()->mPosition;
		auto lNearNode = mAI->GetNearNodeList(lPosition)[0];

		mAI->CreateRoot(lNearNode->GetID());

		GetStatus<EnemyStatus>()->mAllyComplianceCount = 0;
	}

	//�{�[�������������������ꂽ�ꍇ(���Ƃ����ꍇ�A
	//AI�N���A
	if (mField->mBallHoldChara == nullptr) {
		mAI->ClearAI();
	}



	//60�t���[�����ɃS�[���X�V
	if (!MoveNode()||GetStatus<EnemyStatus>()->mAllyComplianceCount >= 60) {
		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();

		DXVector3& lPosition = *mField->mBallHoldChara->GetWorld()->mPosition;
		auto lNearNode = mAI->GetNearNodeList(lPosition)[0];

		mAI->CreateRoot(lNearNode->GetID());

		GetStatus<EnemyStatus>()->mAllyComplianceCount = 0;
	}

	//�ړ����ɓG�𔭌������ꍇ�A���݂�AI��j������
	auto lLookTarget = IsCulling();
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		//�G��������
		GetStatus<EnemyStatus>()->mTargetting = true;
		GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
		mAI->ClearAI();
	}


	//�r���{�[���ɓ��������ꍇ�A�������
	auto lHitBall = UtlCollisionBall();
	if (lHitBall) {
		GetStatus()->mBall = lHitBall;
		mField->SetBallHolder(this);
		//�����AI��i�߂�
		mAI->ClearAI();
	}

	++GetStatus<EnemyStatus>()->mAllyComplianceCount;



}

void Enemy::BetaSearchEnemyShort()
{
	//�z��ł����
	/*
		�����Ɨ����
		�{�[�����t�B�[���h�ɗ�����
		�G�Ƃ̑���
	*/

	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eSerachEnemyAll) {
		printf("Task:%s\n", "SearchEnemyShort");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eSerachEnemyAll;
		GetStatus<EnemyStatus>()->mAllyComplianceCount = 0;

		//�ړ����K���ȏꏊ�ɂ���
		mAI->ClearRoot();
	}

	if (mAI->IsRootEmpty() == true) {
		//���[�g�𐶐�����
		mAI->CreateNextRoot(false);
	}
	//�ړ��Ɏ��s�����AI�X�V
	if (!MoveNode()) {
		mAI->ClearAI();
	}

	//�G�Ƃ̑���
	auto lLookTarget = IsCulling();
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		//����������V�[�P���X�������߂�
		mAI->NextAI();
		//�G�����F��
		GetStatus<EnemyStatus>()->mTargetting = true;
		//�������^�[�Q�b�g���L��
		GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
		return;
	}


	//�{�[�������҂����Ȃ��Ȃ����ꍇ
	if (mField->mBallHoldChara == nullptr) {
		mAI->ClearAI();
	}


//	//60�t���[������(���ݖ�����)
	//���t���[��
	//�����Ƃ̋����v�Z
	//��苗�����ꂽ��AI�N���A(�X�V)
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
		�z��ł����
		//�N�����{�[�����擾
		�G�𑨂���
	*/

	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eSerachEnemyAll) {
		printf("Task:%s\n", "SearchEnemyAll");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eSerachEnemyAll;
	}

	//�����_���ړ�����
	{
		if (mAI->IsRootEmpty() == true) {
			//���[�g�𐶐�����
			mAI->CreateNextRoot(false);
		}
		//�ړ��Ɏ��s�����AI�X�V
		if (!MoveNode()) {
			mAI->ClearAI();
		}

	}

	auto lLookTarget = IsCulling();
	if (lLookTarget) {
		//�{�[���ȊO�̏ꍇ
		if (!dynamic_cast<Ball*>(lLookTarget)) {
			//����������V�[�P���X�������߂�
			mAI->NextAI();
			//�G�����F��
			GetStatus<EnemyStatus>()->mTargetting = true;
			//�������^�[�Q�b�g���L��
			GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
			return;
		}
	}


}

void Enemy::BetaMoveToLookingTarget()
{
	/*
		�z��ł����
		�������{�[�������
		�G���{�[����߂�
		�G�Ƃ̑���
		�{�[�������҂����S
	*/

	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eSerachEnemyAll) {
		printf("Task:%s\n", "MoveToLookingTarget");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eSerachEnemyAll;
	}

	//�Ō�ɓG�������ʒu���S�[���Ƃ���
	if (mAI->IsRootEmpty() == true) {
		//�Ō�ɓG�������ʒu�����ԋ߂��m�[�h���S�[���Ƃ���
		auto lGoalID=mAI->GetNearNodeList(GetStatus<EnemyStatus>()->mLastLookPosiion)[0]->GetID();
	
		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(lGoalID);
	
	}
	
	//�߂�Œ��Ɏ��E�ɓ���Δ����B���̃^�[�Q�b�g�����b�N�I������
	auto lLookTarget = IsCulling();
	if (lLookTarget) {
		//�{�[���ȊO�̏ꍇ
		if (!dynamic_cast<Ball*>(lLookTarget)) {
			//����������V�[�P���X�������߂�
			mAI->NextAI();
			//�G�����F��
			GetStatus<EnemyStatus>()->mTargetting = true;
			//�������^�[�Q�b�g���L��
			GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
			return;
		}
	}
	
	
	if (!MoveNode()) {
		//�ړ��������Ă��܂����ꍇ�A�����ł��Ȃ������̂ŁA���b�N�I���t���O(�^�[�Q�b�g��null�N���A)���O����
		//����AI��
		mAI->ClearAI();
			
		GetStatus<EnemyStatus>()->mTargetChara = nullptr;
	}
}

void Enemy::BetaMoveToBallTarget()
{
	//�z��\�ȏ�
	/*
		�G�Ƃ̑���
		�{�[����������
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
	//�L�����N�^�[�ł���΃��b�N�I��
	//�L�����N�^�[�ł���΃��b�N�I��
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		GetStatus<EnemyStatus>()->mTargetting = true;
		GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
		mAI->ClearAI();
		return;
	}
	
	bool lMoveEnd = MoveNode();
	//�{�[���𗎂Ƃ���(�t�B�[���h�ɏo�������ꍇ)AI�N���A
	if (mField->mBallHoldChara == nullptr) {
		mAI->ClearAI();
		return;
	}

	//�ړ����I������ꍇ(�G�𔭌��ł��Ȃ������ꍇ)
	if (!lMoveEnd) {
		//�ēx�G��T��
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
		�z��ł����

		�G�Ƃ̑���
	*/
	
	if (GetStatus<EnemyStatus>()->mAIType != EnemyAIType::eSearchForAllyArea) {
		printf("Task:%s\n", "SearchForAllyArea");
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eSearchForAllyArea;

		GetStatus<EnemyStatus>()->mAllyComplianceCount = 0;

		//����`�[�����擾
		auto lEnemyTeam = mField->GetTeamEnemy(this);
		

		//�����G���A�̂ǂ������S�[���Ƃ���
		MyNode*lGoalNode;
		//�����`�[���̃`�[���^�C�v���擾����
		eTeamType lEnemyTeamType = lEnemyTeam->mTeamType;
		do {

			lGoalNode = mAI->GetNodeRandom();
			//�قȂ��Ă���Έړ��\�ȃm�[�h
		} while (lGoalNode->mTeamType != lEnemyTeamType);
		//auto lGoalID = mAI->GetNearNodeList(*mField->mBallHoldChara->mTransform->GetWorld().lock()->mPosition)[0]->GetID();

		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(lGoalNode->GetID());

	}

	//�{�[�������������������ꂽ�ꍇ(���Ƃ����ꍇ�A
	//AI�N���A
	if (mField->mBallHoldChara == nullptr) {
		mAI->ClearAI();
	}

	//�ړ��ł��Ȃ��Ȃ����烋�[�g�č\�z����
	if (!MoveNode()) {
		GetStatus<EnemyStatus>()->mAIType = EnemyAIType::eUnKnown;
		return;
	}
	//�ړ����ɓG�𔭌������ꍇ�A���݂�AI��j������
	auto lLookTarget = IsCulling();
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		//�G��������
		GetStatus<EnemyStatus>()->mTargetting = true;
		GetStatus<EnemyStatus>()->mTargetChara = lLookTarget;
		mAI->ClearAI();
	}
}
