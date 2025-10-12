#include "Bomb.h"
#include "GameManager.h"
#include "PhysicsComponent.h"

void Bomb::OnInitialize()
{
	dropSpeed = 0.0f;

	SetSize(GameManager::ActorDefaultSize/2); // 크기 설정
	float HalfSize = Size * 0.5f;
	Position.X = static_cast<float>(HalfSize + rand() % (GameManager::ScreenWidth - Size)); //랜덤 X 위치 설정
	Position.Y = static_cast<float>(-Size); //화면 위쪽에서 시작

	PhysicsComponent* physicsComponent = new PhysicsComponent(this, CollisionType::Rectangle, PhysicsLayer::Bomb);
	physicsComponent->SetWidth(static_cast<float>(HalfSize*1.5f)); //일정 범위안에 다가가면 폭발
	physicsComponent->SetHeight(HalfSize * 0.75f);
	AddComponent(physicsComponent); //물리 컴포넌트 추가
}

void Bomb::OnTick(float deltaTime)
{
	Actor::OnTick(deltaTime); //부모 클래스의 OnTick 호출
	

	//폭탄이 터질려 한다면
	if (IsCatchedFire == true)
	{
		lifeTime -= deltaTime;
	}
}

void Bomb::OnOverlap(Actor* other)
{
	if (other && other != this)
	{
		//폭탄과 다른 액터가 겹칠 때 처리
		OutputDebugString(L"Bomb::OnOverlap called\n");

		IsCatchedFire = true;

		//폭탄 삭제
		DestroyActor();
	}
}




//dropSpeed += dropAcceleration * deltaTime; //낙하 속도 갱신

//Position.Y += dropSpeed * deltaTime;
//lifeTime -= deltaTime;

////화면 밖으로 나가거나 수명이 다하면 삭제
//if(Position.Y > GameManager::ScreenHeight+Size || lifeTime <= 0.0f)
//{
//	DestroyActor();
//}
