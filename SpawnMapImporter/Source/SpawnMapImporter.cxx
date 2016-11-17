#include"SpawnMapImporter.hxx"
#include<fstream>
#include<string>
#include<sstream>
namespace Comfort {

	//•¶Žš—ñ‚ð•ªŠ„‚·‚é
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

	bool SpawnMapImporter::Import(const char * aFilePath)
	{
		std::ifstream lStream(aFilePath);
		//“Ç‚Ýž‚ß‚È‚©‚Á‚½ê‡‚Ífalse‚ð•Ô‚·
		if (lStream.fail())return false;

		SpawnNodeData lNodeData;

		while (lStream) {
			lStream >> lNodeData.mID;
			lStream >> lNodeData.mX >> lNodeData.mY >> lNodeData.mZ;
			mNodeList.push_back(std::move(lNodeData));
		}
		mNodeList.pop_back();
		return true;
	}

	const std::vector<SpawnNodeData>* SpawnMapImporter::GetList() const
	{
		return &mNodeList;
	}
}

