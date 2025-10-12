#pragma once
#include "Component.h"
#include "Common.h"

class MoveComponent:public Component
{
public:
	MoveComponent(Actor* InOwner) :Component(InOwner) {};
	virtual ~MoveComponent() = default;


	//다음 위치 계산
	PointF CalculateNextPosition(float InDeltaTime);

	//점프만 impulse 적용
	
	
	//중력 
	

	

	

private:
	

};

