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

	void OnPlayerGetKey(); //�÷��̾ ���踦 ȹ������ �� ȣ��Ǵ� �Լ�
private:
	int KeyId = 0; //���� ID
};

