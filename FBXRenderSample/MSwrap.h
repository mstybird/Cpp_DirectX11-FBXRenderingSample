#pragma once


/************************************

FBX�֌W

************************************/
//FBX���[�_�[
#include"DX11FbxLoader.h"
//FBX�}�l�[�W��
#include"MSFbxManager.h"



/************************************

�s��֌W�֌W

************************************/
//���[���h�s��Ǘ��N���X
#include"DXWorld.h"
//�r���[�s��Ǘ��N���X
#include"DXCamera.h"
//�ˉe�s��Ǘ��N���X
#include"DXProjection.h"

/************************************

�e�N�X�`���֌W

************************************/
//2D�e�N�X�`���N���X
#include"DX11Texture.h"
//2D�e�N�X�`���Ǘ��N���X
#include"DX11TextureManager.h"


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

�x�N�g���Ǘ��N���X

************************************/
//�񎟌��x�N�g���Ǘ��N���X
#include"DXVector2.h"
//�O�����x�N�g���Ǘ��N���X
#include"DXVector3.h"

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
#include"MSCollisionRayPlane.h"

#include"MSCullingFrustum.h"
#include"MSCullingOcclusion.h"

#include<memory>
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::make_unique;
using std::make_shared;

