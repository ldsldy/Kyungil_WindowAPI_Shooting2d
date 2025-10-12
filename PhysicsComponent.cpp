#include "PhysicsComponent.h"
#include "Actor.h"
#include "GameManager.h"


void PhysicsComponent::OnTick(float InDeltaTime)
{
    if(bCanMove)
    {
        UpdatePhysics(InDeltaTime);
	}
}

void PhysicsComponent::UpdatePhysics(float InDeltaTime)
{
    if (!Owner) return;

	//�߷� ����
	ApplyGravity(InDeltaTime);

	//���� ��ġ ���
    PointF NextPos = CalculateNextPosition(InDeltaTime);

	//�ٴ� �浹 �˻�
	CheckFloorCollision(InDeltaTime);

	//��ġ ������Ʈ
	PointF FinalPos = CalculateNextPosition(InDeltaTime);
	Owner->SetPosition(FinalPos.X, FinalPos.Y);
}

void PhysicsComponent::ApplyGravity(float InDeltaTime)
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

void PhysicsComponent::CheckFloorCollision(float InDeltaTime)
{
	PointF NextPos = CalculateNextPosition(InDeltaTime);

	bool FoundGround = false;
	float GroundY = NextPos.Y;

	const auto& floorBlocks = GameManager::Get().GetFloorComponents();

    for (auto& floor : floorBlocks)
    {
        if (this->IsCollision(floor))
        {
            FloorBlock* floorBlock = dynamic_cast<FloorBlock*>(floor->GetOwner());
            if (floorBlock && floorBlock->CanLandOn(NextPos, Velocity))
            {
                float ThisGroundY = floorBlock->GetTopY();
                if (!FoundGround || ThisGroundY < GroundY)
                {
                    FoundGround = true;
                    GroundY = ThisGroundY;
                }
			}
        }
    }

    // �ٴڰ� �浹 ��� ����
    if (FoundGround)
    {
        if (!IsOnGround)
        {
			//�ٴڿ� ó�� ����� ��
            if (OnGroundHitCallback)
            {
				OnGroundHitCallback();
            }
        }
        SetIsOnGround(true);
        ResetVerticalVelocity();
     
        //��ġ ����
        PointF CurrentPos = Owner->GetPosition();
        Owner->SetPosition(CurrentPos.X, GroundY);
    }
    else
    {
        if (IsOnGround) 
        {
            //�ٴڿ��� ����� ��
            if (OnGroundLeftCallback)
            {
				OnGroundLeftCallback();
            }
        }
		SetIsOnGround(false);
    }
}


bool PhysicsComponent::IsCollision(PhysicsComponent* InOther) const
{
    bool Result = false;

    if (!InOther || InOther == this)
        return Result;

    //�浹 ���̾ None�̸� �浹���� ����
	if (this->GetLayer() == PhysicsLayer::None || InOther->GetLayer() == PhysicsLayer::None)
        return Result;

    if (Collision == InOther->GetCollisionType())
    {
        switch (Collision)
        {
        case CollisionType::Circle:
            Result = CheckCircleToCircleCollision(this, InOther);
			break;
        case CollisionType::Rectangle:
			Result = CheckRectToRectCollision(this, InOther);
            break;
        }
    }
    else
    {
		Result = CheckCircleToRectCollision(this, InOther);
    }

    return Result;
}


bool PhysicsComponent::CheckCircleToCircleCollision(const PhysicsComponent* InFrom, const PhysicsComponent* InTo)
{
	//��ȿ�� �˻�
    if (!InFrom || !InTo) 
        return false;

	const PointF& From = InFrom->GetOwner()->GetPosition();
	const PointF& To = InTo->GetOwner()->GetPosition();

	// �� ���� �߽� �� �Ÿ� ���
    float dx = To.X - From.X;
    float dy = To.Y - From.Y;
    float squaredDistance = dx * dx + dy * dy;

    float radiusSum = InFrom->GetRadius() + InTo->GetRadius();
    float squaredRadiusSum = radiusSum * radiusSum;

	// �߽� �� �Ÿ��� ������ ������ ���� �������� �۰ų� ������ �浹
    return squaredDistance<=squaredRadiusSum;
}

bool PhysicsComponent::CheckRectToRectCollision(const PhysicsComponent* InFrom, const PhysicsComponent* InTo)
{
    //��ȿ�� �˻�
    if (!InFrom || !InTo)
        return false;

    const PointF& From = InFrom->GetOwner()->GetPosition();
    const PointF& To = InTo->GetOwner()->GetPosition();

    float HalfWidthFrom = InFrom->GetWidth() * 0.5f;
    float HalfHeightFrom = InFrom->GetHeight() * 0.5f;
    float HalfWidthTo = InTo->GetWidth() * 0.5f;
    float HalfHeightTo = InTo->GetHeight() * 0.5f;

    float FromLeft = From.X - HalfWidthFrom;
    float FromRight = From.X + HalfWidthFrom;
    float FromTop = From.Y - HalfHeightFrom;
    float FromBottom = From.Y + HalfHeightFrom;

	float ToLeft = To.X - HalfWidthTo;
	float ToRight = To.X + HalfWidthTo;
	float ToTop = To.Y - HalfHeightTo;
	float ToBottom = To.Y + HalfHeightTo;

    return !(FromRight < ToLeft || FromLeft>ToRight || FromBottom < ToTop || FromTop >ToBottom);
}

bool PhysicsComponent::CheckCircleToRectCollision(const PhysicsComponent* InFrom, const PhysicsComponent* InTo)
{
    //��ȿ�� �˻�
    if (!InFrom || !InTo)
        return false;

    const PhysicsComponent* Circle;
    const PhysicsComponent* Rect;

    if(InFrom->GetCollisionType() == CollisionType::Circle)
    {
        Circle = InFrom;
        Rect = InTo;
    }
    else
    {
        Circle = InTo;
        Rect = InFrom;
	}

    const PointF& From = InFrom->GetOwner()->GetPosition();
    const PointF& To = InTo->GetOwner()->GetPosition();

    float RectLeft = From.X - Rect->GetWidth() * 0.5f;
    float RectRight = From.X + Rect->GetWidth() * 0.5f;
    float RectTop = From.Y - Rect->GetHeight() * 0.5f;
    float RectBottom = From.Y + Rect->GetHeight() * 0.5f;

    //���� �߽ɿ��� �ٰ����� ���� ����� ���� ã��
    float ClosestX = To.X;
    float ClosestY = To.Y;

    if (To.X < RectLeft)
        ClosestX = RectLeft;
    else if (To.X > RectRight)
        ClosestX = RectRight;

    if (To.Y < RectTop)
        ClosestY = RectTop;
    else if (To.Y > RectBottom)
        ClosestY = RectBottom;

    float dx = To.X - ClosestX;
	float dy = To.Y - ClosestY;
	float squaredDistance = dx * dx + dy * dy;

    return squaredDistance <= (Circle->GetRadius() * Circle->GetRadius());
}
