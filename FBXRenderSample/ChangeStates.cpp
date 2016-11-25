#include "ChangeStates.hxx"
#include"CharacterBase.h"
#include"StatusBase.h"
#include"NBullet.h"
#include"StatusBulletBase.h"
namespace ChangeStates {
	bool Damage(NBullet * aAttackObj, CharacterBase * aDamageChara)
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

	bool IsAttackDo(CharacterBase * lAttackChara, NBullet * aBullet)
	{
		//�L�����N�^�̏����G�l���M�[���v�Z
		auto lStatus = lAttackChara->GetStatus();
		auto lHoldEnergy = lStatus->mEnergy;
		//�o���b�g�̃R�X�g���擾
		auto lBulletCost = aBullet->GetStatus()->mCost;
		//�����G�l���M�[���o���b�g�R�X�g�ȏ�ł���Δ��ˉ\
		return lHoldEnergy.GetNow() >= lBulletCost;
	}

	bool BulletShot(std::vector<std::unique_ptr<NBullet>>& aBulletList, CharacterBase * aAttakcer, NBullet * aBullet)
	{
		//�G�l���M�[������Ȃ����false��Ԃ�
		if (!IsAttackDo(aAttakcer, aBullet))return false;

		auto lCharaStatus = aAttakcer->GetStatus();
		auto lBulletStatus = aBullet->GetStatus();
		//�C���^�[�o�����c���Ă���Δ��˂ł��Ȃ�
		if (lBulletStatus->mInterval.GetMaxRemainPer() > 0.0f)return false;

		//�G�l���M�[�����炷
		lCharaStatus->mEnergy.SubFix(static_cast<float>(lBulletStatus->mCost));
		//�C���^�[�o���̏�����
		lBulletStatus->mInterval.SetNowPer(0.0f);
		//���ˏ���
		aBullet->Create(aBulletList, aAttakcer);

		return true;
	}

}

