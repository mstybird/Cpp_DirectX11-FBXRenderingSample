#include"MS3DRender.h"
#include"DX11TextureManager.hpp"
#include"DX11RenderResource.h"
#include"MSFbxManager.h"
#include<DX11FBXLoader.hpp>
#include"DXMath.hpp"
#include"MSBase3DShader.h"
#include<vector>
ID3D11Device*MS3DRender::sDevice;				//DirectX11�f�o�C�X
ID3D11DeviceContext*MS3DRender::sDeviceContext;	//DirectX11�f�o�C�X�R���e�L�X�g
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





void MS3DRender::Render(DX11RenderResource*resource)
{
	//shader->SetConstantBuffer1(resource,&display);
	//�V�F�[�_�̐ݒ�
	shader->GetVS()->SetShader();
	shader->GetPS()->SetShader();
	//�v���~�e�B�u�g�|���W�[�̓o�^
	sDeviceContext->IASetPrimitiveTopology(mPrimitiveTopology);

	

	auto meshData = resource->GetMesh()->GetCurrentMeshData();
	auto lVertexBufferArray = resource->GetMesh()->GetCurrentVertexBuffer();
	auto lIndexBufferArray = resource->GetMesh()->GetCurrentIndexBuffer();
	auto lIndexBufferLengthArray = resource->GetMesh()->GetCurrentIndexBufferCount();

	//auto meshData = fbxManager.lock()->GetMeshData();
	//���b�V���̌���
	for (unsigned int i = 0; i < lVertexBufferArray->size(); i++) {
		//���b�V���P�ʂ̐ݒ�
		shader->SetConstantBuffer1(*meshData->at(i), resource, display.get());
		//�S�Ẵ��b�V���ɑ΂��ċ��ʂ̃f�[�^��o�^
		sDeviceContext->VSSetConstantBuffers(0, 1, shader->GetCB1());
		sDeviceContext->PSSetConstantBuffers(0, 1, shader->GetCB1());
		//�T�u���b�V���̌���
		for (unsigned int j = 0; j < meshData->at(i)->subMesh.size(); j++) {
			shader->SetConstantBuffer2(meshData->at(i)->subMesh.at(j));
			sDeviceContext->VSSetConstantBuffers(1, 1, shader->GetCB2());
			sDeviceContext->PSSetConstantBuffers(1, 1, shader->GetCB2());
			UINT stride = shader->GetVertexSize();
			UINT offset = 0;
			ID3D11Buffer* lVertexBuffer = lVertexBufferArray->at(i)[j];

			sDeviceContext->IASetVertexBuffers(0, 1, &lVertexBuffer, &stride, &offset);
			stride = sizeof(int);
			offset = 0;
			sDeviceContext->IASetIndexBuffer(lIndexBufferArray->at(i)[j], DXGI_FORMAT_R32_UINT, 0);
			sDeviceContext->DrawIndexed(lIndexBufferLengthArray->at(i)[j], 0, 0);
			
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

void MS3DRender::SetRenderTarget(DX11RenderResource & resource)
{
	display->SetRenderTarget(
		resource.GetCamera(),
		resource.GetProjection()
	);
}

void MS3DRender::SetRenderTarget(const DXDisplay & aDisplay)
{
	*display = aDisplay;
}

void MS3DRender::GetDisplay(DXDisplay & aOutDisplay)
{
	aOutDisplay = *display;
}



void MS3DRender::SetShader(MSBase3DShader*pShader)
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