#include "Dijkstra.h"
#include <algorithm>
#include<cassert>
const int Dijkstra::Node::cNotInitCost{ -1 };
Dijkstra::Manager::~Manager()
{
	for (auto&lNode : mNodes) {
		delete lNode;
	}
}

void Dijkstra::Manager::CreateNodeSafe(Node * aAddNode)
{
	//NULLチェック
	assert(aAddNode != nullptr);

	//ID重複チェック
	//本来あってはならないので、強制終了させる
	for (const auto&lNode : mNodes) {
		assert(lNode->mId != aAddNode->mId);
	}

	mNodes.push_back(aAddNode);

}

void Dijkstra::Manager::LinkNode(std::pair<int, int>aFromTo, int aCost, bool aBothFlag)
{
	assert(IsBetweenIndex(aFromTo.first));
	assert(IsBetweenIndex(aFromTo.second));
	assert(aCost > 0);

	mNodes[aFromTo.first]->addNode(mNodes[aFromTo.second], aCost);

	//双方向リンクの場合、ToからFromに対してもリンクする
	if (aBothFlag == true) {
		mNodes[aFromTo.second]->addNode(mNodes[aFromTo.first], aCost);
	}

}

void Dijkstra::Manager::RemoveNode(const int aRemoveID)
{
	//それぞれの子ノードから指定IDを持つノードを削除する
	for (auto&lNode : mNodes) {

		for (auto& it = lNode->mEdgeNode.begin(); it != lNode->mEdgeNode.end(); ++it) {
			if ((*it).first->mId == aRemoveID) {
				it = lNode->mEdgeNode.erase(it);
				--it;
			}
		}
	}
	//親ノードから削除



	auto RemoveIt = std::remove_if(mNodes.begin(), mNodes.end(), [&](Dijkstra::Node* lNode) {
		if (lNode->mId == aRemoveID) {
			//解放処理
			delete lNode;
			//ベクタから削除するために削除フラグを立てる
			return true;
		}
		else {
			return false;
		}
	});


	mNodes.erase(RemoveIt, mNodes.end());
}

void Dijkstra::Manager::SetStartNode(const int aStartNode)
{
	assert(IsBetweenIndex(aStartNode));
	mStartIndex = aStartNode;
	//現在ノードを更新
	mNowNode = mNodes[mStartIndex];
	//直前ノードをクリア
	mEndFrontNode = nullptr;
}

void Dijkstra::Manager::SetNowNode(Node * const aNowNode)
{
	assert(aNowNode);
	mNowNode = aNowNode;
}


int Dijkstra::Manager::GetNodeCount() const
{
	return mNodes.size();
}

int Dijkstra::Manager::GetRandomNodeIndex(bool aIncludeNowNodeFlag)
{
	//ノードのインデクスをランダムの取得する

	int lNodeIndex;
	while (true) {
		lNodeIndex = mRand() % mNodes.size();

		//現在ノードを含む場合、どのノードでもいいので選択
		if (aIncludeNowNodeFlag == true)break;
		//現在のノードを含まない場合は、現在のノードと一致しているか比較
		//不一致、もしくは現在ノードがnullであれば確定
		if (mNowNode == nullptr||
			mNowNode->mId != mNodes[lNodeIndex]->mId) {
			break;
		}
	}


	return lNodeIndex;
}

bool Dijkstra::Manager::GenerateRoot()
{
	ResetNodes();
	mIsGenerated = false;
	if (mStartIndex < 0) {
		return false;
	}
	mNodes[mStartIndex]->mCost = 0;


	while (true) {
		Node*lProcessNode{ nullptr };

		for (auto&lNode : mNodes) {
			if (lNode->mSearched || lNode->mCost < 0) {
				continue;
			}
			if (lProcessNode == nullptr) {
				lProcessNode = lNode;
				continue;
			}
			if (lNode->mCost < lProcessNode->mCost) {
				lProcessNode = lNode;
			}
		}

		//初期化ノードがない場合、全走査が終了している
		//もしくは初期化ノードがない
		if (lProcessNode == nullptr) {
			break;
		}


		lProcessNode->mSearched = true;

		for (const auto& lEdgeNode : lProcessNode->mEdgeNode) {
			const auto& lNode = lEdgeNode.first;
			const auto& lCost = lProcessNode->mCost + lEdgeNode.second;

			bool lNeedUpdate = (lNode->mCost<0) || (lNode->mCost>lCost);
			if (lNeedUpdate) {
				lNode->mCost = lCost;
				lNode->mPreviousNode = lProcessNode;
			}

		}
	}

	mIsGenerated = true;

	return true;
}


