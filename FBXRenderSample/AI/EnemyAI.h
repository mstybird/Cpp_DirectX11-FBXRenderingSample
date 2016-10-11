#pragma once
#include"Dijkstra.h"
#include"../DXVector3.h"
class NcgLuaManager;
struct MyNode :public Dijkstra::Node
{
	string mName = "Empty";
	DXVector3 Position;
	MyNode(int id, const string& name,const DXVector3&position) :Node(id) {
		mName = name;
		Position = { position };
	}

};

namespace EnemyAIType {
	enum Type {
		eUnKnown,
		eEnergyShot,
		eMovingHide,
		eChargeEnergy,
		eSearchTarget,
		eMoveToTarget
	};
	const std::string sUnKnown = "unKnown";
	const std::string sEnergyShot = "energyShot";
	const std::string sMovingHide = "movingHide";
	const std::string sChargeEnergy = "chargeEnergy";
	const std::string sSearchTarget = "searchTarget";
	const std::string sMoveToTarget = "moveToTarget";


	Type ConvertString(const std::string&aStr);
}




class EnemyAI :public Dijkstra::Manager {
public:
	virtual void CreateNodes();
	//�߂����Ƀm�[�h���擾����
	//�������A���S�ƂȂ���W
	std::vector<MyNode*> GetNearNodeList(const DXVector3&pCenter);
	//AI�̍X�V
	void Update(bool aTargeting,bool aTarget, bool aChargedEnergy);
	//���݂̍s���p�^�[�����擾����
	EnemyAIType::Type GetNowAI();
	//AI��������߂�
	//�����߂邱�Ƃ��ł��Ȃ������ꍇ��false���Ԃ����
	bool NextAI();
	//��������AI���~������
	void ClearAI();

	//AI�p
	NcgLuaManager* mLuaAI;
	std::vector<EnemyAIType::Type>mAIType;
};
