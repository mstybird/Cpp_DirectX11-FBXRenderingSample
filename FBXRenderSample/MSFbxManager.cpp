#include "MSFbxManager.h"
#include"DX11FbxResource.h"
#include"DX11FbxLoader.h"
#include"DX11Resrouce.h"
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

ID3D11Device*MSFbxManager::sDevice{ nullptr };				//DirectX11デバイス
ID3D11DeviceContext*MSFbxManager::sDeviceContext{ nullptr };	//DirectX11デバイスコンテキスト

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

	//バッファリソースの作成
	//メッシュの数取得
	mLoader->GetMeshCount(mMeshCount);
	//メッシュごとの頂点数取得
	mLoader->GetMeshVertexCount(mMeshVertexCount);
	//メッシュごとのインデクス数取得
	mLoader->GetMeshIndexCount(mMeshIndexCount);

	mVertexBuffer.clear();
	mIndexBuffer.clear();

	mVertexBuffer.resize(mMeshData->size());
	mIndexBuffer.resize(mMeshData->size());
	mMeshVBDesc.resize(mMeshData->size());
	mMeshIBDesc.resize(mMeshData->size());

	for (unsigned int i = 0; i < mMeshData->size(); i++) {
		//サブメッシュの数だけ作成
		mVertexBuffer[i].resize(mMeshData->at(i)->subMesh.size());
		mIndexBuffer[i].resize(mMeshData->at(i)->subMesh.size());
		mMeshVBDesc[i].resize(mMeshData->at(i)->subMesh.size());
		mMeshIBDesc[i].resize(mMeshData->at(i)->subMesh.size());

		//バッファデスクリプションはここで作成する
		for (unsigned int j = 0; j < mMeshData->at(i)->subMesh.size(); j++) {
			{
				mMeshVBDesc[i][j].Usage = D3D11_USAGE_DEFAULT;
				mMeshVBDesc[i][j].ByteWidth = sizeof(FbxVertex)*mMeshData->at(i)->subMesh.at(j)->PosLength;
				mMeshVBDesc[i][j].BindFlags = D3D11_BIND_VERTEX_BUFFER;
				mMeshVBDesc[i][j].CPUAccessFlags = 0;
				mMeshVBDesc[i][j].MiscFlags = 0;
			}
			{
				//インデックスバッファーデスクリプションを作成
				mMeshIBDesc[i][j].Usage = D3D11_USAGE_DEFAULT;
				mMeshIBDesc[i][j].ByteWidth = mMeshData->at(i)->subMesh.at(j)->IndexLength * sizeof(int);
				mMeshIBDesc[i][j].BindFlags = D3D11_BIND_INDEX_BUFFER;
				mMeshIBDesc[i][j].CPUAccessFlags = 0;
				mMeshIBDesc[i][j].MiscFlags = 0;
			}
		}
	}

	//アニメーションしない場合はこの時点でメッシュデータを作成しておく
	if (mAnimationFlag == false) {

		for (unsigned int i = 0; i < mMeshData->size(); i++) {
			for (unsigned int j = 0; j < mMeshData->at(i)->subMesh.size(); j++) {

				D3D11_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = mMeshData->at(i)->subMesh.at(j)->Data.data();
				sDevice->CreateBuffer(&mMeshVBDesc[i][j], &InitData, &mVertexBuffer[i][j]);

				//インデックスバッファーを作成
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
	//アニメーションしない場合は更新処理不要
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

			//インデックスバッファーを作成
			InitData.pSysMem = mMeshData->at(i)->subMesh.at(j)->Index;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;
			sDevice->CreateBuffer(&mMeshIBDesc[i][j], &InitData, &mIndexBuffer[i][j]);
		}
	}

	//バッファの作成
	//メッシュの数だけ作成
	//mVertexBuffer.clear();
	//mIndexBuffer.clear();

	//mVertexBuffer.resize(mMeshData->size());
	//mIndexBuffer.resize(mMeshData->size());
	//for (unsigned int i = 0; i < mMeshData->size(); i++) {
	//	//サブメッシュの数だけ作成
	//	//mVertexBuffer[i].resize(mMeshData->at(i)->subMesh.size());
	//	//mIndexBuffer[i].resize(mMeshData->at(i)->subMesh.size());
	//	for (unsigned int j = 0; j < mMeshData->at(i)->subMesh.size(); j++) {
	//		D3D11_BUFFER_DESC bd;
	//		bd.Usage = D3D11_USAGE_DEFAULT;
	//		bd.ByteWidth = sizeof(FbxVertex)*mMeshData->at(i)->subMesh.at(j)->PosLength;
	//		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//		bd.CPUAccessFlags = 0;
	//		bd.MiscFlags = 0;

	//		D3D11_SUBRESOURCE_DATA InitData;
	//		InitData.pSysMem = mMeshData->at(i)->subMesh.at(j)->Data.data();
	//		sDevice->CreateBuffer(&bd, &InitData, &mVertexBuffer[i][j]);

	//		//インデックスバッファーを作成
	//		bd.Usage = D3D11_USAGE_DEFAULT;
	//		bd.ByteWidth = sizeof(int) * mMeshData->at(i)->subMesh.at(j)->IndexLength;
	//		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//		bd.CPUAccessFlags = 0;
	//		bd.MiscFlags = 0;
	//		InitData.pSysMem = mMeshData->at(i)->subMesh.at(j)->Index;
	//		InitData.SysMemPitch = 0;
	//		InitData.SysMemSlicePitch = 0;
	//		sDevice->CreateBuffer(&bd, &InitData, &mIndexBuffer[i][j]);
	//	}
	//}

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

std::shared_ptr<std::vector<std::shared_ptr<FBXMesh>>> MSFbxManager::GetMeshData()
{
	return mMeshData;
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



