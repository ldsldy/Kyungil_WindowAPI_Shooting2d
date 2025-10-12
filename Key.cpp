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

void Key::OnRender(Gdiplus::Graphics* InGraphics)
{
	if (!InGraphics) return;
	if (!Image)
	{
		OutputDebugString(L"Key::OnRender - Image is null\n");
		return;
	}

	//Image가 로딩되어 있다.
	InGraphics->DrawImage(
		Image,
		static_cast<int>(Position.X - Size * Pivot.X), //그려질 위치
		static_cast<int>(Position.Y - Size * Pivot.Y),
		Size, Size); //그려질 사이즈
}

void Key::OnOverlap(Actor* other)
{
	if (other && other != this)
	{
		//열쇠가 다른 액터와 겹칠 때 처리]
		OutputDebugString(L"Key::OnOverlap called\n");
	}
}

void Key::OnPlayerGetKey()
{
	//열쇠 획득 처리
	OutputDebugString(L"Player has obtained the key!\n");
	DestroyActor(); //열쇠 삭제
}