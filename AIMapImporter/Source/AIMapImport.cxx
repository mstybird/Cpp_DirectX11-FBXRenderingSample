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
			lStream >> lPositionStr;
			//リンクノード取得
			lNodeData.mLinkList = Split<int32_t>(lLinkListStr, ',');
			//座標取得
			lPositionList = Split<float_t>(lPositionStr, ',');
			lNodeData.mX = lPositionList[0];
			lNodeData.mY = lPositionList[1];
			lNodeData.mZ = lPositionList[2];
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