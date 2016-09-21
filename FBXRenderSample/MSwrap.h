#pragma once


/************************************

FBX関係

************************************/
//FBXローダー
#include"DX11FbxLoader.h"
//FBXマネージャ
#include"MSFbxManager.h"



/************************************

行列関係関係

************************************/
//ワールド行列管理クラス
#include"DXWorld.h"
//ビュー行列管理クラス
#include"DXCamera.h"
//射影行列管理クラス
#include"DXProjection.h"

/************************************

テクスチャ関係

************************************/
//2Dテクスチャクラス
#include"DX11Texture.h"
//2Dテクスチャ管理クラス
#include"DX11TextureManager.h"


/************************************

//2Dスプライト基底クラス

************************************/
//スプライト基底クラス(現在2D,3D混同)
#include"MSSprite2DResource.h"
#include"MSSprite2DRender.h"

/************************************

3Dレンダリング基底クラス

***********************************/
//3Dモデル基底リソース
#include"DX11RenderResource.h"
//3Dモデル(FBX)レンダリングマネージャ
#include"MS3DRender.h"


/************************************

シーンベースクラス

************************************/
#include"MSSceneBase.h"



/************************************

ベクトル管理クラス

************************************/
//二次元ベクトル管理クラス
#include"DXVector2.h"
//三次元ベクトル管理クラス
#include"DXVector3.h"

//マクロリソース
#include"DX11Resrouce.h"
//キーリスト
#include"MSKeyCodeList.h"

/************************************

DirectX11マネージャ

************************************/
#include"MSDirect.h"

/************************************

コリジョン関係

************************************/
#include"MSCollisionSphere.h"
#include"MSCollisionRay.h"
#include"MSCollisionRayPicking.h"
#include"MSCollisionRayPlane.h"


#include<memory>
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::make_unique;
using std::make_shared;

