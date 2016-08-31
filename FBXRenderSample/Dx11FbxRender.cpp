#include "Dx11FbxRender.h"

DX11FbxManager::~DX11FbxManager()
{
	Release();
}

void DX11FbxManager::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, ID3D11VertexShader*pVertexShader,
	ID3D11PixelShader*pPixelShader, ID3D11InputLayout*pVertexLayout)
{
	mDevice = pDevice;
	mDeviceContext = pDeviceContext;
	mVertexShader = pVertexShader;
	mPixelShader = pPixelShader;
	mVertexLayout = pVertexLayout;
	//�R���X�^���g�o�b�t�@�̏�����
	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(FBXCONSTANTBUFFER1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(mDevice->CreateBuffer(&cb, NULL, &mConstantBuffer1)))
	{
		//return E_FAIL;
	}

	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(FBXCONSTANTBUFFER2);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(mDevice->CreateBuffer(&cb, NULL, &mConstantBuffer2)))
	{
//		return E_FAIL;
	}
}

void DX11FbxManager::LoadFile(std::string pFileName)
{
	if (mLoader == nullptr) {
		mLoader = new DX11FbxLoader();
	}
	else {
		mLoader->FbxDestroy();
	}
	mLoader->FbxInit(pFileName);
}

void DX11FbxManager::Update()
{
	D3DXVECTOR3 transPos;
	mMeshData = mLoader->GetGeometryData2(&transPos);
	//�o�b�t�@�����݂����ꍇ��U���
	if (mVertexBuffer != nullptr) {
		for (int i = 0; i < mMeshData->size(); i++) {
			for (int j = 0; j < mMeshData->at(i).size(); j++) {
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
	mVertexBuffer = new ID3D11Buffer**[mMeshData->size()];
	mIndexBuffer = new ID3D11Buffer**[mMeshData->size()];

	for (int i = 0; i < mMeshData->size(); i++) {
		mVertexBuffer[i] = new ID3D11Buffer*[mMeshData->at(i).size()];
		mIndexBuffer[i] = new ID3D11Buffer*[mMeshData->at(i).size()];
		for (int j = 0; j < mMeshData->at(i).size(); j++) {
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex)*mMeshData->at(i).at(j)->PosLength;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = mMeshData->at(i).at(j)->Data.data();;
			mDevice->CreateBuffer(&bd, &InitData, &mVertexBuffer[i][j]);

			//�C���f�b�N�X�o�b�t�@�[���쐬
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(int) * mMeshData->at(i).at(j)->IndexLength;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			InitData.pSysMem = mMeshData->at(i).at(j)->Index;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;
			mDevice->CreateBuffer(&bd, &InitData, &mIndexBuffer[i][j]);
		}
	}

}

void DX11FbxManager::SetMatrix(D3DXMATRIX matrixW,
	D3DXMATRIX matrixV,
	D3DXMATRIX matrixP)
{
	D3D11_MAPPED_SUBRESOURCE pData;
	FBXCONSTANTBUFFER1 cb;
	if (SUCCEEDED(mDeviceContext->Map(mConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		cb.mW = matrixW;
		cb.mWVP = matrixW*matrixV*matrixP;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);
		D3DXMatrixTranspose(&cb.mWVP,&cb.mWVP);
		cb.LightDir = D3DXVECTOR4(1, 0, -1, 0);
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		mDeviceContext->Unmap(mConstantBuffer1, 0);

	}

}

void DX11FbxManager::Render()
{
	//�V�F�[�_�̐ݒ�
	mDeviceContext->VSSetShader(mVertexShader, NULL, 0);
	mDeviceContext->PSSetShader(mPixelShader, NULL, 0);

	//�S�Ẵ��b�V���ɑ΂��ċ��ʂ̃f�[�^��o�^
	mDeviceContext->VSSetConstantBuffers(0, 1, &mConstantBuffer1);
	mDeviceContext->PSSetConstantBuffers(0, 1, &mConstantBuffer1);
	//���_�C���v�b�g���C�A�E�g��o�^
	mDeviceContext->IASetInputLayout(mVertexLayout);
	//�v���~�e�B�u�g�|���W�[�̓o�^
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���b�V���̃����_�����O
	D3D11_MAPPED_SUBRESOURCE pData;
	FBXCONSTANTBUFFER2 cb;
	for (int i = 0; i < mMeshData->size(); i++) {
		for (int j = 0; j < mMeshData->at(i).size(); j++) {
			if (SUCCEEDED(mDeviceContext->Map(mConstantBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
			{
				cb.Diffuse = mMeshData->at(0).at(0)->Diffuse->Color;
				memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
				mDeviceContext->Unmap(mConstantBuffer2, 0);
			}

			//���̃R���X�^���g�o�b�t�@�[���A�ǂ̃V�F�[�_�[�Ŏg�������w��
			mDeviceContext->VSSetConstantBuffers(1, 1, &mConstantBuffer2);//�o�[�e�b�N�X�o�b�t�@�[�Ŏg��
			mDeviceContext->PSSetConstantBuffers(1, 1, &mConstantBuffer2);//�s�N�Z���V�F�[�_�[�ł̎g��




			UINT stride = sizeof(SimpleVertex);
			UINT offset = 0;
			mDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer[i][j], &stride, &offset);
			stride = sizeof(int);
			offset = 0;
			mDeviceContext->IASetIndexBuffer(mIndexBuffer[i][j], DXGI_FORMAT_R32_UINT, 0);
			mDeviceContext->DrawIndexed(mMeshData->at(i).at(j)->IndexLength, 0, 0);

		}
	}

}

void DX11FbxManager::Release()
{
	if (mLoader != nullptr) {
		delete mLoader;
		mLoader = nullptr;
	}
	if (mVertexBuffer != nullptr) {
		for (int i = 0; i < mMeshData->size(); i++) {
			for (int j = 0; j < mMeshData->at(i).size(); j++) {
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

}
