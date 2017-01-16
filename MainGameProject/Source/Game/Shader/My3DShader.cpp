#include"My3DShader.h"
#include"DX11Resrouce.h"
#include"DX11RenderResource.h"
#include<DX11FBXLoader.hpp>
#include"DX11TextureManager.hpp"
#include"CBResource0.h"
#include"GameObjectBase.h"
#include"MS3DRender.h"
#include<DXDisplay.h>
#include<vector>
#include<iostream>
My3DShader::My3DShader() :
	MSBase3DShader(
		sizeof(MyFBXCONSTANTBUFFER0),
		sizeof(MyFBXCONSTANTBUFFER1),
		sizeof(MyFBXCONSTANTBUFFER2),
		sizeof(FbxVertex))
{
}

const D3DXVECTOR3 gLightDir{ 1,1,0 };


void My3DShader::Init()
{
	MSBase3DShader::Init();

	D3D11_DEPTH_STENCIL_DESC dc;
	dc.DepthEnable = true;
	dc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	dc.DepthFunc = D3D11_COMPARISON_LESS;

	dc.StencilEnable = true;
	dc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	dc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	dc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	dc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dc.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR;
	dc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	sDevice->CreateDepthStencilState(&dc, &mVolume.mDSState1);

	// ステンシル利用時の挙動を定義するレンダリングステートのオブジェクトを作成
	dc.DepthEnable = false;
	dc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dc.DepthFunc = D3D11_COMPARISON_LESS;
	//ステンシルテスト
	dc.StencilEnable = true;
	dc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dc.StencilWriteMask = 0x00;
	//表面をどうするか
	dc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	dc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
	//裏面をどうするか
	dc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	dc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dc.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;

	sDevice->CreateDepthStencilState(&dc, &mVolume.mDSState2);

	//ボリュームメッシュ用のバッファ作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(FbxVertex) * 100000;//10万頂点まで対応　大きめに作る
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	if (FAILED(sDevice->CreateBuffer(&bd, NULL, &mVolume.mVolumeMeshBuffer))) {
		return;
	}

	//画面前面のポリゴン用　バーテックスバッファーを作成
	FbxVertex VertexData[] =
	{
		D3DXVECTOR3(-1,-1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,0),
		D3DXVECTOR3(-1,1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,0),
		D3DXVECTOR3(1,-1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,0),
		D3DXVECTOR3(1,1,0),D3DXVECTOR3(0,0,-1),D3DXVECTOR2(0,0)
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(FbxVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = VertexData;
	if (FAILED(sDevice->CreateBuffer(&bd, &InitData, &mVolume.mVertexBuffer))) {
		return;
	}


}

void My3DShader::SetConstantBuffer0(CBResource0 & aResource)
{
	MyFBXCONSTANTBUFFER0 cb;
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {

		cb.vDirectLightDir = aResource.vLightDir;
		D3DXVec4Normalize(&cb.vDirectLightDir, &cb.vDirectLightDir);
		cb.vEye = aResource.vEye;
		cb.vEye.w = 0;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer0, 0);
	}
}

void My3DShader::SetConstantBuffer1(
	FBXMesh& fbxMesh,
	DX11RenderResource* resource,
	DXDisplay* pDisplay
)
{
	MyFBXCONSTANTBUFFER1 cb;
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
		
		cb.mW = *fbxMesh.mWorld * *resource->GetMatrixWorld();
		resource->GetMatrixWVP(&cb.mWVP,*fbxMesh.mWorld,*pDisplay);
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer1, 0);
	}
}


void My3DShader::SetConstantBuffer2(std::weak_ptr<FBXModelData> modelData)
{
	D3D11_MAPPED_SUBRESOURCE pData;

	ID3D11SamplerState*lSampler{ nullptr };
	ID3D11ShaderResourceView*lTexture{ nullptr };
	//テクスチャがあった場合のみセットする
	if (modelData.lock()->Diffuse.lock()->mTexture) {
		lSampler = modelData.lock()->Diffuse.lock()->mTexture->GetSampler();
		lTexture = modelData.lock()->Diffuse.lock()->mTexture->GetTexture();
	}
	else {
	}
	//cb.Ambient = modelData.lock()->Ambient.lock()->Color;
	//cb.Diffuse = modelData.lock()->Diffuse.lock()->Color;
	//cb.Specular = modelData.lock()->Specular.lock()->Color;

	MyFBXCONSTANTBUFFER2 cb;
	cb.Ambient = D3DXVECTOR4( 0.3,0.3,0.3,1.0 );
	cb.Diffuse = D3DXVECTOR4(1,1,1,0);
	cb.Specular = D3DXVECTOR4(0.2,0.2,0.2,1.0);

	if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {
		memcpy(pData.pData,(void*)(&cb), sizeof(cb));
		sDeviceContext->Unmap(mConstantBuffer2, 0);

	}
	sDeviceContext->PSSetSamplers(0, 1, &lSampler);
	sDeviceContext->PSSetShaderResources(0, 1, &lTexture);
}

