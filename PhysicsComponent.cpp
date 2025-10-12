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

	//중력 적용
	ApplyGravity(InDeltaTime);

	//다음 위치 계산
    PointF NextPos = CalculateNextPosition(InDeltaTime);

	//바닥 충돌 검사
	CheckFloorCollision(InDeltaTime);

	//위치 업데이트
	PointF FinalPos = CalculateNextPosition(InDeltaTime);
	Owner->SetPosition(FinalPos.X, FinalPos.Y);
}

void PhysicsComponent::ApplyGravity(float InDeltaTime)
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

    // 바닥과 충돌 결과 적용
    if (FoundGround)
    {
        if (!IsOnGround)
        {
			//바닥에 처음 닿았을 때
            if (OnGroundHitCallback)
            {
				OnGroundHitCallback();
            }
        }
        SetIsOnGround(true);
        ResetVerticalVelocity();
     
        //위치 조정
        PointF CurrentPos = Owner->GetPosition();
        Owner->SetPosition(CurrentPos.X, GroundY);
    }
    else
    {
        if (IsOnGround) 
        {
            //바닥에서 벗어났을 때
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

    //충돌 레이어가 None이면 충돌하지 않음
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
	//유효성 검사
    if (!InFrom || !InTo) 
        return false;

	const PointF& From = InFrom->GetOwner()->GetPosition();
	const PointF& To = InTo->GetOwner()->GetPosition();

	// 두 원의 중심 간 거리 계산
    float dx = To.X - From.X;
    float dy = To.Y - From.Y;
    float squaredDistance = dx * dx + dy * dy;

    float radiusSum = InFrom->GetRadius() + InTo->GetRadius();
    float squaredRadiusSum = radiusSum * radiusSum;

	// 중심 간 거리의 제곱이 반지름 합의 제곱보다 작거나 같으면 충돌
    return squaredDistance<=squaredRadiusSum;
}

bool PhysicsComponent::CheckRectToRectCollision(const PhysicsComponent* InFrom, const PhysicsComponent* InTo)
{
    //유효성 검사
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
    //유효성 검사
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

    //원의 중심에서 다각형에 가장 가까운 점을 찾기
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
