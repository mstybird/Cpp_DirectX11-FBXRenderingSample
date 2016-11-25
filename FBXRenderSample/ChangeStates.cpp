#include "ChangeStates.hxx"
#include"CharacterBase.h"
#include"StatusBase.h"
#include"NBullet.h"
#include"StatusBulletBase.h"
namespace ChangeStates {
	bool Damage(NBullet * aAttackObj, CharacterBase * aDamageChara)
	{
		//ダメージ処理。
		//死んだらtrueを返す

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
			//処理なし
			break;
		}

		return lResultFlag==MSPRogressFlag::LESSMIN;
	}

	bool IsAttackDo(CharacterBase * lAttackChara, NBullet * aBullet)
	{
		//キャラクタの所持エネルギーを計算
		auto lStatus = lAttackChara->GetStatus();
		auto lHoldEnergy = lStatus->mEnergy;
		//バレットのコストを取得
		auto lBulletCost = aBullet->GetStatus()->mCost;
		//所持エネルギーがバレットコスト以上であれば発射可能
		return lHoldEnergy.GetNow() >= lBulletCost;
	}

	bool BulletShot(std::vector<std::unique_ptr<NBullet>>& aBulletList, CharacterBase * aAttakcer, NBullet * aBullet)
	{
		//エネルギーが足りなければfalseを返す
		if (!IsAttackDo(aAttakcer, aBullet))return false;

		auto lCharaStatus = aAttakcer->GetStatus();
		auto lBulletStatus = aBullet->GetStatus();
		//インターバルが残っていれば発射できない
		if (lBulletStatus->mInterval.GetMaxRemainPer() > 0.0f)return false;

		//エネルギーを減らす
		lCharaStatus->mEnergy.SubFix(static_cast<float>(lBulletStatus->mCost));
		//インターバルの初期化
		lBulletStatus->mInterval.SetNowPer(0.0f);
		//発射処理
		aBullet->Create(aBulletList, aAttakcer);

		return true;
	}

}

