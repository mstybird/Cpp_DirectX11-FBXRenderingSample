#include "Graphic_Text.h"
#include<DX11TextureManager.hpp>
#include<locale>

ID3D11Device* TextGraphic::sDevice{};
ID3D11DeviceContext* TextGraphic::sDeviceContext{};


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

void TextGraphic::SetPosition(const DXVector2 & aPosition)
{
	mPosition = aPosition;
}

void TextGraphic::SetPosition(float aX, float aY)
{
	mPosition.Set(aX, aY);
}

void TextGraphic::AddPosition(const DXVector2 & aPosition)
{
	mPosition += aPosition;
}

void TextGraphic::AddPosition(float aX, float aY)
{
	mPosition.Add(aX, aY);
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
	float lStX{ mPosition.x };
	float lStY{ mPosition.y };

	for (const auto&lLine : mLineAry) {
		//行の左側
		lStX = mPosition.x;

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
		
		//文字単位で描画
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
	//ポリゴンそのものはローカル座標で作成
	//Pivotの値を使用して中心位置を調整する
	DXVector2 lPivotPos{ mSize->x*mPivot->x,mSize->y*mPivot->y };
	pPolygon[0] = {
		{ 0 - lPivotPos.x, 0 - lPivotPos.y,0 },{ mSplitImageX.x,mSplitImageY.x }	//左上
	};
	pPolygon[1] = {
		{ mSize->x - lPivotPos.x,0 - lPivotPos.y,0 },{ mSplitImageX.y,mSplitImageY.x }	//右上
	};
	pPolygon[2] = {
		{ 0 - lPivotPos.x,mSize->y - lPivotPos.y,0 },{ mSplitImageX.x,mSplitImageX.y }	//左下
	};
	pPolygon[3] = {
		{ mSize->x - lPivotPos.x,mSize->y - lPivotPos.y,0 },{ mSplitImageX.y,mSplitImageX.y }	//右下
	};


	//位置が確定
	//位置をずらす
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
		ポリゴン再生成でなんか直った
	
	*/

}

ID3D11Device* TextManager::sDevice{};
ID3D11DeviceContext* TextManager::sDeviceContext{};


TextManager::TextManager() :
	mDBFont{ std::make_unique<FontDatabase>() },
	mDBChar{ std::make_unique<CharDatabase>() }
{
}

TextManager::~TextManager()
{
}

void TextManager::SetDevice(ID3D11Device *& aDevice)
{
	sDevice = aDevice;
}

void TextManager::SetDeviceContext(ID3D11DeviceContext *& aDeviceContext)
{
	sDeviceContext = aDeviceContext;
}

bool TextManager::RegisterFont(const FontLog & aFontDescription, const int aID)
{
	bool lResult = mDBFont->Load(aFontDescription, aID);
	return lResult;
}

bool TextManager::PreLoadString(const std::string & aText)
{
	const int lFontSize = mDBFont->GetActiveFontDesc()->GetFontSize();
	const auto lHFont = mDBFont->GetActiveFontHFont();
	auto lHdc = GetDC(nullptr);
	auto lOldFont = static_cast<HFONT>(SelectObject(lHdc, lHFont));



	bool lResult = mDBChar->Load(lHdc, aText, mDBFont->GetActiveFontID(), lFontSize);

	SelectObject(lHdc, lOldFont);
	ReleaseDC(nullptr, lHdc);

	return lResult;
}

bool TextManager::SetFont(const int aID)
{
	
	bool lResult = mDBFont->Enable(aID);
	return lResult;
}

void TextManager::SetDefaultSize(const int aWidth, const int aHeight)
{
	mSize.Set(static_cast<float>(aWidth), static_cast<float>(aHeight));
}

std::shared_ptr<TextGraphic> TextManager::Create(const std::string& aText)
{
	auto lHdc = GetDC(nullptr);
	auto lText = std::make_shared<TextGraphic>();
	const FontLog* lFontDesc = mDBFont->GetActiveFontDesc();
	const int lFontID = mDBFont->GetActiveFontID();
	const auto lHFont = mDBFont->GetActiveFontHFont();
	
	auto lOldFont = static_cast<HFONT>(SelectObject(lHdc, *lHFont));

	mDBChar->Load(lHdc,aText,lFontID,lFontDesc->GetFontSize());

	
	//フォントビットマップ取得
	SelectObject(lHdc, lOldFont);
	ReleaseDC(nullptr, lHdc);


	auto lCharTextures = mDBChar->GetArray(aText, lFontID, lFontDesc->GetFontSize());
	//フォントサイズの登録
	clock_t start, end;
	start = clock();
	lText->mFontSize = lFontDesc->GetFontSize();
	//テキストの登録
	auto lTextLine = new TextureTextLine;
	for (auto&lChar : lCharTextures) {
		//TextureChar lChar;

		//auto& lTexture = lCharResource.first;
		//auto& lResource = lCharResource.second;

		//内部でのポリゴン生成を必須にしないようにする
		//lChar.SetTexture(lTexture);
		//lChar.SetSize({ static_cast<float>(lResource->Width),static_cast<float>(lResource->Height) });
		//lChar.mWidth = lResource->Width;
		//文字加算をしてはみ出すかチェック
		if (lTextLine->mWidth + lChar.mWidth > mSize.x) {
			//折り返す
			lText->mLineAry.push_back(lTextLine);
			lTextLine = new TextureTextLine;
		}

		lTextLine->push_back(std::move(lChar));
		lTextLine->mWidth += lChar.mWidth;

	}

	if (lTextLine->size()) {
		lText->mLineAry.push_back(lTextLine);
	}
	else {
		delete lTextLine;
	}
	end = clock();
	printf("%d\n", end - start);
	//一度文字列をデータベースに登録

	return std::move(lText);
}

FontDatabase::FontDatabase()
{
}

FontDatabase::~FontDatabase()
{
	this->Release();
}

bool FontDatabase::Load(const FontLog & aFontDescription, const int aID)
{
	//既にフォントが存在している場合は登録できない
	if (IsExist(aID))return false;

	HFONT lHFont = CreateFontIndirect(&aFontDescription);
	//作成に失敗すればfalse返して終わり
	if (!lHFont)return false;


	//フォント情報を登録
	mDb.insert({ aID,{aFontDescription,lHFont } });

	auto lHdc = GetDC(nullptr);
	auto lOldFont = static_cast<HFONT>(SelectObject(lHdc, mDb[aID].Font));
	TEXTMETRIC lTm;
	GetTextMetrics(lHdc, &lTm);


	return true;
}

void FontDatabase::Release()
{
	//論理フォントの解放
	for (auto& lFont : mDb) {
		DeleteObject(lFont.second.Font);
		lFont.second.Font = nullptr;
	}
}

bool FontDatabase::Enable(const int aID)
{
	bool lResult{};
	//フォントが存在するかチェック
	if (IsExist(aID)) {
		mActiveID = aID;
		lResult = true;
	}
	else {
		lResult = false;
	}
	return lResult;
}

bool FontDatabase::Disable()
{
	bool lResult{};
	//現在フォントが有効かチェック
	if (mActiveID != -1) {
		lResult = true;
		mActiveID = -1;
	}
	else {
		//無効だった場合は無効化できない(既に無効状態)
		lResult = false;
	}

	return lResult;
}

bool FontDatabase::IsExist(const int aID)
{
	return mDb.count(aID) > 0;
}

int FontDatabase::GetActiveFontID()
{
	int lFontID{ -1 };
	//フォントが有効ならそのIDを返す
	if (mActiveID != -1) {
		lFontID = mActiveID;
	}
	return lFontID;
}

FontLog * FontDatabase::GetActiveFontDesc()
{
	FontLog* lFontDesc{ nullptr };
	//フォントが有効ならそのIDを返す
	if (mActiveID != -1) {
		lFontDesc = &mDb[mActiveID].Desc;
	}
	return lFontDesc;
}

HFONT * FontDatabase::GetActiveFontHFont()
{
	HFONT* lHFont{ nullptr };
	//フォントが有効ならそのIDを返す
	if (mActiveID != -1) {
		lHFont = &mDb[mActiveID].Font;
	}
	return lHFont;
}


ID3D11Device* CharDatabase::sDevice{};
ID3D11DeviceContext* CharDatabase::sDeviceContext{};


void CharDatabase::SetDevice(ID3D11Device *& aDevice)
{
	sDevice = aDevice;
}

void CharDatabase::SetDeviceContext(ID3D11DeviceContext *& aDeviceContext)
{
	sDeviceContext = aDeviceContext;
}

bool CharDatabase::Load(HDC&aHandle,const std::string & aString, const int aFontID, const int aFontSize)
{
	uint32_t lCharCode{};

	auto lCharCodeArray = GetCharCodeArray(aString);
	//文字コードごとに文字生成
	for (const auto& lChar : lCharCodeArray) {
		//1文字単位で読み込み
		Load(aHandle,lChar, aFontID, aFontSize);
	}
	return true;
}

bool CharDatabase::Load(HDC&aHandle,const uint32_t aCharCode, const int aFontID, const int aFontSize)
{
	//既に読み込んでいるか調べる
	if (IsExist(aCharCode, aFontID, aFontSize) == true) {
		return true;
	}


	//フォントビットマップ取得
	TEXTMETRIC lTm;
	GetTextMetrics(aHandle, &lTm);
	GLYPHMETRICS lGm;
	const MAT2 lMat = {
		{ 0, 1 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 1 }
	};
	DWORD lSize = GetGlyphOutline(aHandle, aCharCode, GGO_GRAY4_BITMAP, &lGm, 0, nullptr, &lMat);
	//画像データ
	auto lPtr = new byte[lSize];

	//1文字分の画像を取得
	if (GDI_ERROR == GetGlyphOutline(aHandle, aCharCode, GGO_GRAY4_BITMAP, &lGm, lSize, lPtr, &lMat)) {
		return false;
	}

	//テクスチャ作成
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

	ID3D11Texture2D*lTex2D;
	auto lResult = sDevice->CreateTexture2D(&lDesc, nullptr, &lTex2D);

	if (FAILED(lResult)) {
		return false;
	}

	//フォント情報をテクスチャに書き込む
	D3D11_MAPPED_SUBRESOURCE lMappedResource;

	//フォント情報の書き込み
	int lOffsetX = lGm.gmptGlyphOrigin.x;
	int lOffsetY = lTm.tmAscent - lGm.gmptGlyphOrigin.y;
	int lBmpWidth = lGm.gmBlackBoxX;
	int lBmpHeight = lGm.gmBlackBoxY;
	//実際に確保されているビットマップの幅
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
			//α値はGGO_GRAY4_BITMAPが17階調(0~16)であるため、16で割る
			lAlpha = (0xFF * lPtr[lPixelX - lOffsetX + lBmpPitch*(lPixelY - lOffsetY)]) >> 4;
			lColor = 0x00FF'FFFF | (lAlpha << 24);
			memcpy(lBits + lMappedResource.RowPitch*lPixelY + 4 * lPixelX, &lColor, sizeof(DWORD));
		}
	}
	delete lPtr;
	sDeviceContext->Unmap(lTex2D, 0);


	//テクスチャ登録
	RegisterTexture(aFontID, aFontSize, aCharCode, lTex2D);
	//テクスチャ情報登録
	RegisterDescription(aFontID, aFontSize, aCharCode, lDesc.Width, lDesc.Height);

	TextureChar lChar;
	auto lResource = GetResource(aCharCode, aFontID, aFontSize);
	lChar.SetTexture(lResource.first);
	lChar.SetSize({ static_cast<float>(lResource.second->Width),static_cast<float>(lResource.second->Height) });
	lChar.mWidth = lResource.second->Width;
	lChar.CreateBuffer();
	RegisterChar(aFontID, aFontSize, aCharCode, lChar);

	if (lTex2D) {
		lTex2D->Release();
	}



	return true;
}

