#pragma once
#include"GameObjectBase.h"
class StaticObject :public GameObjectBase {
public:
	virtual ~StaticObject() {}
	void Initialize()override;
	void Update()override;
	void Render()override;
};