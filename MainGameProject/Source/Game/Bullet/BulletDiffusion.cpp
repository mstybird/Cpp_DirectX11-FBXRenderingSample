#include "BulletDiffusion.h"
#include"StatusField.h"
BulletDiffusion::~BulletDiffusion()
{
}

void BulletDiffusion::Create(std::vector<std::unique_ptr<BulletObjectBase>>& aOutBulletList, CharacterBase * aShoter)
{
	//発射角度
	float lDegrees[] = { -45,0,45 };
	int lDegreeCount = 3;

	//初期位置
	auto& lShotPos = *aShoter->GetWorld()->GetMatrix();
	for (int i = 0; i < lDegreeCount; ++i) {
		//バレット生成
		std::unique_ptr<BulletDiffusion>lBullet = std::make_unique<BulletDiffusion>();
		//シェーダの設定
		lBullet->SetShader(mShader);
		//リソースの初期化
		lBullet->Initialize();
		lBullet->InitStatus(&mStatus);
		//レンダーの設定
		MS3DRender* lRender;
		aShoter->GetRenderer(lRender);
		lBullet->SetRenderer(lRender);

		//発射方向
		D3DXMATRIX lRotateMatrix;
		D3DXMatrixRotationY(&lRotateMatrix, D3DXToRadian(lDegrees[i]));
		D3DXVec3TransformNormal(&lBullet->GetStatus()->mDirection, &D3DXVECTOR3(0, 0, 1), &(lRotateMatrix*lShotPos));


		lBullet->GetStatus()->mDirection.Normalize();
		lBullet->SetMesh(*GetMesh());
		lBullet->SetCollisionMesh(*GetMesh());

		//発射速度の設定
		lBullet->GetStatus()->mVelocity = GetStatus()->mVelocity;
		//トランスフォームの設定

		DXVector3 lSpwanPos;
		DXVector3 lScaling;

		lShotPos.GetT(lSpwanPos);
		lBullet->GetWorld()->SetT(lSpwanPos);
		lScaling = GetWorld()->mScale;
		lBullet->GetWorld()->SetS(lScaling);


		//衝突対象を登録
		//壁を登録
		for (auto& lCollision : *aShoter->GetCollisionTargets()) {
			lBullet->AddCollisionTarget(lCollision);
		}



		//捜索対象(ターゲットを登録)
		for (auto&lTarget : *aShoter->GetSearchTargets()) {
			lBullet->AddCollisionTarget(lTarget);
		}
		//非衝突オブジェクトの追加
		lBullet->AddNoCollisionObject(aShoter->GetField()->mBall);

		//レイピックの初期化
		lBullet->GetRayPick()->SetFramePosition(*lBullet->GetTransform());

		//発射主の登録
		lBullet->SetParentChara(aShoter);
		lBullet->SetEffectShot(this->mShotEffect);
		lBullet->SetEffectHit(this->mHitEffect);
		lBullet->SetEffectKill(this->mKillEffect);


		lBullet->GetEffectShot()->SetPosition({ lSpwanPos.x,lSpwanPos.y,lSpwanPos.z });
		DXVector3 lRotate = aShoter->GetWorld()->mRotationCenter;
		lBullet->GetEffectShot()->Play();
		lBullet->GetEffectShot()->SetRotation({
			lRotate.x,
			lRotate.y,
			lRotate.z
		});
		aOutBulletList.push_back(std::move(lBullet));



	}

}

void BulletDiffusion::ShotFirstEffect(CharacterBase * aShoter)
{
	mFirstEffect = this->mHitEffect;

	DXMatrix mMatrix = *aShoter->GetWorld()->GetMatrix();
	DXVector3 lPosition;
	DXVector3 lOffset{ 0.0f,0.0f,1.0f };
	D3DXVec3TransformCoord(&lPosition, &lOffset, &mMatrix);

	mFirstEffect.SetPosition({ lPosition.x,lPosition.y,lPosition.z });
	DXVector3 lRotate = aShoter->GetWorld()->mRotationCenter;
	mFirstEffect.Play();
	mFirstEffect.SetRotation({
		lRotate.x,
		lRotate.y,
		lRotate.z
	});
}

void BulletDiffusion::UpdateMove()
{
	GetWorld()->AddT(this->mStatus.mDirection*this->mStatus.mVelocity);
	GetWorld()->AddRC(0.0f, 3.0f, 0.0f);
}
