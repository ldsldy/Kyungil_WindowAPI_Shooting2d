#pragma once
#include "Actor.h"

class Key : public Actor
{
public:
	Key() = delete;
	Key(ResourceID resourceID) : Actor(resourceID) {};
	virtual ~Key() = default;

	virtual void OnInitialize() override;
	virtual void OnTick(float deltaTime) override;

	virtual void OnOverlap(Actor* other) override;

	//Getters
	int GetKeyId() const { return KeyId; }
	
	//Setters
	void SetKeyId(int InKeyID) { KeyId = InKeyID; }

	void OnPlayerGetKey(); //플레이어가 열쇠를 획득했을 때 호출되는 함수
private:
	int KeyId = 0; //열쇠 ID
};

