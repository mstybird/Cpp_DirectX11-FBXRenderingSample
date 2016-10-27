#include "StatusField.h"
#include"DXWorld.h"
#include"DXMatrix.h"
#include"AI\EnemyAI.h"
#include"CharacterBase.h"
#include<cassert>
#include<AIMapImport.hpp>
void StatusField::CreateFieldNodes()
{
	Comfort::AIMapImporter im;
	im.Import("sample.anm");
	auto v = im.GetList();
	using NodeControl::AddNodeSafe;
	AddNodeSafe(mFieldNodes,new MyNode{ 0,"obj0",{ 4.00,0.00,-6.75 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 1,"obj1",{ 6.75,0.00,-8.75 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 2,"obj2",{ 9.75,0.00,-5.75 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 3,"obj3",{ 11.75,0.00,-3.75 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 4,"obj4",{ 9.00,0.00,-0.50 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 5,"obj5",{ 6.25,0.00,2.50 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 6,"obj6",{ 2.75,0.00,0.25 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 7,"obj7",{ 0.50,0.00,-2.50 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 8,"obj8",{ -4.75,0.00,-2.50 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 9,"obj9",{ -8.25,0.00,-2.50 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 10,"obj10",{ -11.00,0.00,1.00 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 11,"obj11",{ -11.00,0.00,5.25 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 12,"obj12",{ -8.00,0.00,8.50 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 13,"obj13",{ -4.00,0.00,8.50 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 14,"obj14",{ -0.00,0.00,4.50 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 15,"obj15",{ 2.50,0.00,7.50 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 16,"obj16",{ 7.00,0.00,7.50 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 17,"obj17",{ 11.00,0.00,4.00 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 18,"obj18",{ 11.00,0.00,-8.50 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 19,"obj19",{ 11.00,0.00,7.50 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 20,"obj20",{ -16.25,0.00,5.25 } });
	AddNodeSafe(mFieldNodes,new MyNode{ 21,"obj21",{ -16.25,0.00,-3.50 } });


	using std::pair;
	using NodeControl::LinkNodeSafe;
	LinkNodeSafe(mFieldNodes,{ 0, 1 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 0, 7 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 0, 8 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 1, 2 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 2, 3 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 2, 18 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 3, 4 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 3, 17 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 3, 18 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 4, 5 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 4, 19 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 5, 6 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 5, 16 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 5, 19 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 6, 7 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 6, 14 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 7, 8 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 8, 9 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 8, 14 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 9, 10 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 9, 11 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 10, 21 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 11, 12 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 11, 20 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 11, 21 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 12, 13 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 12, 20 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 13, 14 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 13, 15 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 14, 15 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 15, 16 }, 1, true);
	LinkNodeSafe(mFieldNodes,{ 16, 19 }, 1, true);




}
void StatusField::CreateSpawnCharaNodes()
{
	using NodeControl::AddNodeSafe;
	AddNodeSafe(mSpawnCharaNodes, new MyNode{ 8,"obj8",{ -4.75,0.00,-2.50 } });
	AddNodeSafe(mSpawnCharaNodes, new MyNode{ 19,"obj19",{ 11.00,0.00,7.50 } });
	AddNodeSafe(mSpawnCharaNodes, new MyNode{ 20,"obj20",{ -16.25,0.00,5.25 } });
}
void StatusField::CreateSpawnBallNodes()
{
	using NodeControl::AddNodeSafe;
	AddNodeSafe(mSpawnBallNodes, new MyNode{ 0,"obj0",{ 4.00,0.00,-6.75 } });
	AddNodeSafe(mSpawnBallNodes, new MyNode{ 5,"obj5",{ 6.25,0.00,2.50 } });
	AddNodeSafe(mSpawnBallNodes, new MyNode{ 13,"obj13",{ -4.00,0.00,8.50 } });
}
std::vector<Dijkstra::Node*> StatusField::GetFieldNodesClone()
{
	return mFieldNodes;
}
void StatusField::Respawn(CharacterBase * aSpawnChara)
{
	printf("%s is Respawn\n", typeid(aSpawnChara).name());

	auto lCount = rand() % mSpawnCharaNodes.size();
	//ランダムなノードから座標を取り出す
	auto& lPosition = static_cast<MyNode*>(mSpawnCharaNodes[lCount])->Position;
	aSpawnChara->GetWorld()->SetT(lPosition);

	aSpawnChara->SetActive(true);
	aSpawnChara->InitStatus();
	
}
StatusField::StatusField():
	mBallHoldChara{nullptr},
	mBallIsField{false},
	mBall{nullptr}
{
}

void StatusField::SetBallHolder(GameObjectBase * pBallHolder)
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
		//位置が不定の場合、
		//ボールがスポーン可能な場所ランダムにスポーンさせる
		auto lCount = rand()%mSpawnBallNodes.size();
		auto& lPosition =static_cast<MyNode*>(mSpawnBallNodes[lCount])->Position;
		mBall->GetWorld()->SetT(lPosition);
	}
}

void NodeControl::AddNodeSafe(std::vector<Dijkstra::Node*>& aNodeList, Dijkstra::Node * aAddNode)
{
	//NULLチェック
	assert(aAddNode != nullptr);
	
	//ID重複チェック
	//本来あってはならないので、強制終了させる
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

	//双方向リンクの場合、ToからFromに対してもリンクする
	if (aBothFlag == true) {
		aNodeList[aFromTo.second]->addNode(aNodeList[aFromTo.first], aCost);
	}
}
