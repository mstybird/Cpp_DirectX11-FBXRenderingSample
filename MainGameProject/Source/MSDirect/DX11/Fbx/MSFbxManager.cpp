#include "MSFbxManager.h"
#include<DX11FBXLoader.hpp>
#include"DX11Resrouce.h"
#include"MSCollisionSphere.h"
#include"DXMatrix.h"
#include"DX11RenderResource.h"
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

ID3D11Device*MSFbxManager::sDevice{ nullptr };				//DirectX11デバイス
ID3D11DeviceContext*MSFbxManager::sDeviceContext{ nullptr };	//DirectX11デバイスコンテキスト

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

	//lMeshBuffer->mMeshData = mLoader->GetGeometryData2(mLoader->GetStartFrame());

	////バッファリソースの作成
	////メッシュの数取得
	//mLoader->GetMeshCount(mMeshCount);
	////メッシュごとの頂点数取得
	//mLoader->GetMeshVertexCount(mMeshVertexCount);
	////メッシュごとのインデクス数取得
	//mLoader->GetMeshIndexCount(mMeshIndexCount);
	//
	//mVertexBuffer.clear();
	//mIndexBuffer.clear();

	//mVertexBuffer.resize(lMeshBuffer->mMeshData->size());
	//mIndexBuffer.resize(lMeshBuffer->mMeshData->size());
	//mMeshVBDesc.resize(lMeshBuffer->mMeshData->size());
	//mMeshIBDesc.resize(lMeshBuffer->mMeshData->size());

	//for (unsigned int i = 0; i < lMeshBuffer->mMeshData->size(); i++) {
	//	//サブメッシュの数だけ作成
	//	mVertexBuffer[i].resize(lMeshBuffer->mMeshData->at(i)->subMesh.size());
	//	mIndexBuffer[i].resize(lMeshBuffer->mMeshData->at(i)->subMesh.size());
	//	mMeshVBDesc[i].resize(lMeshBuffer->mMeshData->at(i)->subMesh.size());
	//	mMeshIBDesc[i].resize(lMeshBuffer->mMeshData->at(i)->subMesh.size());

	//	//バッファデスクリプションはここで作成する
	//	for (unsigned int j = 0; j < lMeshBuffer->mMeshData->at(i)->subMesh.size(); j++) {
	//		{
	//			mMeshVBDesc[i][j].Usage = D3D11_USAGE_DEFAULT;
	//			mMeshVBDesc[i][j].ByteWidth = sizeof(FbxVertex)*lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->PosLength;
	//			mMeshVBDesc[i][j].BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//			mMeshVBDesc[i][j].CPUAccessFlags = 0;
	//			mMeshVBDesc[i][j].MiscFlags = 0;
	//		}
	//		{
	//			//インデックスバッファーデスクリプションを作成
	//			mMeshIBDesc[i][j].Usage = D3D11_USAGE_DEFAULT;
	//			mMeshIBDesc[i][j].ByteWidth = lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->IndexLength * sizeof(int);
	//			mMeshIBDesc[i][j].BindFlags = D3D11_BIND_INDEX_BUFFER;
	//			mMeshIBDesc[i][j].CPUAccessFlags = 0;
	//			mMeshIBDesc[i][j].MiscFlags = 0;
	//		}
	//	}
	//} 

	////アニメーションしない場合はこの時点でメッシュデータを作成しておく
	//if (true) {

	//	for (unsigned int i = 0; i < lMeshBuffer->mMeshData->size(); i++) {
	//		for (unsigned int j = 0; j < lMeshBuffer->mMeshData->at(i)->subMesh.size(); j++) {

	//			D3D11_SUBRESOURCE_DATA InitData;
	//			InitData.pSysMem = lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->Data.data();
	//			sDevice->CreateBuffer(&mMeshVBDesc[i][j], &InitData, &mVertexBuffer[i][j]);

	//			//インデックスバッファーを作成
	//			InitData.pSysMem = lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->Index;
	//			InitData.SysMemPitch = 0;
	//			InitData.SysMemSlicePitch = 0;
	//			sDevice->CreateBuffer(&mMeshIBDesc[i][j], &InitData, &mIndexBuffer[i][j]);
	//		}
	//	}
	//}



}

void MSFbxManager::RegisterMesh(FbxTime & mCurrentFrame, const int pAnimationIndex)
{
	//指定タイムにメッシュが存在する場合は生成しない


	long long l = mCurrentFrame.Get();
	if (mMeshBuffer.count(pAnimationIndex) != 0) {
		if (mMeshBuffer[pAnimationIndex].count(mCurrentFrame.Get()) != 0) {
			return;
		}
	}

	auto lMeshBuffer = &mMeshBuffer[pAnimationIndex][mCurrentFrame.Get()];
	mLoader->SetAnimation(pAnimationIndex);
	lMeshBuffer->mMeshData = mLoader->GetGeometryData2(mCurrentFrame);


	//インデックスバッファのそれぞれの長さを記憶する
	lMeshBuffer->mIndexLength.resize(lMeshBuffer->mMeshData->size());
	for (auto i = 0; i < lMeshBuffer->mMeshData->size(); i++) {
		lMeshBuffer->mIndexLength[i].resize(lMeshBuffer->mMeshData->at(i)->subMesh.size());
		for (auto j = 0; j < lMeshBuffer->mMeshData->at(i)->subMesh.size(); j++) {
			lMeshBuffer->mIndexLength[i][j] = lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->IndexLength;
		}
	}


	lMeshBuffer->mVertexBuffer.resize(lMeshBuffer->mMeshData->size());
	lMeshBuffer->mIndexBuffer.resize(lMeshBuffer->mMeshData->size());
	for (unsigned int i = 0; i < lMeshBuffer->mMeshData->size(); i++) {
		lMeshBuffer->mVertexBuffer[i].resize(lMeshBuffer->mMeshData->at(i)->subMesh.size());
		lMeshBuffer->mIndexBuffer[i].resize(lMeshBuffer->mMeshData->at(i)->subMesh.size());
		for (unsigned int j = 0; j < lMeshBuffer->mMeshData->at(i)->subMesh.size(); j++) {

			D3D11_BUFFER_DESC mVBDesc;
			D3D11_BUFFER_DESC mIBDesc;
			//頂点バッファ情報初期化
			mVBDesc.Usage = D3D11_USAGE_DEFAULT;
			mVBDesc.ByteWidth = sizeof(FbxVertex)*lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->PosLength;
			mVBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			mVBDesc.CPUAccessFlags = 0;
			mVBDesc.MiscFlags = 0;

			

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->Data.data();
			sDevice->CreateBuffer(&mVBDesc, &InitData, &lMeshBuffer->mVertexBuffer[i][j]);

			//インデックスバッファ情報初期化
			mIBDesc.Usage = D3D11_USAGE_DEFAULT;
			mIBDesc.ByteWidth = lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->IndexLength * sizeof(int);
			mIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			mIBDesc.CPUAccessFlags = 0;
			mIBDesc.MiscFlags = 0;
			//インデックスバッファーを作成
			InitData.pSysMem = lMeshBuffer->mMeshData->at(i)->subMesh.at(j)->Index;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;
			ID3D11Buffer* IB;
			sDevice->CreateBuffer(&mIBDesc, &InitData, &IB);
			lMeshBuffer->mIndexBuffer[i][j] = IB;
		}
	}

	//コリジョンメッシュの生成
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
		メッシュのインデックスに対応するように設計
	*/

	//コリジョンのベクタを作成
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
