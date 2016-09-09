#include "DX11FbxManager.h"
#include"DX11FbxResource.h"
#include"DX11FbxLoader.h"
#include"DX11Resrouce.h"
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

ID3D11Device*DX11FbxManager::sDevice{ nullptr };				//DirectX11�f�o�C�X
ID3D11DeviceContext*DX11FbxManager::sDeviceContext{ nullptr };	//DirectX11�f�o�C�X�R���e�L�X�g

DX11FbxManager::DX11FbxManager() :
	mLoader{ nullptr },
	mVertexBuffer{ },
	mIndexBuffer{ }
{
}

DX11FbxManager::~DX11FbxManager()
{
	Release();
	FBX_SAFE_DELETE(mLoader);
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
	mFileName = pFileName;
	if (mLoader == nullptr) {
		mLoader = new DX11FbxLoader();
	}
	else {
		mLoader->FbxDestroy();
	}
	mLoader->FbxInit(pFileName,animationLoad);
	mLoader->GetGeometryData2();

	//�o�b�t�@���\�[�X�̍쐬
	//���b�V���̐��擾
	mLoader->GetMeshCount(mMeshCount);
	//���b�V�����Ƃ̒��_���擾
	mLoader->GetMeshVertexCount(mMeshVertexCount);
	//���b�V�����Ƃ̃C���f�N�X���擾
	mLoader->GetMeshIndexCount(mMeshIndexCount);

	mVertexBuffer.clear();
	mIndexBuffer.clear();

	mVertexBuffer.resize(mMeshCount.size());
	mIndexBuffer.resize(mMeshCount.size());
	mMeshVBDesc.resize(mMeshCount.size());
	mMeshIBDesc.resize(mMeshCount.size());

	for (unsigned int i = 0; i < mMeshCount.size(); i++) {
		//�T�u���b�V���̐������쐬
		mVertexBuffer[i].resize(mMeshCount[i]);
		mIndexBuffer[i].resize(mMeshCount[i]);
		mMeshVBDesc[i].resize(mMeshCount[i]);
		mMeshIBDesc[i].resize(mMeshCount[i]);

		//�o�b�t�@�f�X�N���v�V�����͂����ō쐬����
		for (int j = 0; j < mMeshCount[j]; j++) {
			{
				mMeshVBDesc[i][j].Usage = D3D11_USAGE_DEFAULT;
				mMeshVBDesc[i][j].ByteWidth = mMeshVertexCount[i][j]* sizeof(FbxVertex);
				mMeshVBDesc[i][j].BindFlags = D3D11_BIND_VERTEX_BUFFER;
				mMeshVBDesc[i][j].CPUAccessFlags = 0;
				mMeshVBDesc[i][j].MiscFlags = 0;
			}
			{
				//�C���f�b�N�X�o�b�t�@�[���쐬
				mMeshIBDesc[i][j].Usage = D3D11_USAGE_DEFAULT;
				mMeshIBDesc[i][j].ByteWidth = mMeshIndexCount[i][j]*sizeof(int);
				mMeshIBDesc[i][j].BindFlags = D3D11_BIND_INDEX_BUFFER;
				mMeshIBDesc[i][j].CPUAccessFlags = 0;
				mMeshIBDesc[i][j].MiscFlags = 0;
			}
		}

	}


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
}

void DX11FbxManager::Update()
{
	
	D3DXVECTOR3 transPos;
	mMeshData = mLoader->GetGeometryData2();
	//�o�b�t�@�����݂����ꍇ��U���


	for (int i = 0; i < mVertexBuffer.size(); i++) {
		for (int j = 0; j < mVertexBuffer.at(i).size(); j++) {

			mVertexBuffer[i][j]->Release();
			mIndexBuffer[i][j]->Release();

		}
	}


	//�o�b�t�@�̍쐬
	//���b�V���̐������쐬
	//mVertexBuffer.clear();
	//mIndexBuffer.clear();

	//mVertexBuffer.resize(mMeshData->size());
	//mIndexBuffer.resize(mMeshData->size());
	for (unsigned int i = 0; i < mMeshData->size(); i++) {
		//�T�u���b�V���̐������쐬
		//mVertexBuffer[i].resize(mMeshData->at(i)->subMesh.size());
		//mIndexBuffer[i].resize(mMeshData->at(i)->subMesh.size());
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

	for (int i = 0; i < mVertexBuffer.size(); i++) {
		for (int j = 0; j < mVertexBuffer.at(i).size(); j++) {
			SAFE_RELEASE(mVertexBuffer[i][j]);
			SAFE_RELEASE(mIndexBuffer[i][j]);
		}
	}
	mVertexBuffer.clear();
	mIndexBuffer.clear();
	//

	//if (mMeshData.use_count() == 0)return;
	//for (unsigned int i = 0; i < mMeshData->size(); i++) {
	//	for (unsigned int j = 0; j < mMeshData->at(i)->subMesh.size(); j++) {
	//		SAFE_RELEASE(mVertexBuffer[i][j]);
	//		SAFE_RELEASE(mIndexBuffer[i][j]);
	//	}
	//	SAFE_DELETE_ARRAY(mVertexBuffer[i]);
	//	SAFE_DELETE_ARRAY(mVertexBuffer[i]);

	//}
	//SAFE_DELETE_ARRAY(mVertexBuffer);
	//SAFE_DELETE_ARRAY(mVertexBuffer);

}

std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>> DX11FbxManager::GetMeshData()
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