bool Dijkstra::Manager::SetNextNode()
{
	//辿れるノードがあった場合
	if (mRootNodes.size() > 1) {
		//直前ノードを記憶
		mEndFrontNode = mRootNodes.back();
		mRootNodes.pop_back();
		//現在ノードを更新
		mNowNode = mRootNodes.back();
		return true;
	}
	else {
		//なかった場合、最後のノードが一つ残るので、削除する
		//本当に空だった場合は何もしない
		if (mRootNodes.size() != 0) {
			mRootNodes.pop_back();
		}
		return false;
	}
}

bool Dijkstra::Manager::CreateNextRoot(bool aBackFlag)
{
	mRootNodes.clear();
	int EdgeCount = GetEdgeNodeCount(mNowNode);
	//直前ノードに戻る可能性があるかどうか

	//移動先候補数が0ならfalse
	if (EdgeCount == 0)return false;

	//エッジノードインデクスを計算
	int lNodeIndex;
	while (true) {
		lNodeIndex = mRand() % EdgeCount;

		//直前ノードに戻ってはいけない場合
		if (aBackFlag == false) {
			//直前ノードを持ってる場合
			if (mEndFrontNode != nullptr) {
				//直前ノードと一致すれば再取得
				if (mNowNode->mEdgeNode[lNodeIndex].first->mId == mEndFrontNode->mId) {
					continue;
				}
			}
		}
		break;
	}

	mRootNodes.push_back(mNowNode->mEdgeNode[lNodeIndex].first);
	//直前ノードを更新
	mEndFrontNode = mNowNode;
	//現在ノードを更新
	mNowNode = mRootNodes.back();

	return true;
}

int Dijkstra::Manager::GetEdgeNodeCount(const Node * aRootNode)
{
	assert(aRootNode);
	return aRootNode->mEdgeNode.size();
}

bool Dijkstra::Manager::IsEdgeIntoNode(const Node * IntoNodes, const Node * aSearchNode)
{
	//サーチするノードがnullだった場合、IntoNodesには含まれないとする
	if (aSearchNode == nullptr) {
		return false;
	}

	for (auto& lNode : IntoNodes->mEdgeNode) {
		if (lNode.first->mId = aSearchNode->mId) {
			//一致するノードがあればこの時点でtrueを返す
			return true;
		}
	}
	return false;
}

bool Dijkstra::Manager::CreateRoot(int aGoalIndex)
{
	//未探索の場合は処理しない
	if (mIsGenerated == false) {
		return false;
	}
	assert(IsBetweenIndex(aGoalIndex));
	auto lCurrentNode = mNodes[aGoalIndex];
	//現在のノードを更新
	mNowNode = mNodes[mStartIndex];
	mRootNodes.clear();
	mRootNodes.push_back(lCurrentNode);
	while (true) {
		auto lNextNode = lCurrentNode->mPreviousNode;
		if (!lNextNode) {
			break;
		}
		mRootNodes.push_back(lNextNode);
		lCurrentNode = lNextNode;
	}
	//reverse(aRootNodes.begin(), aRootNodes.end());

	return true;
}


void Dijkstra::Manager::ResetNodes()
{
	for (auto& lNode : mNodes) {
		lNode->Reset();
	}
	mNowNode = nullptr;
	mEndFrontNode = nullptr;
}

bool Dijkstra::Manager::IsBetweenIndex(const int aNodeIndex)
{
	const auto& lNodeSize = mNodes.size();
	return (aNodeIndex >= 0) & (aNodeIndex < lNodeSize);
}

bool Dijkstra::Manager::IsRootEmpty()
{
	return mRootNodes.size() == 0;
}


