#pragma once
#include "Actor.h"

class Key : public Actor
{
public:
	Key() = delete;
	Key(ResourceID resourceID) : Actor(resourceID) {};

	virtual void OnInitialize() override;
	virtual void OnTick(float deltaTime) override;

	virtual void OnOverlap(Actor* other) override;
};

