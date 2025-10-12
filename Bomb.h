#pragma once
#include "Actor.h"

class Bomb : public Actor
{
public:
	Bomb() = delete;
	Bomb(ResourceID resourceID): Actor(resourceID) {};

	virtual void OnInitialize() override;
	virtual void OnTick(float deltaTime) override;

	virtual void OnOverlap(Actor* other) override;

	//Getters
	//Setters
	void SetDropAcceleration(float newAcceleration) { dropAcceleration = newAcceleration; }

private:
	float dropAcceleration = 200.0f; //��ź ���� ���ӵ�(�ʴ� �ȼ� ����)
	float dropSpeed = 0.0f; //���� ���� �ӵ�
	float lifeTime = 5.0f; //��ź�� ����(�� ����)

	bool IsCatchedFire = false;
};

