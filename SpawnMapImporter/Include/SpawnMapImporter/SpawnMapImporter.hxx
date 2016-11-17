#pragma once
#include<vector>

namespace Comfort {
	struct SpawnNodeData {
		int32_t mID;
		union
		{
			struct {
				float_t mX;
				float_t mY;
				float_t mZ;
			};
			float_t mPosition[3];
		};
	};

	class SpawnMapImporter {
	public:
		//ファイルを読み込む
		bool Import(const char*aFilePath);
		//ノードリストの参照を取得
		const std::vector<SpawnNodeData>* GetList()const;
	private:
		std::vector<SpawnNodeData> mNodeList;
	};

}