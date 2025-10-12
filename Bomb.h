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
	float dropAcceleration = 200.0f; //ÆøÅº ³«ÇÏ °¡¼Óµµ(ÃÊ´ç ÇÈ¼¿ ´ÜÀ§)
	float dropSpeed = 0.0f; //ÇöÀç ³«ÇÏ ¼Óµµ
	float lifeTime = 5.0f; //ÆøÅºÀÇ ¼ö¸í(ÃÊ ´ÜÀ§)

	bool IsCatchedFire = false;
};

