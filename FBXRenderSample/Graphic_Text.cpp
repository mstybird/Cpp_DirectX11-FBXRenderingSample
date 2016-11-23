#include "Graphic_Text.h"
#include<DX11TextureManager.hpp>
#include<locale>

const int TextureIDBaseKey{ 10000 };
ID3D11Device* TextGraphic::sDevice{};
ID3D11DeviceContext* TextGraphic::sDeviceContext{};

//1�������̃e�L�X�g���������e�N�X�`�����쐬
void TextGraphic::CreateTextureChar(
	TextureChar& aTexture,
	int& aTexWidGet,
	uint32_t aUChar,
	HFONT aFont,
	DWORD aBaseColor
) {
	ID3D11ShaderResourceView* lTexture{ nullptr };
	byte* lPtr{ nullptr };
	try {
		HDC lHdc = GetDC(nullptr);
		HFONT lOldFont = static_cast<HFONT>(SelectObject(lHdc, aFont));
		
		//�t�H���g�r�b�g�}�b�v�擾
		TEXTMETRIC lTm;
		GetTextMetrics(lHdc, &lTm);
		GLYPHMETRICS lGm;
		const MAT2 lMat = {
			{0, 1},
			{0, 0},
			{0, 0},
			{0, 1}
		};
		DWORD lSize = GetGlyphOutline(lHdc, aUChar, GGO_GRAY4_BITMAP, &lGm, 0, nullptr, &lMat);
		lPtr = new byte[lSize];

		//1�������̉摜���擾
		if (GDI_ERROR == GetGlyphOutline(lHdc, aUChar, GGO_GRAY4_BITMAP, &lGm, lSize, lPtr, &lMat)) {
			throw;
		}

		SelectObject(lHdc, lOldFont);
		ReleaseDC(nullptr, lHdc);

		//�e�N�X�`���쐬
		D3D11_TEXTURE2D_DESC lDesc{};
		lDesc.Width = lGm.gmCellIncX;
		lDesc.Height = lTm.tmHeight;
		lDesc.MipLevels = 1;
		lDesc.ArraySize = 1;
		lDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		lDesc.SampleDesc.Count = 1;
		lDesc.Usage = D3D11_USAGE_DYNAMIC;
		lDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		lDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


		aTexture.SetSize({ (float)lDesc.Width,(float)lDesc.Height });

		ID3D11Texture2D*lTex2D;
		auto lResult = sDevice->CreateTexture2D(&lDesc, nullptr, &lTex2D);

		if (FAILED(lResult)) {
			throw;
		}


		//�t�H���g�����e�N�X�`���ɏ�������
		D3D11_MAPPED_SUBRESOURCE lMappedResource;

		//�t�H���g���̏�������
		int lOffsetX = lGm.gmptGlyphOrigin.x;
		int lOffsetY = lTm.tmAscent - lGm.gmptGlyphOrigin.y;
		int lBmpWidth = lGm.gmBlackBoxX;
		int lBmpHeight = lGm.gmBlackBoxY;
		//���ۂɊm�ۂ���Ă���r�b�g�}�b�v�̕�
		int lBmpPitch = lGm.gmBlackBoxX + (4 - (lGm.gmBlackBoxX % 4)) % 4;

		int lPixelX;
		int lPixelY;
		DWORD lAlpha;
		DWORD lColor;

		lResult = sDeviceContext->Map(
			lTex2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &lMappedResource
		);
		byte* lBits = static_cast<byte*>(lMappedResource.pData);
		ZeroMemory(lBits, lMappedResource.RowPitch*lDesc.Height);
		for (lPixelY = lOffsetY; lPixelY < lOffsetY + lBmpHeight; ++lPixelY) {
			for (lPixelX = lOffsetX; lPixelX < lOffsetX + lBmpPitch; ++lPixelX) {
				//���l��GGO_GRAY4_BITMAP��17�K��(0~16)�ł��邽�߁A16�Ŋ���
				lAlpha = (0xFF * lPtr[lPixelX - lOffsetX + lBmpPitch*(lPixelY - lOffsetY)]) >> 4;
				lColor = aBaseColor | (lAlpha << 24);
				memcpy(lBits + lMappedResource.RowPitch*lPixelY + 4 * lPixelX, &lColor, sizeof(DWORD));
			}
		}
		sDeviceContext->Unmap(lTex2D, 0);
		
		if (!mTextManager.IsRegisterd(TextureIDBaseKey + aUChar)) {
			//�e�N�X�`���o�^
			mTextManager.RegistFromMemory(lTex2D, TextureIDBaseKey + aUChar);
		}
		aTexture.SetTexture(mTextManager, TextureIDBaseKey + aUChar);

//		D3DX11SaveTextureToFile(sDeviceContext, lTex2D, D3DX11_IFF_PNG, "sample.png");

		//D3D11_SHADER_RESOURCE_VIEW_DESC lSRVDesc{};
		//lSRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//lSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//lSRVDesc.Texture2D.MipLevels = 1;
		//sDevice->CreateShaderResourceView(lTex2D, &lSRVDesc, &aTexture.GetTexture()->mTexture);
		if (lTex2D) {
			lTex2D->Release();
		}

		aTexture.mWidth = lGm.gmCellIncX;

	}
	catch (...) {
		printf("Error");
	}

}

