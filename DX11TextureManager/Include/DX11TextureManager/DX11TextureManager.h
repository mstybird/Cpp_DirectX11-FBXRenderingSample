#pragma once
#include<unordered_map>
#include<memory>
#include<random>
class DXTexture;
struct ID3D11Texture2D;
//テクスチャ管理クラス
class DX11TextureManager {
public:

	~DX11TextureManager();

	//テクスチャの一括登録用
	//テクスチャID、相対ファイルパス
	typedef std::unordered_map<int, std::string>FileList;

	//テクスチャ登録
	//既にファイルが存在した場合、登録しない
	//デバッグ時はassetチェック
	bool RegistFromFile(const std::string&pFileName, const int pID);

	//テクスチャ登録
	//メモリ内のテクスチャをそのまま登録する
	bool RegistFromMemory(ID3D11Texture2D*&pTexture, const int pID);

	//テクスチャの一括登録
	//既にファイルが存在した場合、登録しない
	//デバッグ時はassetチェック
	bool RegisterFileList(const FileList&pFileList);

	//テクスチャを読み込み(参照)
	//ファイルが存在しなかった場合、取得しない
	//デバッグ時はassetチェック
	bool Load(DXTexture*&pDstTexture,const int pID)const;

	//指定したIDにテクスチャが登録されているかどうか
	bool IsRegisterd(const int pID);
	//登録したファイルを削除する
	void UnRegisterFile(const int pID);
	//登録したファイルすべてを削除する
	void UnRegisterFileAll();

	//未使用のIDを探す
	int GetNotRegisteredID();
	int GetRegisteredCount();
private:
	//テクスチャID生成用乱数
	std::mt19937 mTextureIDRandom;
	//数値IDの指すテクスチャデータ
	std::unordered_map<int, std::shared_ptr<DXTexture>> mTextureMap;
};