void CharDatabase::Release()
{
	for (auto&lTetureMap : mTextureManager) {
		for (auto&lTexture : lTetureMap.second) {
			lTexture.second.UnRegisterFileAll();
		}
	}
	mTextureManager.clear();
	mDescManager.clear();
	mCharManager.clear();
}

void CharDatabase::Release(const int aFontID, const int aFontSize)
{
	mTextureManager[aFontID][aFontSize].UnRegisterFileAll();
}

CharResource CharDatabase::GetResource(const uint32_t aCharCode, const int aFontID, const int aFontSize)
{
	CharResource lResource;
	mTextureManager[aFontID][aFontSize].Load(lResource.first, aCharCode);
	lResource.second = &mDescManager[aFontID][aFontSize];
	return lResource;
}

TextureChar CharDatabase::GetTextureChar(const uint32_t aCharCode, const int aFontID, const int aFontSize)
{
	return mCharManager[aFontID][aFontSize][aCharCode];
}

std::vector<TextureChar> CharDatabase::GetArray(const std::string & aText, const int aFontID, const int aFontSize)
{
	auto lCharCodeArray = GetCharCodeArray(aText);
	std::vector<TextureChar>lCharArray;
	for (auto&lCharCode : lCharCodeArray) {
		//1文字ごとにバッファを取得
		TextureChar lChar = GetTextureChar(lCharCode, aFontID, aFontSize);
		lCharArray.push_back(std::move(lChar));
	}
	
	return std::move(lCharArray);
}

