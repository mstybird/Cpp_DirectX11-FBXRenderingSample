#include "ChangeStates.hxx"
#include"CharacterBase.h"
#include"StatusBase.h"
#include"NBullet.h"
#include"StatusBulletBase.h"
#include"BulletManager.h"
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

	bool IsAttackDo(CharacterBase * lAttackChara, BulletManager*aBulletManager)
	{
		//アクティブなバレットを取得
		auto lBullet = aBulletManager->GetActiveBullet(lAttackChara);

		//キャラクタの所持エネルギーを計算
		auto lStatus = lAttackChara->GetStatus();
		auto lHoldEnergy = lStatus->mEnergy;
		//バレットのコストを取得
		auto lBulletCost = lBullet->GetStatus()->mCost;
		//所持エネルギーがバレットコスト以上であれば発射可能
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
		aAttakcer->SetBulletShotInterval(10);
		aAttakcer->SetBulletShotWait(true);
	}

	bool BulletShot(std::vector<std::unique_ptr<NBullet>>& aBulletList, CharacterBase * aAttakcer, BulletManager*aBulletManager)
	{
		auto lBullet = aBulletManager->GetActiveBullet(aAttakcer);

		//エネルギーが足りなければfalseを返す
		if (!IsAttackDo(aAttakcer, aBulletManager))return false;

		auto lCharaStatus = aAttakcer->GetStatus();
		auto lBulletStatus = aBulletManager->GetActiveStatus(aAttakcer);
		//インターバルが残っていれば発射できない
		if (lBulletStatus->mInterval.GetMaxRemainPer() > 0.0f)return false;

		//エネルギーを減らす
		lCharaStatus->mEnergy.SubFix(static_cast<float>(lBulletStatus->mCost));
		//インターバルの初期化
		lBulletStatus->mInterval.SetNowPer(0.0f);
		//発射処理
		lBullet->Create(aBulletList, aAttakcer);

		return true;
	}

}

