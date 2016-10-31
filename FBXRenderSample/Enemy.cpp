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
	
	mRayPick->SetFramePosition(*mTransform);
	//��ԋ߂��m�[�h���������W�Ƃ��Ă���
	DXVector3 lPosition;
	GetWorld()->GetMatrix().lock()->GetT(lPosition);
	auto lNearNode = mAI->GetNearNodeList(lPosition)[0];
	mAI->SetStartNode(lNearNode->GetID());
}
void Enemy::SetAI(NcgLuaManager * aAI)
{
	mAI->mLuaAI = aAI;
	mAI->mLuaAI->SetFunction("GetPlan", 1, 6);	//�֐��̐ݒ�
}

void Enemy::SetGoalIndex(int aIndex)
{
	
	GetStatus<EnemyStatus>()->mGoalIndex = aIndex;
}


void Enemy::Update()
{
	UpdateAlive();
	//UpdateMesh();
	UpdateBullets();

	switch (mAI->GetNowAI())
	{
	case EnemyAIType::eUnKnown:
		UpdateAI();//AI���N�����Ă��Ȃ�(��~���Ă���)�ꍇ�AAI�X�V
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
		//�X�|�[��
		InitStatus();
		Respawn();

		break;
	default:
		break;
	}

}

void Enemy::InitStatus()
{
	auto lStatus = GetStatus<EnemyStatus>();

	lStatus->mTargetting = false;
	lStatus->mBall = nullptr;
	lStatus->mEnergy.Set(5.0f,0.0f,2.0f);
	lStatus->mHp.Set(10.0f, .0f, 10.0f);
	lStatus->mInitMoveToBall = false;
	lStatus->mInitMoveToBallTarget = false;
	lStatus->mInitMoveToGoal = false;
	lStatus->mIsTargetingBall = false;
	lStatus->mLive = CharaStateFlag::ALIVE;
	lStatus->mTarget = nullptr;
	lStatus->mTargetting = false;
}

