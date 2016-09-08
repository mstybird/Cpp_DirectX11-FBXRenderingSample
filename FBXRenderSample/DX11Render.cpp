#include"DX11Render.h"
#include"DX11Texture.h"
#include"DX11RenderResource.h"
#include"DX11FbxManager.h"
#include"DX11FbxResource.h"
#include"DXDisplay.h"
#include"DXCamera.h"
#include"DXProjection.h"
#include"DX11BaseShader.h"

ID3D11Device*DX11Render::sDevice;				//DirectX11デバイス
ID3D11DeviceContext*DX11Render::sDeviceContext;	//DirectX11デバイスコンテキスト
ID3D11RenderTargetView*DX11Render::sRenderTargetView;
ID3D11DepthStencilView*DX11Render::sDepthStencilView;

void DX11Render::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, ID3D11RenderTargetView*pRenderTargetView,
	ID3D11DepthStencilView*pDepthStencilView)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
	sRenderTargetView = pRenderTargetView;
	sDepthStencilView = pDepthStencilView;
}

void DX11Render::Clear(D3DXVECTOR4 pColor)
{
	sDeviceContext->ClearRenderTargetView(sRenderTargetView, pColor);
	sDeviceContext->ClearDepthStencilView(sDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}





void DX11Render::Render(DX11FbxManager * fbxManager, DX11RenderResource * resource)
{
	//shader->SetConstantBuffer1(resource,&display);
	//シェーダの設定
	shader->GetVS()->SetShader();
	shader->GetPS()->SetShader();
	//プリミティブトポロジーの登録
	sDeviceContext->IASetPrimitiveTopology(mPrimitiveTopology);

	auto meshData = fbxManager->GetMeshData();
	//メッシュの個数分
	for (unsigned int i = 0; i < meshData->size(); i++) {
		//メッシュ単位の設定
		shader->SetConstantBuffer1(meshData->at(i), resource, display);
		//全てのメッシュに対して共通のデータを登録
		sDeviceContext->VSSetConstantBuffers(0, 1, shader->GetCB1());
		sDeviceContext->PSSetConstantBuffers(0, 1, shader->GetCB1());
		//サブメッシュの個数分
		for (unsigned int j = 0; j < meshData->at(i)->subMesh.size(); j++) {
			shader->SetConstantBuffer2(meshData->at(i)->subMesh.at(j));
			sDeviceContext->VSSetConstantBuffers(1, 1, shader->GetCB1());
			sDeviceContext->PSSetConstantBuffers(1, 1, shader->GetCB1());
			UINT stride = shader->GetVertexSize();
			UINT offset = 0;
			ID3D11Buffer* lVertexBuffer = fbxManager->GetVertexBuffer(i, j);
			ID3D11Buffer* lIndexBuffer = fbxManager->GetIndexBuffer(i, j);
			unsigned int* indexLength = fbxManager->GetIndexBufferCount(i, j);
			sDeviceContext->IASetVertexBuffers(0, 1, &lVertexBuffer, &stride, &offset);
			stride = sizeof(int);
			offset = 0;
			sDeviceContext->IASetIndexBuffer(lIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			sDeviceContext->DrawIndexed(*indexLength, 0, 0);

		}
	}

}

void DX11Render::SetRenderTarget(DX11RenderResource * resource)
{
	display->SetRenderTarget(
		resource->GetCamera(),
		resource->GetProjection()
		);
}

void DX11Render::SetShader(DX11BaseShader * pShader)
{
	shader = pShader;
}

DX11Render::DX11Render():
	mPrimitiveTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST }
{
	display = new DXDisplay;
}

DX11Render::~DX11Render()
{
}
