#include "ChangeStates.hxx"
#include"CharacterBase.h"
#include"StatusBase.h"
#include"NBullet.h"
#include"StatusBulletBase.h"
#include"BulletManager.h"
#include"MyScene.h"
namespace ChangeStates {
	bool Damage(BulletObjectBase * aAttackObj, CharacterBase * aDamageChara)
	{
		//�_���[�W�����B
		//���񂾂�true��Ԃ�

		auto lBulletStatus = aAttackObj->GetStatus();
		auto lCharaStatus = aDamageChara->GetStatus();
		MSPRogressFlag lResultFlag;
		switch (aAttackObj->GetStatus()->mType)
		{
		case BulletDamageType::NORMAL:
			lResultFlag = lCharaStatus->mHp.SubFix(lBulletStatus->mAtk);
			break;
		case BulletDamageType::PERCENT:
			lResultFlag = lCharaStatus->mHp.SubPer(lBulletStatus->mAtk);
			break;
		case BulletDamageType::FIXED:
			lResultFlag = lCharaStatus->mHp.SubFix(lBulletStatus->mAtk);
			break;
		default:
			//�����Ȃ�
			break;
		}
		
		return lResultFlag==MSPRogressFlag::LESSMIN;
	}

	bool IsAttackDo(CharacterBase * lAttackChara, BulletManager*aBulletManager)
	{
		//�A�N�e�B�u�ȃo���b�g���擾
		auto lBullet = aBulletManager->GetActiveBullet(lAttackChara);

		//�L�����N�^�̏����G�l���M�[���v�Z
		auto lStatus = lAttackChara->GetStatus();
		auto lHoldEnergy = lStatus->mEnergy;
		//�o���b�g�̃R�X�g���擾
		auto lBulletCost = lBullet->GetStatus()->mCost;
		//�����G�l���M�[���o���b�g�R�X�g�ȏ�ł���Δ��ˉ\
		return lHoldEnergy.GetNow() >= lBulletCost;
	}

	bool IsBulletWaiting(CharacterBase * aAttakcer, BulletManager * aBulletManager)
	{
		auto lStatus = aBulletManager->GetActiveStatus(aAttakcer);

		return lStatus->mInterval.GetMaxRemainPer() > 0.0f;
	}

	void FirstBulletProc(CharacterBase * aAttakcer, BulletManager * aBulletManager)
	{
		auto lBullet = aBulletManager->GetActiveBullet(aAttakcer);
		lBullet->ShotFirstEffect(aAttakcer);
		aAttakcer->GetTransform()->GetMesh()->SetAnimation(ValueMyScene::Chara::cAnimSkill);
		aAttakcer->GetTransform()->GetMesh()->SetLoopFlag(false);
		aAttakcer->GetTransform()->GetMesh()->SetFrontFrame();
		aAttakcer->SetBulletShotInterval(40);
		aAttakcer->SetBulletShotWait(true);
	}

	bool BulletShot(std::vector<std::unique_ptr<BulletObjectBase>>& aBulletList, CharacterBase * aAttakcer, BulletManager*aBulletManager)
	{
		auto lBullet = aBulletManager->GetActiveBullet(aAttakcer);

		//�G�l���M�[������Ȃ����false��Ԃ�
		if (!IsAttackDo(aAttakcer, aBulletManager))return false;

		auto lCharaStatus = aAttakcer->GetStatus();
		auto lBulletStatus = aBulletManager->GetActiveStatus(aAttakcer);
		//�C���^�[�o�����c���Ă���Δ��˂ł��Ȃ�
		if (lBulletStatus->mInterval.GetMaxRemainPer() > 0.0f)return false;

		//�G�l���M�[�����炷
		lCharaStatus->mEnergy.SubFix(static_cast<float>(lBulletStatus->mCost));
		//�C���^�[�o���̏�����
		lBulletStatus->mInterval.SetNowPer(0.0f);
		//���ˏ���
		lBullet->Create(aBulletList, aAttakcer);

		return true;
	}

}

