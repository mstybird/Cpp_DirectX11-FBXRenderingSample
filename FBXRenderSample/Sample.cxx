#include"MSwrap.h"

namespace Collisions {

	//�J�������烌�C���΂�
	void RayFromCamera() {

		MSCollisionRay lRay;
		DX11RenderResource lEye, lTarget;
		MSFbxManager lTargetModel;
		//���C���΂��J�������Z�b�g(�J���������݂��郊�\�[�X���g������)
		lRay.SetRay(*lEye.GetCamera().lock());
		if (MSCollisionRay::Collision(lRay, lEye, lTarget, lTargetModel)) {
			//�q�b�g���̏���
		}
	}

	//���C�s�b�L���O
	void RayPicking() {
		//���O�t���[���̍��W���K�v�Ȃ̂Ŏg���܂킷�悤��

		MSCollisionRayPicking lRay;
		DX11RenderResource lEye, lTarget;
		MSFbxManager lTargetModel;
		DXVector3 lResult;
		lRay.SetSlipFlag(true);
		//�ŏ��̃t���[����ݒ�
		lRay.SetFramePosition(lEye);

		if (lRay.Collision(lResult, lEye, lTarget, lTargetModel)) {
			//�q�b�g���̏���
			//���͉����߂������Ȃ�
		}
		//�t���[���̍X�V
		lRay.SetFramePosition(lEye);



	}

	//���E������
	void Sphere() {
		DX11RenderResource lEye, lTarget;
		MSFbxManager lEyeModel,lTargetModel;
		lEyeModel.CreateCollisionSphere();
		lTargetModel.CreateCollisionSphere();

		//�����_�ł̓X�}�[�g�|�C���^�łȂ���΂Ȃ�Ȃ�
		lEyeModel.RegisterCollision(lEye);
		lTargetModel.RegisterCollision(lTarget);
		if (lEye.CollisionSphere(lTarget)) {
			//�q�b�g���̏���
		}
	}

	//������
	void CullingFrustum() {
		MSCullingFrustum lFrustum;
		lFu
	}

}