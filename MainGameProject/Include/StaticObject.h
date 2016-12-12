#pragma once
#include"GameObjectBase.h"
struct StatusField;
class StaticObject :public GameObjectBase {
public:
	virtual ~StaticObject() {}
	void Initialize()override;
	void Update()override;
	void Render()override;

	void ClearTrans();

};