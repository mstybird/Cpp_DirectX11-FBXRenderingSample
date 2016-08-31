#include"DX11FbxLoader.h"

ColorChannel FbxMaterialCache::sEmissive;
ColorChannel FbxMaterialCache::sAmbient;
ColorChannel FbxMaterialCache::sDiffuse;
ColorChannel FbxMaterialCache::sSpecular;


FbxMaterialCache::FbxMaterialCache() :
	Shinness(0)
{

}

FbxMaterialCache::~FbxMaterialCache()
{
}

//�擾�ς݂̃}�e���A������F���擾
bool FbxMaterialCache::Initialize(const FbxSurfaceMaterial * pMaterial)
{
	//�G�~�b�V�u�̎擾
	const auto lEmissive = GetMaterialProperty(pMaterial, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor, Emissive.TextureName);
	Emissive.Color[0] = static_cast<FbxFloat>(lEmissive[0]);
	Emissive.Color[1] = static_cast<FbxFloat>(lEmissive[1]);
	Emissive.Color[2] = static_cast<FbxFloat>(lEmissive[2]);


	const FbxDouble3 lAmbient = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, Ambient.TextureName);
	Ambient.Color[0] = static_cast<FbxFloat>(lAmbient[0]);
	Ambient.Color[1] = static_cast<FbxFloat>(lAmbient[1]);
	Ambient.Color[2] = static_cast<FbxFloat>(lAmbient[2]);

	const FbxDouble3 lDiffuse = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, Diffuse.TextureName);
	Diffuse.Color[0] = static_cast<FbxFloat>(lDiffuse[0]);
	Diffuse.Color[1] = static_cast<FbxFloat>(lDiffuse[1]);
	Diffuse.Color[2] = static_cast<FbxFloat>(lDiffuse[2]);

	const FbxDouble3 lSpecular = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, Specular.TextureName);
	Specular.Color[0] = static_cast<FbxFloat>(lSpecular[0]);
	Specular.Color[1] = static_cast<FbxFloat>(lSpecular[1]);
	Specular.Color[2] = static_cast<FbxFloat>(lSpecular[2]);

	auto lShininessProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	if (lShininessProperty.IsValid()) {
		auto lShininess = lShininessProperty.Get<FbxDouble>();
		Shinness = static_cast<FbxFloat>(lShininess);
	}

	return true;
}

FbxDouble3 FbxMaterialCache::GetMaterialProperty(const FbxSurfaceMaterial * pMaterial, FbxString pPropertyName, FbxString pFactorPropertyName, FbxString & pTextureName)
{



	FbxDouble3 lResult(0, 0, 0);
	const FbxProperty lProperty = pMaterial->FindProperty(pPropertyName);
	const FbxProperty lFactorProperty = pMaterial->FindProperty(pFactorPropertyName);

	int layerNum = lProperty.GetSrcObjectCount();
	if (layerNum == 0) {
		//�e�N�X�`�������`�F�b�N
		int numGeneralTexture = lProperty.GetSrcObjectCount(FbxTexture::ClassId);
		//�e�e�N�X�`�����`�F�b�N
		std::vector<std::string>textureNames;
		for (int i = 0; i < numGeneralTexture; i++) {
			FbxTexture* lTexture = FbxCast<FbxTexture>(lProperty.GetSrcObject(FbxTexture::ClassId, i));
			auto lFileTexture = FbxCast<FbxFileTexture>(lTexture);

			textureNames.push_back(lFileTexture->GetFileName());
		}

		if (textureNames.size() != 0) {
			auto i = 10;
		}

	}



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

				pTextureName = *(static_cast<FbxString*>(lTexture->GetUserDataPtr()));
			}
		}
	}

	return lResult;
}

void FbxMaterialCache::SetCurrentMaterial(FBXModelData* pModelData)
{
	pModelData->Emissive = &Emissive;
	pModelData->Ambient = &Ambient;
	pModelData->Diffuse = &Diffuse;
	pModelData->Specular = &Specular;
	return;
}

void FbxMaterialCache::SetDefaultMaterial(FBXModelData * pModelData)
{
	pModelData->Emissive = &sEmissive;
	pModelData->Ambient = &sAmbient;
	pModelData->Diffuse = &sDiffuse;
	pModelData->Specular = &sSpecular;

}

