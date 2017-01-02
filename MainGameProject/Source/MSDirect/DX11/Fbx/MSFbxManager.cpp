#include "MSFbxManager.h"
#include<DX11FBXLoader.hpp>
#include"DX11Resrouce.h"
#include"MSCollisionSphere.h"
#include"DXMatrix.h"
#include"DX11RenderResource.h"
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

ID3D11Device*MSFbxManager::sDevice{ nullptr };				//DirectX11�f�o�C�X
ID3D11DeviceContext*MSFbxManager::sDeviceContext{ nullptr };	//DirectX11�f�o�C�X�R���e�L�X�g

MSFbxManager::MSFbxManager() :
	mLoader{ nullptr }
{
}

MSFbxManager::~MSFbxManager()
{
	FbxTime f;
	
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

	RegisterMesh(mLoader->GetStartFrame(), mLoader->GetDefaultAnimation());

}

void MSFbxManager::RegisterMesh(FbxTime & mCurrentFrame, const int pAnimationIndex)
{
	//�w��^�C���Ƀ��b�V�������݂���ꍇ�͐������Ȃ�


	long long l = mCurrentFrame.Get();
	if (mMeshBuffer.count(pAnimationIndex) != 0) {
		if (mMeshBuffer[pAnimationIndex].count(mCurrentFrame.Get()) != 0) {
			return;
		}
	}

	auto lMeshBuffer = &mMeshBuffer[pAnimationIndex][mCurrentFrame.Get()];
	mLoader->SetAnimation(pAnimationIndex);
	lMeshBuffer->mMeshData = mLoader->GetGeometryData2(mCurrentFrame);


	//�C���f�b�N�X�o�b�t�@�̂��ꂼ��̒������L������
	lMeshBuffer->mIndexLength.resize(lMeshBuffer->mMeshData->size());
	for (auto i = 0; i < lMeshBuffer->mMeshData->size(); i++) {
		lMeshBuffer->mIndexLength[i].resize(lMeshBuffer->mMeshData->at(i)->subMesh.size());
		for (auto j = 0; j < lMeshBuffer->mMeshData->at(i)->subMesh.size(); j++) {
			lMeshBuffer->mIndexLength[i][j] = lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->IndexLength;
		}
	}


	lMeshBuffer->mVertexBuffer.resize(lMeshBuffer->mMeshData->size());
	lMeshBuffer->mVertexCollisionBuffer.resize(lMeshBuffer->mMeshData->size());
	lMeshBuffer->mIndexBuffer.resize(lMeshBuffer->mMeshData->size());
	for (unsigned int i = 0; i < lMeshBuffer->mMeshData->size(); i++) {
		lMeshBuffer->mVertexBuffer[i].resize(lMeshBuffer->mMeshData->at(i)->subMesh.size());
		lMeshBuffer->mVertexCollisionBuffer[i].resize(lMeshBuffer->mMeshData->at(i)->subMesh.size());
		lMeshBuffer->mIndexBuffer[i].resize(lMeshBuffer->mMeshData->at(i)->subMesh.size());
		for (unsigned int j = 0; j < lMeshBuffer->mMeshData->at(i)->subMesh.size(); j++) {

			D3D11_BUFFER_DESC mVBDesc;
			D3D11_BUFFER_DESC mIBDesc;
			//���_�o�b�t�@��񏉊���
			mVBDesc.Usage = D3D11_USAGE_DEFAULT;
			mVBDesc.ByteWidth = sizeof(FbxVertex)*lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->PosLength;
			mVBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			mVBDesc.CPUAccessFlags = 0;
			mVBDesc.MiscFlags = 0;

			

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->Data.data();
			sDevice->CreateBuffer(&mVBDesc, &InitData, &lMeshBuffer->mVertexBuffer[i][j]);

			//�C���f�b�N�X�o�b�t�@��񏉊���
			mIBDesc.Usage = D3D11_USAGE_DEFAULT;
			mIBDesc.ByteWidth = lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->IndexLength * sizeof(int);
			mIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			mIBDesc.CPUAccessFlags = 0;
			mIBDesc.MiscFlags = 0;
			//�C���f�b�N�X�o�b�t�@�[���쐬
			InitData.pSysMem = lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->Index;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;
			ID3D11Buffer* IB;
			sDevice->CreateBuffer(&mIBDesc, &InitData, &IB);
			lMeshBuffer->mIndexBuffer[i][j] = IB;


			//�R���W�����o�b�t�@(���_�̂�)�̍쐬
			std::vector<DXVector3> mPositions;
			mPositions.reserve(lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->PosLength);
			for (int k = 0; k < lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->PosLength; ++k) {
				mPositions.push_back(lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->Data[k].Pos);
			}

			//���_�o�b�t�@��񏉊���
			D3D11_BUFFER_DESC mVBCDesc;
			mVBCDesc.Usage = D3D11_USAGE_DEFAULT;
			mVBCDesc.ByteWidth = sizeof(DXVector3)*lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->PosLength;
			mVBCDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			mVBCDesc.CPUAccessFlags = 0;
			mVBCDesc.MiscFlags = 0;
			InitData.pSysMem = mPositions.data();
			sDevice->CreateBuffer(&mVBCDesc, &InitData, &lMeshBuffer->mVertexCollisionBuffer[i][j]);

		}
	}

	//�R���W�������b�V���̐���
//	CreateCollisionSphere(mCurrentFrame, pAnimationIndex);

}