TextureTextLine::TextureTextLine()
{
	mAlign = AlignLeft;
	mWidth = 0;
}

TextureTextLine::~TextureTextLine()
{
}

TextGraphic::TextGraphic()
{
	mRect = { 0,0,0,0 };
	mFontSize = 10;
	mAlphaColor = 0xFFFF'FFFF;
}

TextGraphic::~TextGraphic()
{
	Release();
}

void TextGraphic::SetDevice(ID3D11Device *& aDevice)
{
	sDevice = aDevice;
}

void TextGraphic::SetDeviceContext(ID3D11DeviceContext *& aDeviceContext)
{
	sDeviceContext = aDeviceContext;
}

void TextGraphic::Create(LPCSTR aTxt, int aLeft, int aTop, int aRight, int aBottom, LOGFONT & aLogFont)
{
	Release();
	mRect.left = aLeft;
	mRect.top = aTop;
	mRect.right = aRight;
	mRect.bottom = aBottom;
	mFontSize = aLogFont.lfHeight;
	HFONT lFont = CreateFontIndirect(&aLogFont);
	
	if (!lFont)return;

	TCHAR* t = (TCHAR*)aTxt;
	TextureTextLine* lAddLine = new TextureTextLine();
#ifdef UNICODE
	uint32_t lStrNum = wcslen(aTxt);
	int lLen = 1;	//1�������g�p���Ă���z��
#else
	//�}���`�o�C�g�̎�
	setlocale(LC_CTYPE, "jpn");
	uint32_t lStrNum = _mbstrlen(aTxt);
	int lLen;	//�}���`�o�C�g�̂Ƃ���1�����̃o�C�g���͕ϓ�����
#endif

	uint32_t lCode;

	for (uint32_t c = 0; c < lStrNum; ++c) {
		TextureChar lAdd;
		if (*t == TEXT('\n')) {
			mLineAry.push_back(lAddLine);
			lAddLine = new TextureTextLine();
			++t;
			continue;
		}

		//�������쐬
#ifdef UNICODE
		//lLen=1;	//UNICODE�̏ꍇ�͏��1�Œ�
#else
		//�������g�p����o�C�g�����擾
		lLen = mblen(t, MB_CUR_MAX);
		switch (lLen)
		{
		case 1:
			lCode = static_cast<uint32_t>(*t);
			break;
		case 2:
			lCode = static_cast<byte>(t[0]) << 8 | static_cast<byte>(t[1]);
			break;
		default:
			break;
		}
#endif

		//�����F�͔��ō쐬(�|���S���̃f�B�t�[�Y�F���g���ē��߂��\
		CreateTextureChar(lAdd, lAdd.mWidth, lCode, lFont, 0x00'FF'00'FF);

		//���������Z����Ƃ͂ݏo�Ă��܂����ǂ���
		if (lAddLine->mWidth + lAdd.mWidth > mRect.GetWidth()) {
			//�܂�Ԃ�
			mLineAry.push_back(lAddLine);
			lAddLine = new TextureTextLine();
		}

		lAddLine->push_back(std::move(lAdd));
		lAddLine->mWidth += lAdd.mWidth;
		t += lLen;


	}



	if (lAddLine->size()) {
		mLineAry.push_back(lAddLine);
	}
	else {
		delete lAddLine;
	}
	DeleteObject(lFont);


}


void TextGraphic::SetTextAlign(TextAlign aAlign)
{
	if (mLineAry.size() < 0)return;
	for (auto& lLine : mLineAry) {
		lLine->mAlign = aAlign;
	}
}

void TextGraphic::SetTextAlign(uint32_t aLineIdx, uint32_t aLineIdxTo, TextAlign aAlign)
{
	if (mLineAry.size() >= aLineIdx) {
		return;
	}
	if (mLineAry.size() >= aLineIdxTo) {
		return;
	}
	for (unsigned int i = aLineIdx; i <= aLineIdxTo; i++) {
		mLineAry[i]->mAlign = aAlign;
	}
}

void TextGraphic::SetTextAlpha(DWORD aAlpha)
{
	mAlphaColor = aAlpha;
}

void TextGraphic::Release()
{
	for (auto&lLine : mLineAry) {
		lLine->clear();
		delete lLine;
	}
	mLineAry.clear();
}

void TextGraphic::Render(MSSprite2DRender & aRender)
{
	float lStX = mRect.left;
	float lStY = mRect.top;
	int lCellWidth;

	for (const auto&lLine : mLineAry) {
		//�s�̍���
		lStX = mRect.left;

		switch (lLine->mAlign)
		{
		case AlignLeft:
			break;
		case AlignRight:
			lStX += (mRect.GetWidth() - lLine->mWidth);
			break;
		case AlignCenter:
			lStX += (mRect.GetWidth() - lLine->mWidth) >> 1;
			break;
		default:
			break;
		}
		
		//�����P�ʂŕ`��
		for (auto&lChar : *lLine) {
			lChar.SetPosition({ lStX,lStY });
//			lChar.SetSplitSizeX({ 0.0f,1.0f });

			aRender.Render(lChar);
			lStX += lChar.mWidth;
		}
		lStY += mFontSize;
	}

}


void TextureChar::CreatePolygon(SpriteVertex pPolygon[4])
{
	//�|���S�����̂��̂̓��[�J�����W�ō쐬
	//Pivot�̒l���g�p���Ē��S�ʒu�𒲐�����
	DXVector2 lPivotPos{ mSize->x*mPivot->x,mSize->y*mPivot->y };
	pPolygon[0] = {
		{ 0 - lPivotPos.x, 0 - lPivotPos.y,0 },{ mSplitImageX.x,mSplitImageY.x }	//����
	};
	pPolygon[1] = {
		{ mSize->x - lPivotPos.x,0 - lPivotPos.y,0 },{ mSplitImageX.y,mSplitImageY.x }	//�E��
	};
	pPolygon[2] = {
		{ 0 - lPivotPos.x,mSize->y - lPivotPos.y,0 },{ mSplitImageX.x,mSplitImageX.y }	//����
	};
	pPolygon[3] = {
		{ mSize->x - lPivotPos.x,mSize->y - lPivotPos.y,0 },{ mSplitImageX.y,mSplitImageX.y }	//�E��
	};


	//�ʒu���m��
	//�ʒu�����炷
	pPolygon[0].Pos.x += mSize->x*mSplitPolygonX.x;
	pPolygon[0].Pos.y += mSize->y*mSplitPolygonY.x;

	pPolygon[1].Pos.x -= mSize->x*(1.0f - mSplitPolygonX.y);
	pPolygon[1].Pos.y += mSize->y*mSplitPolygonY.x;

	pPolygon[2].Pos.x += mSize->x*mSplitPolygonX.x;
	pPolygon[2].Pos.y -= mSize->y*(1.0f - mSplitPolygonY.y);

	pPolygon[3].Pos.x -= mSize->x*(1.0f - mSplitPolygonX.y);
	pPolygon[3].Pos.y -= mSize->y*(1.0f - mSplitPolygonY.y);

	SpriteVertex aPolygon[4];
	aPolygon[0] = pPolygon[0];
	aPolygon[1] = pPolygon[1];
	aPolygon[2] = pPolygon[2];
	aPolygon[3] = pPolygon[3];

	/*
		�|���S���Đ����łȂ񂩒�����
	
	*/

}
