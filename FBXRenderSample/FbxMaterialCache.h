#pragma once
#include<D3DX11.h>
#include<fbxsdk.h>
class DX11TextureManager;
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
	void SetCurrentMaterial(const std::weak_ptr<FBXModelData>&pModelData);
	bool HasTexture()const {}

	//èâä˙êF
	static void SetDefaultMaterial(const std::weak_ptr<FBXModelData>&pModelData);
private:

	std::shared_ptr<ColorChannel> Emissive;
	std::shared_ptr<ColorChannel> Ambient;
	std::shared_ptr<ColorChannel> Diffuse;
	std::shared_ptr<ColorChannel> Specular;
	FbxFloat Shinness;

	static std::shared_ptr<ColorChannel> sEmissive;
	static std::shared_ptr<ColorChannel> sAmbient;
	static std::shared_ptr<ColorChannel> sDiffuse;
	static std::shared_ptr<ColorChannel> sSpecular;


};