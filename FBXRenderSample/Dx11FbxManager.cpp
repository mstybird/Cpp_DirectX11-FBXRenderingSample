#include "DX11FbxManager.h"
#include"DX11FbxResource.h"
#include"DX11FbxLoader.h"
#include<vector>
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

ID3D11Device*DX11FbxManager::sDevice{ nullptr };				//DirectX11�f�o�C�X
ID3D11DeviceContext*DX11FbxManager::sDeviceContext{ nullptr };	//DirectX11�f�o�C�X�R���e�L�X�g

DX11FbxManager::DX11FbxManager() :
	mLoader{ nullptr },
	mVertexBuffer{ nullptr },
	mIndexBuffer{ nullptr },
	mMeshData{ nullptr } {
}

DX11FbxManager::~DX11FbxManager()
{
	Release();
}

void DX11FbxManager::InitDevice(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
}

void DX11FbxManager::Initialize()
{


}

void DX11FbxManager::LoadFile(std::string pFileName, bool animationLoad)
{
	if (mLoader == nullptr) {
		mLoader = new DX11FbxLoader();
	}
	else {
		mLoader->FbxDestroy();
	}
	mLoader->FbxInit(pFileName,animationLoad);
	D3DXVECTOR3 transPos;
	mLoader->GetGeometryData2();
}

void DX11FbxManager::Update()
{
	
	D3DXVECTOR3 transPos;
	mMeshData = mLoader->GetGeometryData2();
	//�o�b�t�@�����݂����ꍇ��U���
	if (mVertexBuffer != nullptr) {
		//���b�V���̉��
		for (unsigned int i = 0; i < mMeshData->size(); i++) {
			//�T�u���b�V���̉��
			for (unsigned int j = 0; j < mMeshData->at(i)->subMesh.size(); j++) {
				mVertexBuffer[i][j]->Release();
				mIndexBuffer[i][j]->Release();
			}
			delete[] mVertexBuffer[i];
			delete[] mIndexBuffer[i];
			mVertexBuffer[i] = nullptr;
			mIndexBuffer[i] = nullptr;
		}

		delete[] mVertexBuffer;
		delete[] mIndexBuffer;
		mVertexBuffer = nullptr;
		mIndexBuffer = nullptr;

	}

	//�o�b�t�@�̍쐬
	//���b�V���̐������쐬
	mVertexBuffer = new ID3D11Buffer**[mMeshData->size()];
	mIndexBuffer = new ID3D11Buffer**[mMeshData->size()];

	for (unsigned int i = 0; i < mMeshData->size(); i++) {
		//�T�u���b�V���̐������쐬
		mVertexBuffer[i] = new ID3D11Buffer*[mMeshData->at(i)->subMesh.size()];
		mIndexBuffer[i] = new ID3D11Buffer*[mMeshData->at(i)->subMesh.size()];
		for (unsigned int j = 0; j < mMeshData->at(i)->subMesh.size(); j++) {
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(FbxVertex)*mMeshData->at(i)->subMesh.at(j)->PosLength;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = mMeshData->at(i)->subMesh.at(j)->Data.data();
			sDevice->CreateBuffer(&bd, &InitData, &mVertexBuffer[i][j]);

			//�C���f�b�N�X�o�b�t�@�[���쐬
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(int) * mMeshData->at(i)->subMesh.at(j)->IndexLength;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			InitData.pSysMem = mMeshData->at(i)->subMesh.at(j)->Index;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;
			sDevice->CreateBuffer(&bd, &InitData, &mIndexBuffer[i][j]);
		}
	}

}


void DX11FbxManager::Release()
{
	
	if (mVertexBuffer != nullptr) {
		for (unsigned int i = 0; i < mMeshData->size(); i++) {
			for (unsigned int j = 0; j < mMeshData->at(i)->subMesh.size(); j++) {
				mVertexBuffer[i][j]->Release();
				mIndexBuffer[i][j]->Release();
			}
			delete[] mVertexBuffer[i];
			delete[] mIndexBuffer[i];
			mVertexBuffer[i] = nullptr;
			mIndexBuffer[i] = nullptr;
		}

		delete[] mVertexBuffer;
		delete[] mIndexBuffer;
		mVertexBuffer = nullptr;
		mIndexBuffer = nullptr;
	}
	if (mLoader != nullptr) {
		delete mLoader;
		mLoader = nullptr;
	}
}

std::vector<FBXMesh*>* DX11FbxManager::GetMeshData()
{
	return mMeshData;
}

ID3D11Buffer * DX11FbxManager::GetVertexBuffer(int i, int j)
{
	return mVertexBuffer[i][j];
}

ID3D11Buffer * DX11FbxManager::GetIndexBuffer(int i, int j)
{
	return mIndexBuffer[i][j];
}

unsigned int * DX11FbxManager::GetIndexBufferCount(int i,int j)
{
	return &mMeshData->at(i)->subMesh.at(j)->IndexLength;
}



