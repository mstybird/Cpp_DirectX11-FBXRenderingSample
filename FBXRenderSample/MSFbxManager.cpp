#include "MSFbxManager.h"
#include"DX11FbxResource.h"
#include"DX11FbxLoader.h"
#include"DX11Resrouce.h"
#include"MSCollisionSphere.h"
#include"DXMatrix.h"
#include"DX11RenderResource.h"
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

ID3D11Device*MSFbxManager::sDevice{ nullptr };				//DirectX11�f�o�C�X
ID3D11DeviceContext*MSFbxManager::sDeviceContext{ nullptr };	//DirectX11�f�o�C�X�R���e�L�X�g

MSFbxManager::MSFbxManager() :
	mLoader{ nullptr },
	mVertexBuffer{ },
	mIndexBuffer{ }
{
}

MSFbxManager::~MSFbxManager()
{
	Release();
	FBX_SAFE_DELETE(mLoader);
}

void MSFbxManager::InitDevice(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
}

void MSFbxManager::Initialize()
{


}

void MSFbxManager::LoadFile(std::string pFileName, bool animationLoad)
{
	mFileName = pFileName;
	if (mLoader == nullptr) {
		mLoader = new DX11FbxLoader();
	}
	else {
		mLoader->FbxDestroy();
	}
	mLoader->FbxInit(pFileName,animationLoad);
	mAnimationFlag = animationLoad;
	mMeshData = mLoader->GetGeometryData2();

	//�o�b�t�@���\�[�X�̍쐬
	//���b�V���̐��擾
	mLoader->GetMeshCount(mMeshCount);
	//���b�V�����Ƃ̒��_���擾
	mLoader->GetMeshVertexCount(mMeshVertexCount);
	//���b�V�����Ƃ̃C���f�N�X���擾
	mLoader->GetMeshIndexCount(mMeshIndexCount);

	mVertexBuffer.clear();
	mIndexBuffer.clear();

	mVertexBuffer.resize(mMeshData->size());
	mIndexBuffer.resize(mMeshData->size());
	mMeshVBDesc.resize(mMeshData->size());
	mMeshIBDesc.resize(mMeshData->size());

	for (unsigned int i = 0; i < mMeshData->size(); i++) {
		//�T�u���b�V���̐������쐬
		mVertexBuffer[i].resize(mMeshData->at(i)->subMesh.size());
		mIndexBuffer[i].resize(mMeshData->at(i)->subMesh.size());
		mMeshVBDesc[i].resize(mMeshData->at(i)->subMesh.size());
		mMeshIBDesc[i].resize(mMeshData->at(i)->subMesh.size());

		//�o�b�t�@�f�X�N���v�V�����͂����ō쐬����
		for (unsigned int j = 0; j < mMeshData->at(i)->subMesh.size(); j++) {
			{
				mMeshVBDesc[i][j].Usage = D3D11_USAGE_DEFAULT;
				mMeshVBDesc[i][j].ByteWidth = sizeof(FbxVertex)*mMeshData->at(i)->subMesh.at(j)->PosLength;
				mMeshVBDesc[i][j].BindFlags = D3D11_BIND_VERTEX_BUFFER;
				mMeshVBDesc[i][j].CPUAccessFlags = 0;
				mMeshVBDesc[i][j].MiscFlags = 0;
			}
			{
				//�C���f�b�N�X�o�b�t�@�[�f�X�N���v�V�������쐬
				mMeshIBDesc[i][j].Usage = D3D11_USAGE_DEFAULT;
				mMeshIBDesc[i][j].ByteWidth = mMeshData->at(i)->subMesh.at(j)->IndexLength * sizeof(int);
				mMeshIBDesc[i][j].BindFlags = D3D11_BIND_INDEX_BUFFER;
				mMeshIBDesc[i][j].CPUAccessFlags = 0;
				mMeshIBDesc[i][j].MiscFlags = 0;
			}
		}
	} 

	//�A�j���[�V�������Ȃ��ꍇ�͂��̎��_�Ń��b�V���f�[�^���쐬���Ă���
	if (mAnimationFlag == false) {

		for (unsigned int i = 0; i < mMeshData->size(); i++) {
			for (unsigned int j = 0; j < mMeshData->at(i)->subMesh.size(); j++) {

				D3D11_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = mMeshData->at(i)->subMesh.at(j)->Data.data();
				sDevice->CreateBuffer(&mMeshVBDesc[i][j], &InitData, &mVertexBuffer[i][j]);

				//�C���f�b�N�X�o�b�t�@�[���쐬
				InitData.pSysMem = mMeshData->at(i)->subMesh.at(j)->Index;
				InitData.SysMemPitch = 0;
				InitData.SysMemSlicePitch = 0;
				sDevice->CreateBuffer(&mMeshIBDesc[i][j], &InitData, &mIndexBuffer[i][j]);
			}
		}
	}



}