std::string MSFbxManager::GetFileName()
{
	return mFileName;
}

bool MSFbxManager::Update(FbxTime& mCurrentFrame, const int pAnimationIndex)
{
	RegisterMesh(mCurrentFrame, pAnimationIndex);
	

	return true;
}


void MSFbxManager::Release()
{
	
	for (auto&lMeshBufferAnims : mMeshBuffer) {
		for (auto&lMeshBuffer : lMeshBufferAnims.second) {
			for (auto i = 0; i < lMeshBuffer.second.mVertexBuffer.size(); ++i) {
				for (auto j = 0; j < lMeshBuffer.second.mVertexBuffer.at(i).size(); j++) {
					SAFE_RELEASE(lMeshBuffer.second.mVertexBuffer[i][j]);
					SAFE_RELEASE(lMeshBuffer.second.mVertexCollisionBuffer[i][j]);
					SAFE_RELEASE(lMeshBuffer.second.mIndexBuffer[i][j]);
				}
			}
			lMeshBuffer.second.mVertexBuffer.clear();
			lMeshBuffer.second.mIndexBuffer.clear();
		}
	}

}




void MSFbxManager::CreateCollisionSphere(FbxTime& mCurrentFrame, const int pAnimationIndex)
{
	/*
		���b�V���̃C���f�b�N�X�ɑΉ�����悤�ɐ݌v
	*/

	//�R���W�����̃x�N�^���쐬
	auto lCollisions = &mMeshBuffer[pAnimationIndex][mCurrentFrame.Get()].mCollisions;
	auto lMeshBuffer = &mMeshBuffer[pAnimationIndex][mCurrentFrame.Get()];

	lCollisions->resize(lMeshBuffer->mMeshData->size());
	for (unsigned int i = 0; i < lMeshBuffer->mMeshData->size(); i++) {
		lCollisions->at(i).resize(lMeshBuffer->mMeshData->at(i)->subMesh.size());
		for (unsigned int j = 0; j < lMeshBuffer->mMeshData->at(i)->subMesh.size(); j++) {
			lCollisions->at(i).at(j).CreateCollision(
				*lMeshBuffer->mMeshData->at(i)->subMesh.at(j),
				*lMeshBuffer->mMeshData->at(i)->mWorld
			);
		}
	}
}

//void MSFbxManager::RegisterCollision(const std::shared_ptr<DX11RenderResource>&pResource) {
//	if (mCollisions->size() > 0) {
//		pResource->SetCollisionSphere(mCollisions);
//	}
//}

std::vector<std::shared_ptr<FBXMesh>>* MSFbxManager::GetMeshData(int aAnimation, FbxTime aTime)
{
	return mMeshBuffer[aAnimation][aTime.Get()].mMeshData.get();
}

std::vector<std::vector<ID3D11Buffer*>> * MSFbxManager::GetVertexBuffer(int aAnimation, FbxTime aTime)
{
	return &mMeshBuffer[aAnimation][aTime.Get()].mVertexBuffer;
}

std::vector<std::vector<ID3D11Buffer*>>* MSFbxManager::GetVertexCollisionBuffer(int aAnimation, FbxTime aTime)
{
	return &mMeshBuffer[aAnimation][aTime.Get()].mVertexCollisionBuffer;
}

std::vector<std::vector<ID3D11Buffer*>> * MSFbxManager::GetIndexBuffer(int aAnimation, FbxTime aTime)
{
	return &mMeshBuffer[aAnimation][aTime.Get()].mIndexBuffer;
}

std::vector<std::vector<uint32_t>> * MSFbxManager::GetIndexBufferCount(int aAnimation, FbxTime aTime)
{
	return &mMeshBuffer[aAnimation][aTime.Get()].mIndexLength;
}

std::vector<std::vector<MSCollisionSphere>>* MSFbxManager::GetCollisionSphere(int aAnimation, FbxTime aTime)
{
	return &mMeshBuffer[aAnimation][aTime.Get()].mCollisions;
}

//std::vector<std::shared_ptr<FBXMesh>> MSFbxManager::MoveMeshData() {
//	return std::move(*lMeshBuffer->mMeshData);
//}
//
//std::vector<std::vector<ID3D11Buffer*>> MSFbxManager::MoveVertexBuffer() {
//	return std::move(mVertexBuffer);
//}
//
//std::vector<std::vector<ID3D11Buffer*>> MSFbxManager::MoveIndexBuffer() {
//	return std::move(mIndexBuffer);
//}

MeshBuffer::~MeshBuffer()
{
	printf("End\n");
}
