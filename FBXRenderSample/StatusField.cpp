#include "StatusField.h"
#include"DXWorld.h"
#include"DXMatrix.h"
#include"AI\EnemyAI.h"
#include"CharacterBase.h"
#include<cassert>
#include<AIMapImport.hpp>
#include<string>
StatusField::~StatusField()
{
	for (decltype(auto)lNode : mFieldNodes) {
		delete lNode;
	}
	for (decltype(auto)lNode : mSpawnCharaNodes) {
		delete lNode;
	}
	for (decltype(auto)lNode : mSpawnBallNodes) {
		delete lNode;
	}
}
void StatusField::CreateFieldNodes()
{
	Comfort::AIMapImporter im;
	im.Import("sample.anm");
	auto v = im.GetList();
	using NodeControl::AddNodeSafe;
	//�m�[�h�̒ǉ�
	DXVector3 lNodePosition{};
	for (decltype(auto)lData : *v) {
		lNodePosition.Set(-lData.mX, lData.mY, lData.mZ);
		AddNodeSafe(
			mFieldNodes,
			new MyNode{
				lData.mID, 
				std::string("obj") +std::to_string(lData.mID),lNodePosition
			}
		);
	}


	//AddNodeSafe(mFieldNodes,new MyNode{ 0,"obj0",{ 4.00,0.00,-6.75 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 1,"obj1",{ 6.75,0.00,-8.75 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 2,"obj2",{ 9.75,0.00,-5.75 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 3,"obj3",{ 11.75,0.00,-3.75 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 4,"obj4",{ 9.00,0.00,-0.50 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 5,"obj5",{ 6.25,0.00,2.50 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 6,"obj6",{ 2.75,0.00,0.25 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 7,"obj7",{ 0.50,0.00,-2.50 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 8,"obj8",{ -4.75,0.00,-2.50 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 9,"obj9",{ -8.25,0.00,-2.50 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 10,"obj10",{ -11.00,0.00,1.00 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 11,"obj11",{ -11.00,0.00,5.25 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 12,"obj12",{ -8.00,0.00,8.50 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 13,"obj13",{ -4.00,0.00,8.50 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 14,"obj14",{ -0.00,0.00,4.50 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 15,"obj15",{ 2.50,0.00,7.50 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 16,"obj16",{ 7.00,0.00,7.50 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 17,"obj17",{ 11.00,0.00,4.00 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 18,"obj18",{ 11.00,0.00,-8.50 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 19,"obj19",{ 11.00,0.00,7.50 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 20,"obj20",{ -16.25,0.00,5.25 } });
	//AddNodeSafe(mFieldNodes,new MyNode{ 21,"obj21",{ -16.25,0.00,-3.50 } });


	//�m�[�h�ԌŒ�R�X�g�͋����Ƃ���						
	auto LinkNodeSafeCustom = [this](const int32_t aFromID, const int32_t aToID, const bool aIsBothFlag) {
		using NodeControl::LinkNodeSafe;
		//�m�[�h�Ԃ̋����𓾂�					
		auto NodeLength = [this](const int32_t aFromID, const int32_t aToID) {
			auto& lFromPos = static_cast<MyNode*>(mFieldNodes[aFromID])->Position;
			auto& lToPos = static_cast<MyNode*>(mFieldNodes[aToID])->Position;
			return (lFromPos - lToPos).GetDistance();
		};
		LinkNodeSafe(mFieldNodes, { aFromID,aToID }, NodeLength(aFromID, aToID), aIsBothFlag);
	};

	//�����N�m�[�h�̒ǉ�
	for (decltype(auto)lNode : *v) {
		printf("%d\n", lNode.mID);
		for (decltype(auto)lLink : lNode.mLinkList) {
			LinkNodeSafeCustom(lNode.mID, lLink, true);
		}
	}


}
void StatusField::CreateSpawnCharaNodes()
{
	using NodeControl::AddNodeSafe;
	AddNodeSafe(mSpawnCharaNodes, new MyNode{ 8,"obj8",{ 10.21,0.00,-51.26 } });
	AddNodeSafe(mSpawnCharaNodes, new MyNode{ 79,"obj79",{- 3.67,0.00,- 44.17} });
	AddNodeSafe(mSpawnCharaNodes, new MyNode{ 9,"obj9",{ 5.21,0.00,-51.26 } });
	AddNodeSafe(mSpawnCharaNodes, new MyNode{ 80,"obj80",{ -3.67,0.00,-48.17 } });
	//AddNodeSafe(mSpawnCharaNodes, new MyNode{ 19,"obj19",{ 11.00,0.00,7.50 } });
	//AddNodeSafe(mSpawnCharaNodes, new MyNode{ 20,"obj20",{ -16.25,0.00,5.25 } });
}
void StatusField::CreateSpawnBallNodes()
{
	using NodeControl::AddNodeSafe;
	AddNodeSafe(mSpawnBallNodes, new MyNode{ 0,"obj0",{ -16.40,0.00,- 47.00 } });
	//AddNodeSafe(mSpawnBallNodes, new MyNode{ 5,"obj5",{ 6.25,0.00,2.50 } });
	//AddNodeSafe(mSpawnBallNodes, new MyNode{ 13,"obj13",{ -4.00,0.00,8.50 } });
}
void StatusField::InitGoalIndex()
{
	mTeamWhite.mGoalIndex = 19;
	mTeamBlack.mGoalIndex = 80;
}
std::vector<Dijkstra::Node*> StatusField::GetFieldNodesClone()
{
	return mFieldNodes;
}
void StatusField::Respawn(CharacterBase * aSpawnChara)
{
	printf("%s is Respawn\n", typeid(aSpawnChara).name());

	auto lCount = rand() % mSpawnCharaNodes.size();

	static int sCount = 0;

	//�����_���ȃm�[�h������W�����o��
	auto& lPosition = static_cast<MyNode*>(mSpawnCharaNodes[sCount])->Position;

	++sCount;
	sCount %= mSpawnCharaNodes.size();

	aSpawnChara->GetWorld()->SetT(lPosition);
	aSpawnChara->SetActive(true);
	aSpawnChara->InitStatus();
	
}
void StatusField::RegisterTeamMember(CharacterBase * aRegistMember, eTeamType aType)
{
	switch (aType)
	{
	case eTeamType::White:
		mTeamWhite.mMembers.push_back(aRegistMember);
		break;
	case eTeamType::Black:
		mTeamBlack.mMembers.push_back(aRegistMember);
		break;
	default:
		break;
	}
}
StatusTeam * StatusField::GetTeamAlly(CharacterBase * aMember)
{
	if (aMember == nullptr)return nullptr;


	for (decltype(auto)lMember : mTeamBlack.mMembers) {
		if (lMember == aMember) {
			return &mTeamBlack;
		}
	}
	for (decltype(auto)lMember : mTeamWhite.mMembers) {
		if (lMember == aMember) {
			return &mTeamWhite;
		}
	}
	return nullptr;
}
StatusTeam * StatusField::GetTeamEnemy(CharacterBase * aMember)
{
	if (aMember == nullptr)return nullptr;

	for (decltype(auto)lMember : mTeamBlack.mMembers) {
		if (lMember == aMember) {
			return &mTeamWhite;
		}
	}
	for (decltype(auto)lMember : mTeamWhite.mMembers) {
		if (lMember == aMember) {
			return &mTeamBlack;
		}
	}
	return nullptr;
}
StatusField::StatusField():
	mBallHoldChara{nullptr},
	mBallIsField{false},
	mBall{nullptr}
{
}

