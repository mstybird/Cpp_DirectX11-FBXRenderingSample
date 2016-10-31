#include "AIMapImport.hxx"
#include<fstream>
#include<string>
#include<sstream>
namespace Comfort {

	//文字列を分割する
	template<typename T>
	std::vector<T> Split(const std::string& aString, const char aElem) {
		std::vector<T> lResult{};
		std::stringstream lSs(aString);
		T lTempData{};
		while (lSs) {
			lSs >> lTempData;
			lResult.push_back(lTempData);
			lSs.ignore();
		}
		return std::move(lResult);
	}



	bool AIMapImporter::Import(const char * aFilePath)
	{
		std::ifstream lStream(aFilePath);
		//読み込めなかった場合はfalseを返す
		if (lStream.fail())return false;

		//文字列を数値に分割する
		

		//読み込み終わるまで続ける
		AINodeData lNodeData;
		std::string lLinkListStr;
		std::string lPositionStr;
		std::vector<int32_t> lLinkList;
		std::vector<float_t> lPositionList;

		while (lStream) {
			//ノードID取得
			lStream >> lNodeData.mID;
			lStream >> lLinkListStr;
			//リンクノード取得
			lNodeData.mLinkList = Split<int32_t>(lLinkListStr, ',');
			//リンクノードがない場合
			//サイズは0に固定する
			if (lNodeData.mLinkList.size() == 0 || lNodeData.mLinkList[0] == -1) {
				lNodeData.mLinkList.clear();
			}

			//座標取得
			lStream >> lNodeData.mX >> lNodeData.mY >> lNodeData.mZ;
			mNodeList.push_back(std::move(lNodeData));
		}
		mNodeList.pop_back();

		return true;
	}

	const std::vector<AINodeData>* AIMapImporter::GetList() const
	{
		return &mNodeList;
	}
}