#pragma once
#include "Component.h"
#include "Common.h"

class MoveComponent:public Component
{
public:
	MoveComponent(Actor* InOwner) :Component(InOwner) {};
	virtual ~MoveComponent() = default;


	//���� ��ġ ���
	PointF CalculateNextPosition(float InDeltaTime);

	//������ impulse ����
	
	
	//�߷� 
	

	

	

private:
	

};

