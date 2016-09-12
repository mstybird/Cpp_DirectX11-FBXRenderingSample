#include"MS3DRender.h"
#include"DX11Texture.h"
#include"DX11RenderResource.h"
#include"MSFbxManager.h"
#include"DX11FbxResource.h"
#include"DXDisplay.h"
#include"DXCamera.h"
#include"DXProjection.h"
#include"MSBase3DShader.h"
#include<vector>
ID3D11Device*MS3DRender::sDevice;				//DirectX11デバイス
ID3D11DeviceContext*MS3DRender::sDeviceContext;	//DirectX11デバイスコンテキスト
ID3D11RenderTargetView*MS3DRender::sRenderTargetView;
ID3D11DepthStencilView*MS3DRender::sDepthStencilView;

void MS3DRender::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, ID3D11RenderTargetView*pRenderTargetView,
	ID3D11DepthStencilView*pDepthStencilView)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
	sRenderTargetView = pRenderTargetView;
	sDepthStencilView = pDepthStencilView;
}

void MS3DRender::Clear(D3DXVECTOR4 pColor)
{
	sDeviceContext->ClearRenderTargetView(sRenderTargetView, pColor);
	sDeviceContext->ClearDepthStencilView(sDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}





void MS3DRender::Render(const std::weak_ptr<MSFbxManager>fbxManager, const std::weak_ptr<DX11RenderResource>resource)
{
	//shader->SetConstantBuffer1(resource,&display);
	//シェーダの設定
	shader.lock()->GetVS()->SetShader();
	shader.lock()->GetPS()->SetShader();
	//プリミティブトポロジーの登録
	sDeviceContext->IASetPrimitiveTopology(mPrimitiveTopology);

	auto meshData = fbxManager.lock()->GetMeshData();
	//メッシュの個数分
	for (unsigned int i = 0; i < meshData->size(); i++) {
		//メッシュ単位の設定
		shader.lock()->SetConstantBuffer1(meshData->at(i), resource, display);
		//全てのメッシュに対して共通のデータを登録
		sDeviceContext->VSSetConstantBuffers(0, 1, shader.lock()->GetCB1());
		sDeviceContext->PSSetConstantBuffers(0, 1, shader.lock()->GetCB1());
		//サブメッシュの個数分
		for (unsigned int j = 0; j < meshData->at(i)->subMesh.size(); j++) {
			shader.lock()->SetConstantBuffer2(meshData->at(i)->subMesh.at(j));
			sDeviceContext->VSSetConstantBuffers(1, 1, shader.lock()->GetCB2());
			sDeviceContext->PSSetConstantBuffers(1, 1, shader.lock()->GetCB2());
			UINT stride = shader.lock()->GetVertexSize();
			UINT offset = 0;
			ID3D11Buffer* lVertexBuffer = fbxManager.lock()->GetVertexBuffer(i, j);
			ID3D11Buffer* lIndexBuffer = fbxManager.lock()->GetIndexBuffer(i, j);
			unsigned int* indexLength = fbxManager.lock()->GetIndexBufferCount(i, j);
			sDeviceContext->IASetVertexBuffers(0, 1, &lVertexBuffer, &stride, &offset);
			stride = sizeof(int);
			offset = 0;
			sDeviceContext->IASetIndexBuffer(lIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			sDeviceContext->DrawIndexed(*indexLength, 0, 0);

		}
	}

}


void MS3DRender::SetRenderTarget(const std::weak_ptr<DX11RenderResource> resource)
{
	display->SetRenderTarget(
		resource.lock()->GetCamera(),
		resource.lock()->GetProjection()
		);
}



void MS3DRender::SetShader(const std::shared_ptr<MSBase3DShader> pShader)
{
	shader = pShader;
}



MS3DRender::MS3DRender():
	mPrimitiveTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST },
	display{std::make_shared<DXDisplay>()}
{
}

MS3DRender::~MS3DRender()
{

}
