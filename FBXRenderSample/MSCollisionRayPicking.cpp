#include "MSCollisionRayPicking.h"
#include"DXVector3.h"
#include"MSFbxManager.h"
#include"DX11FbxResource.h"
#include"DXVector3.h"
#include"DXMatrix.h"
#include"DXWorld.h"
#include"DXCamera.h"
#include"DX11RenderResource.h"
#include<array>
#include<vector>
#include<map>

using std::pair;


MSCollisionRayPicking::MSCollisionRayPicking() :
	mSlipFlag{ false },
	mPosBefore{ std::make_unique<DXVector3>() }
{
}

void MSCollisionRayPicking::SetFramePosition(DX11RenderResource & pRayPosion)
{
	//現在の座標情報を取得
	pRayPosion.GetWorld().lock()->GetMatrix().lock()->GetT(*mPosBefore);
}

void MSCollisionRayPicking::SetSlipFlag(bool pSlipFlag)
{
	mSlipFlag = pSlipFlag;
}

bool MSCollisionRayPicking::GetSlipFlag()
{
	return mSlipFlag;
}


bool MSCollisionRayPicking::Collision(DXVector3&pResultPosition, DX11RenderResource&pRayPosition, DX11RenderResource & pRayTarget)
{

	

	using FbxPolygon = std::array<FbxVertex, 3>;
	using FbxPtrPolygon = std::array<FbxVertex*, 3>;

	struct T_MeshPolygon {
		std::weak_ptr<FBXModelData> mesh;	//当たったメッシュ
		FbxPolygon polygon;					//ヒットしたポリゴン
		DXMatrix globalMatrix;				//ヒットしたメッシュに適用されている行列
	};

	//ヒットしたポリゴンを含むメッシュ等を格納する
	pair<float, T_MeshPolygon>lHitPolygon{ FLT_MAX,T_MeshPolygon() };
	bool lHit{ false };			//メッシュに当たった場合はtrue
	float lDistance{ 0.0f };	//当たった場合の距離
	DXVector3 lRayAfterPosition{};	//更新後の位置を取得する

									//レイ判定
	{
		FbxPolygon lPolygon;
		//交差した時の情報を受け取る変数
		DXVector3 lDirection;//変換後の位置、方向を格納する変数
		DXMatrix invMat;		//逆行列を格納する変数
								//リソースのワールド座標
		DXVector3 lPosition[3];		//レイ判定使う最終的な3頂点

									//ターゲットリソース上の行列を取得
		DXMatrix&lResourceMatrix = *pRayTarget.GetWorld().lock()->GetMatrix().lock();

		//更新後のリソースから位置を取得
		pRayPosition.GetWorld().lock()->GetMatrix().lock()->GetT(lRayAfterPosition);

		
		auto lMeshList = pRayTarget.mMesh->mMeshData;
		//移動方向にポリゴンがあるかチェック
		//ここでレイがヒットする一番近いポリゴンを取得する
		DXVector3 lTmpPosBefore = *mPosBefore;
		for (auto&lMesh : lMeshList) {
			
			*mPosBefore = lTmpPosBefore;
			//変換行列の合成
			//メッシュに定義されている行列を取得
			DXMatrix lGlobalMatrix{ *lMesh->mWorld.get() };
			//合成
			lGlobalMatrix = lGlobalMatrix * lResourceMatrix;
			//レイ方向を計算
			D3DXVec3Normalize(&lDirection, &(lRayAfterPosition - *mPosBefore));

			for (auto&lSubMesh : lMesh->subMesh) {
				for (int i = 0; i < lSubMesh->PolygonCount; i++) {
					//判定を行うポリゴンを取得
					FbxPtrPolygon lPtrPolygon;
					lSubMesh->GetPolygon(lPtrPolygon, i);

					//行列を使って頂点位置の修正
					DXMatrix lPositionMatrix[3];
					for (int i = 0; i < 3; ++i) {
						lPositionMatrix[i].Translation(lPtrPolygon[i]->Pos);
						lPositionMatrix[i] *= lGlobalMatrix;
						lPositionMatrix[i].GetT(lPosition[i]);
					}
					int a, b;



					//衝突判定を行う
					//レイ判定
					if (D3DXIntersectTri(
						&lPosition[0],
						&lPosition[1],
						&lPosition[2],
						&*mPosBefore,
						&lDirection,
						NULL,
						NULL,
						&lDistance
						)) {
						lHit = true;

						//現在のポリゴンより距離が近ればそのポリゴンで更新
						if (lHitPolygon.first > lDistance) {
							lHitPolygon.first = lDistance;
							lHitPolygon.second.mesh = lSubMesh;
							lHitPolygon.second.globalMatrix = lGlobalMatrix;
							lHitPolygon.second.polygon[0] = *lPtrPolygon[0];
							lHitPolygon.second.polygon[1] = *lPtrPolygon[1];
							lHitPolygon.second.polygon[2] = *lPtrPolygon[2];

							//頂点情報は行列によって変化しているため更新する
							lHitPolygon.second.polygon[0].Pos = lPosition[0];
							lHitPolygon.second.polygon[1].Pos = lPosition[1];
							lHitPolygon.second.polygon[2].Pos = lPosition[2];

							//法線情報は行列によって変化しているため更新する
							DXMatrix lNormalMatrix[3];
							for (int i = 0; i < 3; ++i) {
								lNormalMatrix[i] *= lGlobalMatrix;
								D3DXVec3TransformNormal(&lHitPolygon.second.polygon[i].Normal, &lHitPolygon.second.polygon[i].Normal, &lGlobalMatrix);
								lHitPolygon.second.polygon[i].Normal.Normalize();
							}
						}
					}
				}
			}
		}
	}


	//レイがヒットした場合、それが実際に衝突したか検知する

	//レイがヒットしなかった場合、ここで処理終了。
	//falseを返す
	if (!lHit)return false;
	//	printf("RayHit!\n");
	DXVector3 polyNormal;	//ポリゴンの法線

							//ポリゴンの向きを計算
	{
		enum class E_POS {
			NONE, X, Y, Z
		};

		//ポリゴンから各法線を求める
		auto fNormalMul = [&](E_POS pType)->float {
			float lResult;
			switch (pType)
			{
			case E_POS::X:
				lResult = lHitPolygon.second.polygon[0].Normal.x + lHitPolygon.second.polygon[1].Normal.x + lHitPolygon.second.polygon[2].Normal.x;
				break;
			case E_POS::Y:
				lResult = lHitPolygon.second.polygon[0].Normal.y + lHitPolygon.second.polygon[1].Normal.y + lHitPolygon.second.polygon[2].Normal.y;
				break;
			case E_POS::Z:
				lResult = lHitPolygon.second.polygon[0].Normal.z + lHitPolygon.second.polygon[1].Normal.z + lHitPolygon.second.polygon[2].Normal.z;
				break;
			default:
				break;
			}
			return lResult / 3.0f;
		};

		polyNormal.x = fNormalMul(E_POS::X);
		polyNormal.y = fNormalMul(E_POS::Y);
		polyNormal.z = fNormalMul(E_POS::Z);

	}

	D3DXPLANE lPlane;		//平面処理用
							//3点を使って無限平面の定義
	D3DXPlaneFromPoints(
		&lPlane,
		&lHitPolygon.second.polygon[0].Pos,
		&lHitPolygon.second.polygon[1].Pos,
		&lHitPolygon.second.polygon[2].Pos
		);

	//移動後の座標がヒットした一番近いポリゴンをまたいでいるか判定
	{
		float lAns[2];

		auto fPlaneCalc = [&](const DXVector3&pPosition)->float {
			float lResult;
			lResult = (lPlane.a * pPosition.x) + (lPlane.b * pPosition.y) + (lPlane.c * pPosition.z) + lPlane.d;
			return lResult;
		};

		lAns[0] = fPlaneCalc(lRayAfterPosition);
		lAns[1] = fPlaneCalc(*mPosBefore);

		//２つの正負が反対であれば、ポリゴンをまたいでいる
		if (lAns[0] * lAns[1] < 0.0f) {
			//滑る場合の処理
			if (mSlipFlag == true) {
				//滑った後の座標が入る一時変数
				DXVector3 lAfterPos;

				//一番近いポリゴンの距離
				float lNearDistance = FLT_MAX;
				//レイ判定
				for (int i = 0; i < lHitPolygon.second.mesh.lock()->PolygonCount; ++i) {
					//判定を行うポリゴンの取得
					FbxPtrPolygon lPtrPolygon;
					lHitPolygon.second.mesh.lock()->GetPolygon(lPtrPolygon, i);

					//行列を使って頂点位置の修正
					DXMatrix lPositionMatrix[3];
					DXVector3 lPosition[3];
					for (int i = 0; i < 3; ++i) {
						lPositionMatrix[i].Translation(lPtrPolygon[i]->Pos);
						lPositionMatrix[i] *= lHitPolygon.second.globalMatrix;
						lPositionMatrix[i].GetT(lPosition[i]);
					}
					//移動後の座標からレイを飛ばしてポリゴンとの交点を計算
					if (D3DXIntersectTri(
						&lPosition[0],
						&lPosition[1],
						&lPosition[2],
						&lRayAfterPosition,
						&polyNormal,
						nullptr,
						nullptr,
						&lDistance
						)) {
						//現在ヒットしているポリゴンより近いポリゴンの衝突した場合
						if (lNearDistance > lDistance) {
							lNearDistance = lDistance;
						}
					}

				}

				if (lNearDistance < FLT_MAX) {
					//当たった場合、滑り後の座標を計算する(少し多めに押し戻す
					lAfterPos = lRayAfterPosition + polyNormal*(lDistance + 0.05f);
					//すべての処理が終わればセットする
					pResultPosition = lAfterPos;
				}
				else {
					return false;
				}

			}
			//滑らない場合の処理
			else {

			}
			//当たったのでtrueを返す
			return true;
		}

		//2つとも負だった場合はポリゴンにめり込んでいるので、反対方向の直近ポリゴンまで移動
		if (lAns[0] < 0.0f&&lAns[1] < 0.0f) {
			//滑る場合の処理
			if (mSlipFlag == true) {
				//滑った後の座標が入る一時変数
				DXVector3 lAfterPos;

				//一番近いポリゴンの距離
				float lNearDistance = FLT_MAX;
				//レイ判定
				//反対方向にレイを飛ばす
				//ポリゴンにめり込んでいる状態が前提条件なので、
				//反対方向にレイを飛ばせば、ポリゴンにヒットする
				DXVector3 lDirection;
				D3DXVec3Normalize(&lDirection, &(*mPosBefore - lRayAfterPosition));
				for (int i = 0; i < lHitPolygon.second.mesh.lock()->PolygonCount; ++i) {
					//判定を行うポリゴンの取得
					FbxPtrPolygon lPtrPolygon;
					lHitPolygon.second.mesh.lock()->GetPolygon(lPtrPolygon, i);

					//行列を使って頂点位置の修正
					DXMatrix lPositionMatrix[3];
					DXVector3 lPosition[3];
					for (int i = 0; i < 3; ++i) {
						lPositionMatrix[i].Translation(lPtrPolygon[i]->Pos);
						lPositionMatrix[i] *= lHitPolygon.second.globalMatrix;
						lPositionMatrix[i].GetT(lPosition[i]);
					}
					//移動後の座標からレイを飛ばしてポリゴンとの交点を計算
					if (D3DXIntersectTri(
						&lPosition[0],
						&lPosition[1],
						&lPosition[2],
						&lRayAfterPosition,
						&lDirection,
						nullptr,
						nullptr,
						&lDistance
						)) {
						//現在ヒットしているポリゴンより近いポリゴンの衝突した場合、距離を更新
						//このようにしないと複雑な形をしたポリゴンの場合、
						//あらぬ場所に行ってしまうおそれがある
						if (lNearDistance > lDistance) {
							lNearDistance = lDistance;
						}
					}

				}

				if (lNearDistance < FLT_MAX) {
					//当たった場合、滑り後の座標を計算する(少し多めに押し戻す
					lAfterPos = lRayAfterPosition + lDirection*(lDistance + 0.05f);
					//すべての処理が終わればセットする
					pResultPosition = lAfterPos;
					return true;
				}
				else {
					return false;
				}


			}
		}

	}


	//レイは当たったものの、衝突はしなかったのでfalseを返す
	return false;
}