void StatusField::SetBallHolder(CharacterBase * pBallHolder)
{
	mBallHoldChara = pBallHolder;
	mBallIsField = false;
	mBall->SetActive(false);
}

void StatusField::RespawnBall(DXVector3*pPosition)
{
	mBall->SetActive(true);
	mBallIsField = true;
	mBallHoldChara = nullptr;

	if (pPosition != nullptr) {
		mBall->GetWorld()->SetT(*pPosition);
	}
	else {
		//�ʒu���s��̏ꍇ�A
		//�{�[�����X�|�[���\�ȏꏊ�����_���ɃX�|�[��������
		auto lCount = rand()%mSpawnBallNodes.size();
		auto& lPosition =static_cast<MyNode*>(mSpawnBallNodes[lCount])->Position;
		mBall->GetWorld()->SetT(lPosition);
	}
}

void NodeControl::AddNodeSafe(std::vector<Dijkstra::Node*>& aNodeList, Dijkstra::Node * aAddNode)
{
	//NULL�`�F�b�N
	assert(aAddNode != nullptr);
	
	//ID�d���`�F�b�N
	//�{�������Ă͂Ȃ�Ȃ��̂ŁA�����I��������
	for (const auto&lNode : aNodeList) {
		assert(lNode->GetID() != aAddNode->GetID());
	}
	aNodeList.push_back(aAddNode);
}

void NodeControl::LinkNodeSafe(std::vector<Dijkstra::Node*>& aNodeList, std::pair<int, int> aFromTo, int aCost, bool aBothFlag)
{
	assert(IsBetweenIndex<Dijkstra::Node*>(aNodeList,aFromTo.first));
	assert(IsBetweenIndex<Dijkstra::Node*>(aNodeList,aFromTo.second));
	assert(aCost > 0);
	aNodeList[aFromTo.first]->addNode(aNodeList[aFromTo.second], aCost);

	//�o���������N�̏ꍇ�ATo����From�ɑ΂��Ă������N����
	if (aBothFlag == true) {
		aNodeList[aFromTo.second]->addNode(aNodeList[aFromTo.first], aCost);
	}
}