bool CharDatabase::IsExist(const uint32_t aCharCode, const int aFontID, const int aFontSize)
{
	if (mTextureManager.count(aFontID) > 0) {
		if (mTextureManager[aFontID].count(aFontSize) > 0) {
			if (mTextureManager[aFontID][aFontSize].IsRegisterd(aCharCode)) {
				return true;
			}
		}
	}
	return false;
}

std::vector<uint32_t> CharDatabase::GetCharCodeArray(const std::string & aText)
{
	std::vector<uint32_t> lCodeArray;
	auto lText = aText.c_str();
#ifdef UNICODE
	uint32_t lStrNum = wcslen(aTxt);
	int lLen = 1;	//1文字が使用している配列数
#else
	//マルチバイトの時
	setlocale(LC_CTYPE, "jpn");
	uint32_t lStrNum = _mbstrlen(lText);
	int lLen;	//マルチバイトのときは1文字のバイト数は変動する
#endif
	uint32_t lCode;


	for (uint32_t c = 0; c < lStrNum; ++c) {


		//文字を作成
#ifdef UNICODE
		//lLen=1;	//UNICODEの場合は常に1固定
		lCode = (UINT)(*t);
#else
		//文字が使用するバイト数を取得
		lLen = mblen(lText, MB_CUR_MAX);
		switch (lLen)
		{
		case 1:
			lCode = static_cast<uint32_t>(*lText);
			break;
		case 2:
			lCode = static_cast<byte>(lText[0]) << 8 | static_cast<byte>(lText[1]);
			break;
		default:
			break;
		}
#endif
		lCodeArray.push_back(lCode);
		lText += lLen;
	}
	return std::move(lCodeArray);
}

void CharDatabase::RegisterTexture(const int aFontID, const int aFontSize, const uint32_t aCharCode,ID3D11Texture2D *& aTexture)
{
	mTextureManager[aFontID][aFontSize].RegistFromMemory(aTexture, aCharCode);

}

void CharDatabase::RegisterDescription(const int aFontID, const int aFontSize, const uint32_t aCharCode, const int aWidth, const int aHeight)
{
	mDescManager[aFontID][aFontSize].Width = aWidth;
	mDescManager[aFontID][aFontSize].Height = aHeight;
}

void CharDatabase::RegisterChar(const int aFontID, const int aFontSize, const uint32_t aCharCode, TextureChar & aCharTexture)
{
	mCharManager[aFontID][aFontSize][aCharCode] = std::move(aCharTexture);
}
