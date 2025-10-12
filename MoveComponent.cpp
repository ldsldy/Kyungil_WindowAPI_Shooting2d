#include "MoveComponent.h"
#include "Actor.h"
void MoveComponent::OnTick(float InDeltaTime)
{
	if (!Owner) return;
	ApplyGravity(InDeltaTime);
}

PointF MoveComponent::CalculateNextPosition(float InDeltaTime)
{
	if (!Owner) return { 0.0f, 0.0f };

	PointF CurrentPos = Owner->GetPosition();
	PointF NextPos;

	// 나중에 Force 개념 추가할 때 여기서 처리
	NextPos.X = CurrentPos.X + Velocity.X * InDeltaTime;
	NextPos.Y = CurrentPos.Y + Velocity.Y * InDeltaTime;

	return NextPos;
}

void MoveComponent::MoveHorizontal(float InVelocity)
{
	Velocity.X = InVelocity;
}

void MoveComponent::AddVeriticalImpulse(float InImpulse)
{
	//바닥에 있을때만 점프 가능
	if (IsOnGround)
	{
		Velocity.Y -= InImpulse;
		IsOnGround = false;
	}
}

void MoveComponent::ApplyGravity(float InDeltaTime)
{
	//바닥에	닿지 않았다면 중력 적용
	if (!IsOnGround)
	{
		Velocity.Y += GravityAcceleration * InDeltaTime;
	}
	else
	{
		Velocity.Y = 0.0f;
	}
}


