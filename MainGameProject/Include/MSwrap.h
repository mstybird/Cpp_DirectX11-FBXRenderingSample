#pragma once


/************************************

FBX�֌W

************************************/
//FBX���[�_�[
#include<DX11FBXLoader.hpp>
//FBX�}�l�[�W��
#include"MSFbxManager.h"




/************************************

�e�N�X�`���֌W

************************************/
#include"DX11TextureManager.hpp"


/************************************

//2D�X�v���C�g���N���X

************************************/
//�X�v���C�g���N���X(����2D,3D����)
#include"MSSprite2DResource.h"
#include"MSSprite2DRender.h"

/************************************

3D�����_�����O���N���X

***********************************/
//3D���f����ꃊ�\�[�X
#include"DX11RenderResource.h"
//3D���f��(FBX)�����_�����O�}�l�[�W��
#include"MS3DRender.h"


/************************************

�V�[���x�[�X�N���X

************************************/
#include"MSSceneBase.h"



/************************************

���w�֌W�N���X

************************************/
#include"DXMath.hpp"

//�}�N�����\�[�X
#include"DX11Resrouce.h"
//�L�[���X�g
#include"MSKeyCodeList.h"

/************************************

DirectX11�}�l�[�W��

************************************/
#include"MSDirect.h"

/************************************

�R���W�����֌W

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

