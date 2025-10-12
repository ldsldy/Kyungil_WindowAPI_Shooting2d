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

	// ���߿� Force ���� �߰��� �� ���⼭ ó��
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
	//�ٴڿ� �������� ���� ����
	if (IsOnGround)
	{
		Velocity.Y -= InImpulse;
		IsOnGround = false;
	}
}

void MoveComponent::ApplyGravity(float InDeltaTime)
{
	//�ٴڿ�	���� �ʾҴٸ� �߷� ����
	if (!IsOnGround)
	{
		Velocity.Y += GravityAcceleration * InDeltaTime;
	}
	else
	{
		Velocity.Y = 0.0f;
	}
}


