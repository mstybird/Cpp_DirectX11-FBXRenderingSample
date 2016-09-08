#include"DX11FbxLoader.h"

std::shared_ptr<ColorChannel> FbxMaterialCache::sEmissive;
std::shared_ptr<ColorChannel> FbxMaterialCache::sAmbient;
std::shared_ptr<ColorChannel> FbxMaterialCache::sDiffuse;
std::shared_ptr<ColorChannel> FbxMaterialCache::sSpecular;


FbxMaterialCache::FbxMaterialCache() :
	Shinness(0)
{
	Emissive = std::make_shared<ColorChannel>();
	Ambient = std::make_shared<ColorChannel>();
	Diffuse = std::make_shared<ColorChannel>();
	Specular = std::make_shared<ColorChannel>();
}

FbxMaterialCache::~FbxMaterialCache()
{
}

//取得済みのマテリアルから色を取得
bool FbxMaterialCache::Initialize(const FbxSurfaceMaterial * pMaterial, const std::string& pTextureRelativePathName)
{
	//エミッシブの取得
	const auto lEmissive = GetMaterialProperty(pMaterial, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor, Emissive->TextureName, pTextureRelativePathName);
	
	Emissive->Color[0] = static_cast<FbxFloat>(lEmissive[0]);
	Emissive->Color[1] = static_cast<FbxFloat>(lEmissive[1]);
	Emissive->Color[2] = static_cast<FbxFloat>(lEmissive[2]);
	Emissive->CreateTexture();

	const FbxDouble3 lAmbient = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, Ambient->TextureName, pTextureRelativePathName);
	Ambient->Color[0] = static_cast<FbxFloat>(lAmbient[0]);
	Ambient->Color[1] = static_cast<FbxFloat>(lAmbient[1]);
	Ambient->Color[2] = static_cast<FbxFloat>(lAmbient[2]);
	Ambient->CreateTexture();

	const FbxDouble3 lDiffuse = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, Diffuse->TextureName, pTextureRelativePathName);
	Diffuse->Color[0] = static_cast<FbxFloat>(lDiffuse[0]);
	Diffuse->Color[1] = static_cast<FbxFloat>(lDiffuse[1]);
	Diffuse->Color[2] = static_cast<FbxFloat>(lDiffuse[2]);
	Diffuse->CreateTexture();

	const FbxDouble3 lSpecular = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, Specular->TextureName, pTextureRelativePathName);
	Specular->Color[0] = static_cast<FbxFloat>(lSpecular[0]);
	Specular->Color[1] = static_cast<FbxFloat>(lSpecular[1]);
	Specular->Color[2] = static_cast<FbxFloat>(lSpecular[2]);
	Specular->CreateTexture();

	auto lShininessProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	if (lShininessProperty.IsValid()) {
		auto lShininess = lShininessProperty.Get<FbxDouble>();
		Shinness = static_cast<FbxFloat>(lShininess);
	}

	return true;
}

FbxDouble3 FbxMaterialCache::GetMaterialProperty(const FbxSurfaceMaterial * pMaterial, std::string pPropertyName, std::string pFactorPropertyName, std::string & pTextureName, const std::string& pTextureRelativePathName)
{



	FbxDouble3 lResult(0, 0, 0);
	const FbxProperty lProperty = pMaterial->FindProperty(pPropertyName.c_str());
	const FbxProperty lFactorProperty = pMaterial->FindProperty(pFactorPropertyName.c_str());



	if (lProperty.IsValid()&lFactorProperty.IsValid()) {
		lResult = lProperty.Get<FbxDouble3>();
		auto lFactor = lFactorProperty.Get<FbxDouble>();
		if (lFactor != 1) {
			lResult[0] *= lFactor;
			lResult[1] *= lFactor;
			lResult[2] *= lFactor;

		}
	}

	if (lProperty.IsValid()) {
		const auto lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
		if (lTextureCount) {
			const auto lTexture = lProperty.GetSrcObject<FbxFileTexture>();
			if (lTexture&&lTexture->GetUserDataPtr()) {

				pTextureName = *(static_cast<std::string*>(lTexture->GetUserDataPtr()));
			}
			else {
				pTextureName = lTexture->GetRelativeFileName();
				
				char str[256];
				char *pStr;
				FbxUTF8ToAnsi(pTextureName.c_str(), pStr);
				pTextureName = pTextureRelativePathName+"\\"+pStr;
			}
		}
	}

	return lResult;
}

void FbxMaterialCache::SetCurrentMaterial(const std::weak_ptr<FBXModelData>&pModelData)
{
	pModelData.lock()->Emissive = Emissive;
	pModelData.lock()->Ambient = Ambient;
	pModelData.lock()->Diffuse = Diffuse;
	pModelData.lock()->Specular = Specular;
	return;
}

void FbxMaterialCache::SetDefaultMaterial(const std::weak_ptr<FBXModelData>&pModelData)
{
	pModelData.lock()->Emissive = sEmissive;
	pModelData.lock()->Ambient = sAmbient;
	pModelData.lock()->Diffuse = sDiffuse;
	pModelData.lock()->Specular = sSpecular;

}
