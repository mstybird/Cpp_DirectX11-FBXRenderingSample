#include "BulletDiffusion.h"
#include"StatusField.h"
BulletDiffusion::~BulletDiffusion()
{
}

void BulletDiffusion::Create(std::vector<std::unique_ptr<BulletObjectBase>>& aOutBulletList, CharacterBase * aShoter)
{
	//���ˊp�x
	float lDegrees[] = { -45,0,45 };
	int lDegreeCount = 3;

	//�����ʒu
	auto& lShotPos = *aShoter->GetWorld()->GetMatrix();
	for (int i = 0; i < lDegreeCount; ++i) {
		//�o���b�g����
		std::unique_ptr<BulletDiffusion>lBullet = std::make_unique<BulletDiffusion>();
		//�V�F�[�_�̐ݒ�
		lBullet->SetShader(mShader);
		//���\�[�X�̏�����
		lBullet->Initialize();
		lBullet->InitStatus(&mStatus);
		//�����_�[�̐ݒ�
		MS3DRender* lRender;
		aShoter->GetRenderer(lRender);
		lBullet->SetRenderer(lRender);

		//���˕���
		D3DXMATRIX lRotateMatrix;
		D3DXMatrixRotationY(&lRotateMatrix, D3DXToRadian(lDegrees[i]));
		D3DXVec3TransformNormal(&lBullet->GetStatus()->mDirection, &D3DXVECTOR3(0, 0, 1), &(lRotateMatrix*lShotPos));


		lBullet->GetStatus()->mDirection.Normalize();
		lBullet->SetMesh(*GetMesh());
		lBullet->SetCollisionMesh(*GetMesh());

		//���ˑ��x�̐ݒ�
		lBullet->GetStatus()->mVelocity = GetStatus()->mVelocity;
		//�g�����X�t�H�[���̐ݒ�

		DXVector3 lSpwanPos;
		DXVector3 lScaling;

		lShotPos.GetT(lSpwanPos);
		lBullet->GetWorld()->SetT(lSpwanPos);
		lScaling = GetWorld()->mScale;
		lBullet->GetWorld()->SetS(lScaling);


		//�ՓˑΏۂ�o�^
		//�ǂ�o�^
		for (auto& lCollision : *aShoter->GetCollisionTargets()) {
			lBullet->AddCollisionTarget(lCollision);
		}



		//�{���Ώ�(�^�[�Q�b�g��o�^)
		for (auto&lTarget : *aShoter->GetSearchTargets()) {
			lBullet->AddCollisionTarget(lTarget);
		}
		//��Փ˃I�u�W�F�N�g�̒ǉ�
		lBullet->AddNoCollisionObject(aShoter->GetField()->mBall);

		//���C�s�b�N�̏�����
		lBullet->GetRayPick()->SetFramePosition(*lBullet->GetTransform());

		//���ˎ�̓o�^
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
