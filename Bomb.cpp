#include "Bomb.h"
#include "GameManager.h"
#include "PhysicsComponent.h"

void Bomb::OnInitialize()
{
	dropSpeed = 0.0f;

	SetSize(GameManager::ActorDefaultSize/2); // ũ�� ����
	float HalfSize = Size * 0.5f;
	Position.X = static_cast<float>(HalfSize + rand() % (GameManager::ScreenWidth - Size)); //���� X ��ġ ����
	Position.Y = static_cast<float>(-Size); //ȭ�� ���ʿ��� ����

	PhysicsComponent* physicsComponent = new PhysicsComponent(this, CollisionType::Rectangle, PhysicsLayer::Bomb);
	physicsComponent->SetWidth(static_cast<float>(HalfSize*1.5f)); //���� �����ȿ� �ٰ����� ����
	physicsComponent->SetHeight(HalfSize * 0.75f);
	AddComponent(physicsComponent); //���� ������Ʈ �߰�
}

void Bomb::OnTick(float deltaTime)
{
	Actor::OnTick(deltaTime); //�θ� Ŭ������ OnTick ȣ��
	

	//��ź�� ������ �Ѵٸ�
	if (IsCatchedFire == true)
	{
		lifeTime -= deltaTime;
	}
}

void Bomb::OnOverlap(Actor* other)
{
	if (other && other != this)
	{
		//��ź�� �ٸ� ���Ͱ� ��ĥ �� ó��
		OutputDebugString(L"Bomb::OnOverlap called\n");

		IsCatchedFire = true;

		//��ź ����
		DestroyActor();
	}
}




//dropSpeed += dropAcceleration * deltaTime; //���� �ӵ� ����

//Position.Y += dropSpeed * deltaTime;
//lifeTime -= deltaTime;

////ȭ�� ������ �����ų� ������ ���ϸ� ����
//if(Position.Y > GameManager::ScreenHeight+Size || lifeTime <= 0.0f)
//{
//	DestroyActor();
//}
