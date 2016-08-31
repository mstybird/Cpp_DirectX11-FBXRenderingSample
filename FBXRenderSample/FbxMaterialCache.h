#pragma once
class FbxMaterialCache {
public:
	FbxMaterialCache();
	~FbxMaterialCache();
	bool Initialize(const FbxSurfaceMaterial*pMaterial);
	static FbxDouble3 GetMaterialProperty(
		const FbxSurfaceMaterial*pMaterial,
		FbxString pPropertyName,
		FbxString pFactorPropertyName,
		FbxString&pTextureName
	);
	void SetCurrentMaterial(FBXModelData* pModelData);
	bool HasTexture()const {}

	//èâä˙êF
	static void SetDefaultMaterial(FBXModelData* pModelData);
private:

	ColorChannel Emissive;
	ColorChannel Ambient;
	ColorChannel Diffuse;
	ColorChannel Specular;
	FbxFloat Shinness;

	static ColorChannel sEmissive;
	static ColorChannel sAmbient;
	static ColorChannel sDiffuse;
	static ColorChannel sSpecular;


};