void MSFbxManager::Update()
{
	//�A�j���[�V�������Ȃ��ꍇ�͍X�V�����s�v
	if (mAnimationFlag == false)return;
	mMeshData = mLoader->GetGeometryData2();

	for (unsigned int i = 0; i < mVertexBuffer.size(); i++) {
		for (unsigned int j = 0; j < mVertexBuffer.at(i).size(); j++) {
			SAFE_RELEASE(mVertexBuffer[i][j]);
			SAFE_RELEASE(mIndexBuffer[i][j]);
		}
	}

	for (unsigned int i = 0; i < mMeshData->size(); i++) {
		for (unsigned int j = 0; j < mMeshData->at(i)->subMesh.size(); j++) {

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = mMeshData->at(i)->subMesh.at(j)->Data.data();
			sDevice->CreateBuffer(&mMeshVBDesc[i][j], &InitData, &mVertexBuffer[i][j]);

			//�C���f�b�N�X�o�b�t�@�[���쐬
			InitData.pSysMem = mMeshData->at(i)->subMesh.at(j)->Index;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;
			sDevice->CreateBuffer(&mMeshIBDesc[i][j], &InitData, &mIndexBuffer[i][j]);
		}
	}
}


void MSFbxManager::Release()
{

	for (unsigned int i = 0; i < mVertexBuffer.size(); i++) {
		for (unsigned int j = 0; j < mVertexBuffer.at(i).size(); j++) {
			SAFE_RELEASE(mVertexBuffer[i][j]);
			SAFE_RELEASE(mIndexBuffer[i][j]);
		}
	}
	mVertexBuffer.clear();
	mIndexBuffer.clear();

}

std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>> &MSFbxManager::GetMeshData()
{
	return mMeshData;
}



void MSFbxManager::CreateCollisionSphere()
{
	/*
		���b�V���̃C���f�b�N�X�ɑΉ�����悤�ɐ݌v
	*/

	//�R���W�����̃x�N�^���쐬
	if (!mCollisions) {
		mCollisions = std::make_shared<std::vector<std::vector<MSCollisionSphere>>>();
	}

	mCollisions->resize(mMeshData->size());
	for (unsigned int i = 0; i < mMeshData->size(); i++) {
		mCollisions->at(i).resize(mCollisions->size());
		for (unsigned int j = 0; j < mMeshData->at(i)->subMesh.size(); j++) {
			mCollisions->at(i).at(j).CreateCollision(
				*mMeshData->at(i)->subMesh.at(j),
				*mMeshData->at(i)->mWorld
			);
		}
	}
}

void MSFbxManager::RegisterCollision(const std::shared_ptr<DX11RenderResource>&pResource) {
	pResource->SetCollisionSphere(mCollisions);
}

ID3D11Buffer * MSFbxManager::GetVertexBuffer(int i, int j)
{
	return mVertexBuffer[i][j];
}

ID3D11Buffer * MSFbxManager::GetIndexBuffer(int i, int j)
{
	return mIndexBuffer[i][j];
}

unsigned int * MSFbxManager::GetIndexBufferCount(int i,int j)
{
	
	return &mMeshData->at(i)->subMesh.at(j)->IndexLength;
}



