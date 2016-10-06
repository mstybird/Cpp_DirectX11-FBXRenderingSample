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
	//NULL�`�F�b�N
	assert(aAddNode != nullptr);

	//ID�d���`�F�b�N
	//�{�������Ă͂Ȃ�Ȃ��̂ŁA�����I��������
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

	//�o���������N�̏ꍇ�ATo����From�ɑ΂��Ă������N����
	if (aBothFlag == true) {
		mNodes[aFromTo.second]->addNode(mNodes[aFromTo.first], aCost);
	}

}

void Dijkstra::Manager::RemoveNode(const int aRemoveID)
{
	//���ꂼ��̎q�m�[�h����w��ID�����m�[�h���폜����
	for (auto&lNode : mNodes) {

		for (auto& it = lNode->mEdgeNode.begin(); it != lNode->mEdgeNode.end(); ++it) {
			if ((*it).first->mId == aRemoveID) {
				it = lNode->mEdgeNode.erase(it);
				--it;
			}
		}
	}
	//�e�m�[�h����폜



	auto RemoveIt = std::remove_if(mNodes.begin(), mNodes.end(), [&](Dijkstra::Node* lNode) {
		if (lNode->mId == aRemoveID) {
			//�������
			delete lNode;
			//�x�N�^����폜���邽�߂ɍ폜�t���O�𗧂Ă�
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
	//���݃m�[�h���X�V
	mNowNode = mNodes[mStartIndex];
	//���O�m�[�h���N���A
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
	//�m�[�h�̃C���f�N�X�������_���̎擾����

	int lNodeIndex;
	while (true) {
		lNodeIndex = mRand() % mNodes.size();

		//���݃m�[�h���܂ޏꍇ�A�ǂ̃m�[�h�ł������̂őI��
		if (aIncludeNowNodeFlag == true)break;
		//���݂̃m�[�h���܂܂Ȃ��ꍇ�́A���݂̃m�[�h�ƈ�v���Ă��邩��r
		//�s��v�A�������͌��݃m�[�h��null�ł���Ίm��
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

		//�������m�[�h���Ȃ��ꍇ�A�S�������I�����Ă���
		//�������͏������m�[�h���Ȃ�
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
	//�H���m�[�h���������ꍇ
	if (mRootNodes.size() > 1) {
		//���O�m�[�h���L��
		mEndFrontNode = mRootNodes.back();
		mRootNodes.pop_back();
		//���݃m�[�h���X�V
		mNowNode = mRootNodes.back();
		return true;
	}
	else {
		//�Ȃ������ꍇ�A�Ō�̃m�[�h����c��̂ŁA�폜����
		//�{���ɋ󂾂����ꍇ�͉������Ȃ�
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
	//���O�m�[�h�ɖ߂�\�������邩�ǂ���

	//�ړ����␔��0�Ȃ�false
	if (EdgeCount == 0)return false;

	//�G�b�W�m�[�h�C���f�N�X���v�Z
	int lNodeIndex;
	while (true) {
		lNodeIndex = mRand() % EdgeCount;

		//���O�m�[�h�ɖ߂��Ă͂����Ȃ��ꍇ
		if (aBackFlag == false) {
			//���O�m�[�h�������Ă�ꍇ
			if (mEndFrontNode != nullptr) {
				//���O�m�[�h�ƈ�v����΍Ď擾
				if (mNowNode->mEdgeNode[lNodeIndex].first->mId == mEndFrontNode->mId) {
					continue;
				}
			}
		}
		break;
	}

	mRootNodes.push_back(mNowNode->mEdgeNode[lNodeIndex].first);
	//���O�m�[�h���X�V
	mEndFrontNode = mNowNode;
	//���݃m�[�h���X�V
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
	//�T�[�`����m�[�h��null�������ꍇ�AIntoNodes�ɂ͊܂܂�Ȃ��Ƃ���
	if (aSearchNode == nullptr) {
		return false;
	}

	for (auto& lNode : IntoNodes->mEdgeNode) {
		if (lNode.first->mId = aSearchNode->mId) {
			//��v����m�[�h������΂��̎��_��true��Ԃ�
			return true;
		}
	}
	return false;
}

bool Dijkstra::Manager::CreateRoot(int aGoalIndex)
{
	//���T���̏ꍇ�͏������Ȃ�
	if (mIsGenerated == false) {
		return false;
	}
	assert(IsBetweenIndex(aGoalIndex));
	auto lCurrentNode = mNodes[aGoalIndex];
	//���݂̃m�[�h���X�V
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


