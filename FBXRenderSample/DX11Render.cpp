#include"DX11Render.h"

ID3D11Device*DX11Render::sDevice;				//DirectX11�f�o�C�X
ID3D11DeviceContext*DX11Render::sDeviceContext;	//DirectX11�f�o�C�X�R���e�L�X�g

void DX11Render::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	sDevice = pDevice;
	sDeviceContext = pDeviceContext;
}

void DX11Render::Render(DX11FbxManager * fbxManager, DX11RenderResource * resource)
{
	shader->SetConstantBuffer1(resource);
	//�V�F�[�_�̐ݒ�
	sDeviceContext->VSSetShader(shader->mVertexShader, NULL, 0);
	sDeviceContext->PSSetShader(shader->mPixelShader, NULL, 0);

	//�S�Ẵ��b�V���ɑ΂��ċ��ʂ̃f�[�^��o�^
	sDeviceContext->VSSetConstantBuffers(0, 1, &shader->mConstantBuffer1);
	sDeviceContext->PSSetConstantBuffers(0, 1, &shader->mConstantBuffer1);
	//���_�C���v�b�g���C�A�E�g��o�^
	sDeviceContext->IASetInputLayout(shader->mVertexLayout);
	//�v���~�e�B�u�g�|���W�[�̓o�^
	sDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto meshData = fbxManager->GetMeshData();
	for (unsigned int i = 0; i < meshData->size(); i++) {
		for (unsigned int j = 0; j < meshData->at(i).size(); j++) {
			shader->SetConstantBuffer2(meshData->at(i).at(j));
			sDeviceContext->VSSetConstantBuffers(1, 1, &shader->mConstantBuffer2);
			sDeviceContext->PSSetConstantBuffers(1, 1, &shader->mConstantBuffer2);
			UINT stride = sizeof(SimpleVertex);
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
