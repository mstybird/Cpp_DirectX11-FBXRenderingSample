#pragma once
#include<D3DX11.h>


class FbxMaterialCache {
public:
	FbxMaterialCache();
	~FbxMaterialCache();
	bool Initialize(const FbxSurfaceMaterial*pMaterial, const std::string& pTextureRelativePathName = { "" });
	static FbxDouble3 GetMaterialProperty(
		const FbxSurfaceMaterial*pMaterial,
		std::string pPropertyName,
		std::string pFactorPropertyName,
		std::string&pTextureName,
		const std::string& pTextureRelativePathName = {""}
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