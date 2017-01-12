#include"BulletNormal.h"
#include "NBullet.h"
#include"My3DShader.h"
#include"ChangeStates.hxx"
#include"CharacterBase.h"
#include<statusbulletbase.h>
#include<DX11RenderResource.h>
#include"StatusField.h"
#include"MS3DRender.h"
#include"MyScene.h"

BulletNormal::~BulletNormal() {};

void BulletNormal::Create(std::vector<std::unique_ptr<BulletObjectBase>>&aOutBulletList, CharacterBase* aShoter) {
	printf("Create %s, From %s\n", typeid(aShoter).name(), typeid(*this).name());
	std::unique_ptr<BulletNormal>bullet = std::make_unique<BulletNormal>();
	//	bullet->mShader = mShader;
	//�V�F�[�_�̐ݒ�
	bullet->SetShader(mShader);
	//���\�[�X�̏�����
	bullet->Initialize();
	bullet->InitStatus(&mStatus);

	MS3DRender* lRender;
	aShoter->GetRenderer(lRender);
	bullet->SetRenderer(lRender);

	//�����ʒu�ݒ�
	auto& lShotPos = *aShoter->GetWorld()->GetMatrix();
	//�e���˕����̊m��
	
	D3DXVec3TransformNormal(&bullet->GetStatus()->mDirection, &D3DXVECTOR3(0, 0, 1), &lShotPos);

	bullet->GetStatus()->mDirection.Normalize();
	bullet->SetMesh(*GetMesh());
	bullet->SetCollisionMesh(*GetMesh());

	//���ˑ��x�̐ݒ�
	bullet->GetStatus()->mVelocity = GetStatus()->mVelocity;
	//�g�����X�t�H�[���̐ݒ�

	DXVector3 lSpwanPos;
	DXVector3 lScaling;

	lShotPos.GetT(lSpwanPos);
	bullet->GetWorld()->SetT(lSpwanPos);
	lScaling = GetWorld()->mScale;
	bullet->GetWorld()->SetS(lScaling);

	//�ՓˑΏۂ�o�^
	//�ǂ�o�^
	for (auto& lCollision : *aShoter->GetCollisionTargets()) {
		bullet->AddCollisionTarget(lCollision);
	}



	//�{���Ώ�(�^�[�Q�b�g��o�^)
	for (auto&lTarget : *aShoter->GetSearchTargets()) {
		bullet->AddCollisionTarget(lTarget);
	}
	//��Փ˃I�u�W�F�N�g�̒ǉ�
	bullet->AddNoCollisionObject(aShoter->GetField()->mBall);

	//���C�s�b�N�̏�����
	bullet->GetRayPick()->SetFramePosition(*bullet->GetTransform());

	//���ˎ�̓o�^
	bullet->SetParentChara(aShoter);
	bullet->SetEffectFirst(this->mFirstEffect);
	bullet->SetEffectShot(this->mShotEffect);
	bullet->SetEffectHit(this->mHitEffect);
	bullet->SetEffectKill(this->mKillEffect);


	bullet->GetEffectShot()->SetPosition({ lSpwanPos.x,lSpwanPos.y,lSpwanPos.z });
	DXVector3 lRotate = aShoter->GetWorld()->mRotationCenter;
	bullet->GetEffectShot()->Play();
	bullet->GetEffectShot()->SetRotation({
		lRotate.x,
		lRotate.y,
		lRotate.z
	});
	aOutBulletList.push_back(std::move(bullet));


}

void BulletNormal::ShotFirstEffect(CharacterBase* aShoter) {
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

void BulletNormal::UpdateMove()
{
	GetWorld()->AddT(this->mStatus.mDirection*this->mStatus.mVelocity);
	GetWorld()->AddRC(0.0f, 3.0f, 0.0f);
}






