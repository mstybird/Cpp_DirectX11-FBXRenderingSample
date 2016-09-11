#pragma once
//FBXローダー
#include"DX11FbxLoader.h"
//FBXマネージャ
#include"MSFbxManager.h"

//3Dモデル基底リソース
#include"DX11RenderResource.h"
//ワールド行列管理クラス
#include"DXWorld.h"
//ビュー行列管理クラス
#include"DXCamera.h"
//射影行列管理クラス
#include"DXProjection.h"

//3Dモデル(FBX)レンダリングマネージャ
#include"MS3DRender.h"


//2Dテクスチャクラス
#include"DX11Texture.h"
//2Dテクスチャ管理クラス
#include"DX11TextureManager.h"

//2Dスプライト基底クラス
#include"MSSprite2DResource.h"
#include"MSSprite2DRender.h"


/************************************

ベクトル管理クラス

************************************/
//二次元ベクトル管理クラス
#include"DXVector2.h"
//三次元ベクトル管理クラス
#include"DXVector3.h"

//マクロリソース
#include"DX11Resrouce.h"

#include<memory>
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::make_unique;
using std::make_shared;

