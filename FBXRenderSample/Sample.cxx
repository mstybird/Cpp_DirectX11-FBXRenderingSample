#include"MSwrap.h"

namespace Collisions {

	//カメラからレイを飛ばす
	void RayFromCamera() {

		MSCollisionRay lRay;
		DX11RenderResource lEye, lTarget;
		MSFbxManager lTargetModel;
		//レイを飛ばすカメラをセット(カメラが存在するリソースを使うこと)
		lRay.SetRay(*lEye.GetCamera().lock());
		if (MSCollisionRay::Collision(lRay, lEye, lTarget, lTargetModel)) {
			//ヒット時の処理
		}
	}

	//レイピッキング
	//物体の座標移動を含むのでカメラの設定はこの後に行うこと
	void RayPicking() {
		//直前フレームの座標が必要なので使いまわすように

		MSCollisionRayPicking lRay;
		DX11RenderResource lEye, lTarget;
		MSFbxManager lTargetModel;
		DXVector3 lResult;
		lRay.SetSlipFlag(true);
		//最初のフレームを設定
		lRay.SetFramePosition(lEye);
		//lEye(座標)更新処理
		if (lRay.Collision(lResult, lEye, lTarget, lTargetModel)) {
			//ヒット時の処理
			//大抵は押し戻す処理など
		}
		//フレームの更新
		lRay.SetFramePosition(lEye);



	}

	//境界球判定
	void Sphere() {
		DX11RenderResource lEye, lTarget;
		MSFbxManager lEyeModel,lTargetModel;
		lEyeModel.CreateCollisionSphere();
		lTargetModel.CreateCollisionSphere();

		//現時点ではスマートポインタでなければならない
		lEyeModel.RegisterCollision(lEye);
		lTargetModel.RegisterCollision(lTarget);
		if (lEye.CollisionSphere(lTarget)) {
			//ヒット時の処理
		}
	}

	//視錐台
	void CullingFrustum() {
		MSCullingFrustum lFrustum;
		lFu
	}

}