void Enemy::UpdateAI()
{
	mAI->ClearRoot();

	auto lStatus = GetStatus<EnemyStatus>();
	auto lBulletStatus = mBulletNormal->GetStatus();

	mAI->Update(
		lStatus->mTargetting,
		lStatus->mTarget!=nullptr,
		lStatus->mEnergy.GetNow() > lBulletStatus->mCost,
		mField->mBallHoldChara!=nullptr,
		lStatus->mBall!=nullptr,
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
	���͈�x�B�ꂽ����(�����������Ƃɏ��������)
	�B�ꂽ���Ƃ͈ړ������S�ɏI����Ă���̂ŁA�ړ����[�g�͋��ԁB
	�K�����Ԃł��邱�Ƃ𗘗p���āA��̂Ƃ��Ɉړ����[�g������������
*/
void Enemy::UpdateMoveToTarget()
{
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
			GetStatus<EnemyStatus>()->mTarget = lLookTarget;
			return;
		}
	}


	//�B����ʒu�܂ňړ�
	if (!MoveNode()) {
		//�ړ��������Ă��܂����ꍇ�A�����ł��Ȃ������̂ŁA���b�N�I���t���O(�^�[�Q�b�g��null�N���A)���O����
		//����AI��
		mAI->ClearAI();
		
		GetStatus<EnemyStatus>()->mTarget = nullptr;
	}

}


/*
	�ŏ��Ɉړ��悪���肵�Ă��邩���ׂ�B
	���肵�Ă��Ȃ���΁A���Ɉړ�����m�[�h�̌���B(��߂�͂��Ȃ�)

	�ړ������B�ړ���m�[�h�Ɍ������Ĉړ�����
	���̈ړ��̍ہA���t���[���^�[�Q�b�g�{������B
	�^�[�Q�b�g���m���Ɏ��E�ɑ��������A
	���̃��[�`���ɐi�ށB
*/
void Enemy::UpdateSearching()
{
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


	//�o�^���Ă��鑖���^�[�Q�b�g�S�ĂŃ��[�v������
	auto lLookTarget = IsCulling();
	if (lLookTarget) {
		//����������V�[�P���X�������߂�
		mAI->NextAI();
		//�G�����F��
		GetStatus<EnemyStatus>()->mTargetting = true;
		//�������^�[�Q�b�g���L��
		GetStatus<EnemyStatus>()->mTarget = lLookTarget;				
	}







}

//�G�l���M�[�̃`���[�W
//�`���[�W��AI�ڍs
void Enemy::UpdateChargeEnergy()
{
	mAI->NextAI();
	GetStatus<EnemyStatus>()->mEnergy.SetNowPer(1.0f);
}

//�G����B��邽�߁A�߂����Ƀm�[�h���擾����
//�e�m�[�h���烌�C�𔭎˂��A���C��������Ȃ��m�[�h���S�[���Ƃ���
void Enemy::UpdateMovingHide()
{
	//���̃t���[�ɗ���ꍇ�͕K��AI��������Ƃ���
	if (mAI->IsRootEmpty() == true) {
		//�������g�̍��W
		DXVector3 lPosition;
		GetWorld()->GetMatrix().lock()->GetT(lPosition);
		auto lNearNodeList = mAI->GetNearNodeList(lPosition);

		//���C���ˈʒu��ݒ�(�^�[�Q�b�g���W)
		mRayPick->SetFramePosition(*GetStatus<EnemyStatus>()->mTarget->GetTransform());

		//��ԋ߂��m�[�h�����C���ˈʒu�Ƃ���
		mAI->SetStartNode(lNearNodeList[0]->GetID());
		//�^�[�Q�b�g����e�m�[�h�̍��W���烌�C���΂��ăq�b�g����Ό��������܂܂Ȃ̂ŃA�E�g
		DXVector3 lTmpVec3;
		Dijkstra::Node* lGoalNodePtr{};
		for (auto& lNode : lNearNodeList) {
			//�m�[�h�Ɉړ������Ɖ��肵�ď���
			GetWorld()->SetT(lNode->Position);

			//�ǂꂩ��ł��q�b�g���Ă���ƉB�ꂽ����
			for (auto&lCollision : mCollisionTargets) {
				if (mRayPick->Collision(lTmpVec3, *mTransform, *lCollision->GetTransform())) {
					//�S�[���m�[�h�̊m��
					lGoalNodePtr = lNode;
				}
			}
			if (lGoalNodePtr) {
				break;
			}
		}
		//�B��邱�Ƃ��ł��Ȃ��ꍇ�A��ԉ����̃m�[�h�Ɉړ�����
		lGoalNodePtr = lGoalNodePtr ? lGoalNodePtr : lNearNodeList.back();


		//�ʒu�����ɖ߂�
		GetWorld()->SetT(lPosition);
		//�ړ����[�g�̊m��
		mAI->CreateRoot(lGoalNodePtr->GetID());
		//�Ō�Ƀ^�[�Q�b�g�������ʒu���L������
		GetStatus<EnemyStatus>()->mTarget->GetTransform()->GetWorld().lock()->GetMatrix().lock()->GetT(GetStatus<EnemyStatus>()->mLastLookPosiion);
		//�B��邽�߁A�K�R�I�Ɏ��E���猩����
		GetStatus<EnemyStatus>()->mTargetting = false;
	}
	
	//�B����ʒu�܂ňړ�
	if (!MoveNode()) {
		//�ړ����������玟��AI��
		mAI->NextAI();
	}

}

void Enemy::UpdateEnergyShot()
{
	//�^�[�Q�b�g�̕���������
	float lRotateY = MSHormingY(*mTransform, *GetStatus<EnemyStatus>()->mTarget->GetTransform(), 3.0f);
	if (IsZero(lRotateY, 0.001f)) {
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

}

//�{�[������ɓ���Ă���̂ŃS�[���܂ňړ�����
void Enemy::UpdateMoveToGoal()
{
	if (GetStatus<EnemyStatus>()->mInitMoveToGoal == false) {
		GetStatus<EnemyStatus>()->mInitMoveToGoal = true;
		//�ړ�����S�[���ɂ���
		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(GetStatus<EnemyStatus>()->mGoalIndex);
	}

	//�ړ����ɓG�𔭌������ꍇ�A���݂�AI��j������
	auto lLookTarget = IsCulling();
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		GetStatus<EnemyStatus>()->mTargetting = true;
		GetStatus<EnemyStatus>()->mTarget = lLookTarget;
		mAI->ClearAI();
		GetStatus<EnemyStatus>()->mInitMoveToGoal = false;
	}

	//�S�[���ɓ���������{�[���������
	if (!MoveNode()) {
		mField->RespawnBall();
		GetStatus<EnemyStatus>()->mBall = nullptr;
		GetStatus<EnemyStatus>()->mInitMoveToGoal = false;
		//AI�ċN��
		mAI->ClearAI();
	}

}
//�{�[���̈ʒu�܂ňړ�����(�E���ɍs��)
void Enemy::UpdateMoveToBall()
{
	//�{�[���̈ʒu���擾����
	if (GetStatus<EnemyStatus>()->mInitMoveToBall == false) {
		GetStatus<EnemyStatus>()->mInitMoveToBall = true;
		DXVector3 lBallPos;
		mField->mBall->GetWorld()->GetMatrix().lock()->GetT(lBallPos);
		auto lGoalID = mAI->GetNearNodeList(lBallPos)[0]->GetID();
		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(lGoalID);
	}

	auto lLookTarget = IsCulling();

	//�L�����N�^�[�ł���΃��b�N�I��
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		//�Ǐ]�Ώۂ��L�����N�^�ɐ؂�ւ���̂ŁA
		//�{�[�������t���O������
		GetStatus<EnemyStatus>()->mIsTargetingBall = false;

		GetStatus<EnemyStatus>()->mTargetting = true;
		GetStatus<EnemyStatus>()->mTarget = lLookTarget;
		mAI->ClearAI();
		GetStatus<EnemyStatus>()->mInitMoveToBall = false;
		return;
	}

	if (GetStatus<EnemyStatus>()->mIsTargetingBall|| dynamic_cast<Ball*>(lLookTarget)) {
		//�{�[���������ꍇ�͒ǐՂ���
		//���E�ɓ������̂Ń{�[���𑨂��Ă���
		GetStatus<EnemyStatus>()->mIsTargetingBall = true;
		float lRotateY;
		lRotateY = MSHormingY(*mTransform, *mField->mBall->GetTransform(), 6.0f);
		GetWorld()->AddRC(0, lRotateY, 0);
		//�ړ�����
		if (IsZero(lRotateY, 0.1f)) {
			GetWorld()->AddT(DXWorld::TYPE_ROTATE, 0.1f, { 0,0,1 });
		}
	}
	else {
		//�{�[������Ȃ������ꍇ
		auto lChara = dynamic_cast<CharacterBase*>(lLookTarget);
		//�L�����N�^�[�ł���΃��b�N�I��
		if (lChara) {
			//�Ǐ]�Ώۂ��L�����N�^�ɐ؂�ւ���̂ŁA
			//�{�[�������t���O������
			GetStatus<EnemyStatus>()->mIsTargetingBall = false;

			GetStatus<EnemyStatus>()->mTargetting = true;
			GetStatus<EnemyStatus>()->mTarget = lChara;
			mAI->ClearAI();
			GetStatus<EnemyStatus>()->mInitMoveToBall = false;
		}
		else {
		//�ړ����I����ă{�[������Ɏ��Ȃ������ꍇ�A�ď�����
			if (!MoveNode()) {
				GetStatus<EnemyStatus>()->mInitMoveToBall = false;
				GetStatus<EnemyStatus>()->mIsTargetingBall = false;
			}

		}
	}

			//�N�����{�[����������ꍇ�AAI�N���A
			if (mField->mBallHoldChara != nullptr) {
				mAI->ClearAI();
				GetStatus<EnemyStatus>()->mInitMoveToBall = false;
				GetStatus<EnemyStatus>()->mIsTargetingBall = false;
			}
	//�{�[���ƏՓ˂��������ׂ�
	auto lHitTargets = UpdateCollision(false);
	for (auto&lHitTarget : lHitTargets) {
		if (lHitTarget) {
			//�{�[���ɓ��������ꍇ�A���̃{�[�����������
			Ball* lBall = dynamic_cast<Ball*>(lHitTarget);
			if (lBall) {
				GetStatus<EnemyStatus>()->mBall = lBall;
				mField->SetBallHolder(this);
				//�����AAI��i�߂�
				mAI->NextAI();
				GetStatus<EnemyStatus>()->mInitMoveToBall = false;
				GetStatus<EnemyStatus>()->mIsTargetingBall = false;
			}
		}
	}



}

void Enemy::UpdateMoveToBallTarget()
{
	//�{�[���������Ă��鑊��̈ʒu���擾����
	if (GetStatus<EnemyStatus>()->mInitMoveToBallTarget == false) {
		GetStatus<EnemyStatus>()->mInitMoveToBallTarget = true;
		DXVector3 lBallPos;
		mField->mBallHoldChara->GetWorld()->GetMatrix().lock()->GetT(lBallPos);
		auto lGoalID = mAI->GetNearNodeList(lBallPos)[0]->GetID();
		mAI->SetStartNode(mAI->GetNowNode()->GetID());
		mAI->GenerateRoot();
		mAI->CreateRoot(lGoalID);
	}

	auto lLookTarget = IsCulling();
	//�L�����N�^�[�ł���΃��b�N�I��
	if (dynamic_cast<CharacterBase*>(lLookTarget)) {
		GetStatus<EnemyStatus>()->mTargetting = true;
		GetStatus<EnemyStatus>()->mTarget = lLookTarget;
		mAI->ClearAI();
		GetStatus<EnemyStatus>()->mInitMoveToBallTarget = false;
		return;
	}

	bool lMoveEnd = MoveNode();
	//�{�[���𗎂Ƃ���(�t�B�[���h�ɏo�������ꍇ)AI�N���A
	if (mField->mBallHoldChara == nullptr) {
		mAI->ClearAI();
		GetStatus<EnemyStatus>()->mInitMoveToBallTarget = false;
		return;
	}
	//�ړ����I������ꍇ(�G�𔭌��ł��Ȃ������ꍇ)
	if (!lMoveEnd) {
		//�ēx�G��T��
		GetStatus<EnemyStatus>()->mInitMoveToBallTarget = false;
	}


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
	//���E����
	//MSCullingFrustum cf;
	for (auto&lTarget : mSearchTargets) {
		if (/*cf.IsCullingWorld(*mTransform, *lTarget)*/true) {
			if (MSCullingOcculusion::IsCullingWorld(
				*mRender, *mTransform, *lTarget->GetTransform(), 0.2f,
				[&,this]() {
				for (auto&lCollision : mCollisionTargets) {
					//�o�^�ς݃R���W�������J�����O�^�[�Q�b�g�Ɠ����������ꍇ�͏�Q���Ƃ��ĕ`�悵�Ȃ�
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

