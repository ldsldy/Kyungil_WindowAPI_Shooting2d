#include "Key.h"
#include "GameManager.h"
#include "PhysicsComponent.h"

void Key::OnInitialize()
{
	SetSize(GameManager::ActorDefaultSize / 2); // 크기 설정
	float HalfSize = Size * 0.5f;
	
	PhysicsComponent* physicsComponent = new PhysicsComponent(this, CollisionType::Rectangle, PhysicsLayer::Key);
	physicsComponent->SetWidth(static_cast<float>(HalfSize));
	physicsComponent->SetHeight(HalfSize*0.5f);
	AddComponent(physicsComponent); //물리 컴포넌트 추가
}

void Key::OnTick(float deltaTime)
{
	Actor::OnTick(deltaTime);
}

void Key::OnOverlap(Actor* other)
{
	if (other && other != this)
	{
		//열쇠가 다른 액터와 겹칠 때 처리]
		OutputDebugString(L"Key::OnOverlap called\n");
	}
}
