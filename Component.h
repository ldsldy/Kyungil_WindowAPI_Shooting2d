#pragma once

class Actor;
class Component
{
public:
	Component(Actor* InOwner) : Owner(InOwner) {}
	virtual ~Component() = default;

	virtual void OnInitialize() {};
	virtual void OnDestroy() {};
	virtual void OnTick(float InDeltaTime) {};

	inline Actor* GetOwner() const { return Owner; }

protected:
	Actor* Owner = nullptr;	//이 컴포넌트를 소유한 액터
};