void My3DShader::CustomRender(MS3DRender* aRender,GameObjectBase * aObject,const int aIndexX,const int aIndexY)
{
	//使用するメッシュ
	FBXModelData* lMesh = aObject->GetTransform()->GetMesh()->GetCurrentMeshData()->at(aIndexY)->subMesh.at(aIndexX).get();
	D3DXMATRIX* mObjectWorld = aObject->GetTransform()->GetWorld()->GetMatrix();
	D3DXMATRIX* lFbxWorld = aObject->GetTransform()->GetMesh()->GetCurrentMeshData()->at(aIndexY)->mWorld.get();
	D3DXMATRIX mWorld = *mObjectWorld * *lFbxWorld;
	//CreateVolume
	{
		mVolume.mNumVolumeVertices = 0;
		FbxVertex vVertices[5];
		std::vector<int>lEdges;
		lEdges.resize(lMesh->PolygonCount * 6);
		
		DXMatrix mInv;
		D3DXVECTOR3 lLightDir=aObject->GetFrameResource()->vLightDir;
		D3DXVec3Normalize(&lLightDir, &lLightDir);

		D3DXMATRIX mRotateZ;
		D3DXMATRIX mTrans;
		D3DXMatrixTranslation(&mTrans, mWorld._41, mWorld._42, mWorld._43);

		if (lLightDir.x == 0) {

		}
		else if (lLightDir.y == 0) {
			D3DXMatrixRotationZ(&mRotateZ, D3DXToRadian(aObject->GetTransform()->GetWorld()->mRotationCenter.y));

		}
		else if (lLightDir.z == 0) {
			D3DXMatrixTranslation(&mTrans, mWorld._41, mWorld._43, mWorld._42);
			D3DXMatrixRotationY(&mRotateZ, D3DXToRadian(aObject->GetTransform()->GetWorld()->mRotationCenter.y));
		}


		D3DXMatrixInverse(&mInv, nullptr, &(mRotateZ));
		mInv._41 = 0.0f;
		mInv._42 = 0.0f;
		mInv._43 = 0.0f;

		D3DXVec3TransformCoord(&lLightDir, &lLightDir, &mInv);

		DWORD lNumEdges = 0;

		for (int i = 0; i < lMesh->PolygonCount; ++i) {
			WORD wPolyIndex0 = lMesh->Index[3 * i + 0];
			WORD wPolyIndex1 = lMesh->Index[3 * i + 1];
			WORD wPolyIndex2 = lMesh->Index[3 * i + 2];
			vVertices[0].Pos = lMesh->Data[wPolyIndex0].Pos;
			vVertices[1].Pos = lMesh->Data[wPolyIndex1].Pos;
			vVertices[2].Pos = lMesh->Data[wPolyIndex2].Pos;

			D3DXVECTOR3 vNormal;
			D3DXVECTOR3 vCross1(vVertices[2].Pos - vVertices[1].Pos);
			D3DXVECTOR3 vCross2(vVertices[1].Pos - vVertices[0].Pos);
			D3DXVec3Cross(&vNormal, &vCross1, &vCross2);

			if (D3DXVec3Dot(&vNormal, &lLightDir) <= 0)//裏側のポリゴン全部が該当してしまう。最適化の余地あり
			{
				lEdges[2 * lNumEdges] = wPolyIndex0;
				lEdges[2 * lNumEdges + 1] = wPolyIndex1;

				lEdges[2 * lNumEdges + 2] = wPolyIndex1;
				lEdges[2 * lNumEdges + 3] = wPolyIndex2;

				lEdges[2 * lNumEdges + 4] = wPolyIndex2;
				lEdges[2 * lNumEdges + 5] = wPolyIndex0;

				lNumEdges += 3;
			}


		}

		D3D11_MAPPED_SUBRESOURCE lData;
		sDeviceContext->Map(mVolume.mVolumeMeshBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &lData);

		lLightDir = -lLightDir;
		FbxVertex*lVertices = (FbxVertex*)lData.pData;
		for (int i = 0; i < lNumEdges; ++i) {
			memcpy(&vVertices[1], &lMesh->Data[lEdges[2 * i]], sizeof(FbxVertex) * 2);
			vVertices[1].Pos = lMesh->Data[lEdges[2 * i]].Pos;
			vVertices[2].Pos = lMesh->Data[lEdges[2 * i + 1]].Pos;
			vVertices[3].Pos = vVertices[1].Pos + lLightDir * 10000;
			vVertices[4].Pos = vVertices[2].Pos + lLightDir * 10000;

			memcpy(&lVertices[mVolume.mNumVolumeVertices], &vVertices[1], sizeof(FbxVertex) * 3);

			lVertices[mVolume.mNumVolumeVertices + 3].Pos = vVertices[2].Pos;
			lVertices[mVolume.mNumVolumeVertices + 4].Pos = vVertices[4].Pos;
			lVertices[mVolume.mNumVolumeVertices + 5].Pos = vVertices[3].Pos;
			mVolume.mNumVolumeVertices += 6;
		}

		sDeviceContext->Unmap(mVolume.mVolumeMeshBuffer, 0);

	}

	//RenderVolumeToStencil
	{
		//現在の設定を退避
		ID3D11DepthStencilState* lCurrentDSState{ nullptr };
		uint32_t lNum{ 0 };
		sDeviceContext->OMGetDepthStencilState(&lCurrentDSState, &lNum);
		//レンダリングステート変更
		sDeviceContext->OMSetDepthStencilState(mVolume.mDSState1, 1);
		//シェーダのコンスタントバッファを渡す




		MyFBXCONSTANTBUFFER2 cb2;
		cb2.Ambient = D3DXVECTOR4(0, 0, 0, 1);
		cb2.Diffuse = D3DXVECTOR4(0, 0, 0, 1);
		cb2.Specular = D3DXVECTOR4(0, 0, 0, 1);
		D3D11_MAPPED_SUBRESOURCE lData;
		if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &lData))) {
			memcpy(lData.pData, (void*)(&cb2), sizeof(cb2));
			sDeviceContext->Unmap(mConstantBuffer2, 0);
		}

		uint32_t stride = sizeof(FbxVertex);
		uint32_t offset = 0;
		sDeviceContext->IASetVertexBuffers(0, 1, &mVolume.mVolumeMeshBuffer, &stride, &offset);
		sDeviceContext->Draw(mVolume.mNumVolumeVertices, 0);

		sDeviceContext->OMSetDepthStencilState(lCurrentDSState, lNum);
	}

	//RenderShadow
	{
		ID3D11DepthStencilState* lCurrentDSState = nullptr;
		UINT lNum = 0;
		sDeviceContext->OMGetDepthStencilState(&lCurrentDSState, &lNum);
		sDeviceContext->OMSetDepthStencilState(mVolume.mDSState2, 0);
		

		MyFBXCONSTANTBUFFER1 cb1;
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData))) {

			//ワールド行列を渡す
			D3DXMATRIX m;
			D3DXMatrixIdentity(&m);
			D3DXMatrixTranspose(&m, &m);
			cb1.mWVP = m;
			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb1), sizeof(cb1));
			sDeviceContext->Unmap(mConstantBuffer1, 0);
		}



		//シェーダのコンスタントバッファを渡す
		MyFBXCONSTANTBUFFER2 cb2;
		cb2.Ambient = D3DXVECTOR4(0.2, 0.2, 0.2, 0);
		cb2.Diffuse = D3DXVECTOR4(0.2, 0.2, 0.2, 0.7);
		cb2.Specular = D3DXVECTOR4(0.2, 0.2, 0.2, 0);
		D3D11_MAPPED_SUBRESOURCE lData;
		if (SUCCEEDED(sDeviceContext->Map(mConstantBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &lData))) {
			memcpy(lData.pData, (void*)(&cb2), sizeof(cb2));
			sDeviceContext->Unmap(mConstantBuffer2, 0);
		}

		//プリミティブ・トポロジーをセット
		sDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//バーテックスバッファーをセット
		UINT stride = sizeof(FbxVertex);
		UINT offset = 0;
		sDeviceContext->IASetVertexBuffers(0, 1, &mVolume.mVertexBuffer, &stride, &offset);

		sDeviceContext->Draw(4, 0);

		// レンダリングステートを元の状態に戻す
		sDeviceContext->OMSetDepthStencilState(lCurrentDSState, lNum);


	}


	return;
}

void My3DShader::SetLayout(std::vector<D3D11_INPUT_ELEMENT_DESC>&pLayout)
{
	pLayout.push_back(INPUTLAYOUT_POSITION3D(0));
	pLayout.push_back(INPUTLAYOUT_NORMAL(12));
	pLayout.push_back(INPUTLAYOUT_TEXCOORD2D(24));
}
