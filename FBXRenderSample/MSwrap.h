#pragma once


/************************************

FBX関係

************************************/
//FBXローダー
#include<DX11FBXLoader.hpp>
//FBXマネージャ
#include"MSFbxManager.h"




/************************************

テクスチャ関係

************************************/
#include"DX11TextureManager.hpp"


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

数学関係クラス

************************************/
#include"DXMath.hpp"

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

#include"MSCullingFrustum.h"
#include"MSCullingOcclusion.h"

#include<memory>
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::make_unique;
using std::make_shared;

