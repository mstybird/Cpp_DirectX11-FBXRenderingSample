#pragma once
#include"Dijkstra.h"
#include"../DXVector3.h"
struct MyNode :public Dijkstra::Node
{
	string mName = "Empty";
	DXVector3 Position;
	MyNode(int id, const string& name,const DXVector3&position) :Node(id) {
		mName = name;
		Position = { position };
	}

};

class EnemyAI :public Dijkstra::Manager {
public:
	virtual void CreateNodes();
	//近い順にノードを取得する
	//第一引数、中心となる座標
	std::vector<MyNode*> GetNearNodeList(const DXVector3&pCenter);


};
