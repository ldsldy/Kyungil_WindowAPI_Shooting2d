#include "Key.h"
#include "GameManager.h"
#include "PhysicsComponent.h"

void Key::OnInitialize()
{
	SetSize(GameManager::ActorDefaultSize / 2); // Å©±â ¼³Á¤
	float HalfSize = Size * 0.5f;
	
	PhysicsComponent* physicsComponent = new PhysicsComponent(this, CollisionType::Rectangle, PhysicsLayer::Key);
	physicsComponent->SetWidth(static_cast<float>(HalfSize));
	physicsComponent->SetHeight(HalfSize*0.5f);
	AddComponent(physicsComponent); //¹°¸® ÄÄÆ÷³ÍÆ® Ãß°¡
}

void Key::OnTick(float deltaTime)
{
	Actor::OnTick(deltaTime);
}

void Key::OnOverlap(Actor* other)
{
	if (other && other != this)
	{
		//¿­¼è°¡ ´Ù¸¥ ¾×ÅÍ¿Í °ãÄ¥ ¶§ Ã³¸®]
		OutputDebugString(L"Key::OnOverlap called\n");
	}
}

void Key::OnPlayerGetKey()
{
		//¿­¼è È¹µæ Ã³¸®
		OutputDebugString(L"Player has obtained the key!\n");
		DestroyActor(); //¿­¼è »èÁ¦
}