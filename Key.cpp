#include "Key.h"
#include "GameManager.h"
#include "PhysicsComponent.h"

void Key::OnInitialize()
{
	SetSize(GameManager::ActorDefaultSize / 2); // ũ�� ����
	float HalfSize = Size * 0.5f;
	
	PhysicsComponent* physicsComponent = new PhysicsComponent(this, CollisionType::Rectangle, PhysicsLayer::Key);
	physicsComponent->SetWidth(static_cast<float>(HalfSize));
	physicsComponent->SetHeight(HalfSize*0.5f);
	AddComponent(physicsComponent); //���� ������Ʈ �߰�
}

void Key::OnTick(float deltaTime)
{
	Actor::OnTick(deltaTime);
}

void Key::OnOverlap(Actor* other)
{
	if (other && other != this)
	{
		//���谡 �ٸ� ���Ϳ� ��ĥ �� ó��]
		OutputDebugString(L"Key::OnOverlap called\n");
	}
}

void Key::OnPlayerGetKey()
{
		//���� ȹ�� ó��
		OutputDebugString(L"Player has obtained the key!\n");
		DestroyActor(); //���� ����
}