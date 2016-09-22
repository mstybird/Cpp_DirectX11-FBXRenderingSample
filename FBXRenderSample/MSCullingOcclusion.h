#pragma once
#include<d3dx11.h>
class MSCullingOcculusion {
public:
	bool IsCulling();
	MSCullingOcculusion Create();
private:
	//�r���[�̍쐬���K�{�ł��邽�߁A
	//�R���X�g���N�^�ł�����s���������A
	//D3D11�������O�ɃR���X�g���N�^�����s������
	//�r���[�̍쐬���ł��Ȃ�(�ň��A�N�Z�X�ᔽ�ŗ�����)
	MSCullingOcculusion();
	//�J�����O�Ŏg�p����
	ID3D11RenderTargetView*mRTV;
	ID3D11DepthStencilView*mDSV;
	ID3D11Texture2D*mDS2D;

};