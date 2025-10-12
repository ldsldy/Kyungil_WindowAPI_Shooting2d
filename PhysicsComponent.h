#pragma once
#include "Component.h"
#include "enums.h"
#include "Common.h"
#include <functional>

class PhysicsComponent:public Component
{
public:
	PhysicsComponent(Actor* InOwner, CollisionType InType = CollisionType::Circle, PhysicsLayer InLayer = PhysicsLayer::None)
	:Component(InOwner), Collision(InType), Layer(InLayer){}

	virtual void OnTick(float InDeltaTime) override;

	//충돌 검사
	bool IsCollision(PhysicsComponent* InOther) const;

	//Getter
	inline float GetRadius() const { return Data1; } //원형 충돌의 반지름 반환
	inline float GetWidth() const { return Data1; }  //사각형 충돌의 너비 반환
	inline float GetHeight() const { return Data2; } //사각형 충돌의 높이 반환
	inline CollisionType GetCollisionType() const { return Collision; } //충돌 타입 반환
	inline PhysicsLayer GetLayer() const { return Layer; } //물리 레이어 반환

	//Setter
	inline void SetRadius(float InRadius) { Data1 = InRadius; } //원형 충돌의 반지름 설정
	inline void SetWidth(float InWidth) { Data1 = InWidth; } //사각형 충돌의 너비 설정
	inline void SetHeight(float InHeight) { Data2 = InHeight; } //사각형 충돌의 높이 설정
	inline void SetCanMove(bool InCanMove) { bCanMove = InCanMove; } //이 컴포넌트가 속한 액터가 움직일 수 있는지 설정

	//이동 관련 함수
	void MoveHorizontal(float InVelocity);
	void AddVeriticalImpulse(float InImpulse);

	//중력 설정
	inline void SetGravity(float InGravity) { GravityAcceleration = InGravity; }
	inline const float GetGravity() const { return GravityAcceleration; }

	//속도
	inline PointF GetVelocity() const { return Velocity; }
	inline void SetVelocity(const PointF& InVelocity) { Velocity = InVelocity; }
	inline void ResetVerticalVelocity() { Velocity.Y = 0.0f; }

	//바닥 접촉 여부
	inline void SetIsOnGround(bool InIsOnGround) { IsOnGround = InIsOnGround; }
	inline bool GetIsOnGround() const { return IsOnGround; }

	//다음 위치 계산
	PointF CalculateNextPosition(float InDeltaTime);

	//물리 업데이트
	void UpdatePhysics(float InDeltaTime);
private:
	static bool CheckCircleToCircleCollision(const PhysicsComponent* InFrom, const PhysicsComponent* InTo);
	static bool CheckRectToRectCollision(const PhysicsComponent* InFrom, const PhysicsComponent* InTo);
	static bool CheckCircleToRectCollision(const PhysicsComponent* InFrom, const PhysicsComponent* InTo);

	CollisionType Collision = CollisionType::Circle;
	PhysicsLayer Layer = PhysicsLayer::None;
	float Data1 = 0.0f;
	float Data2 = 0.0f;

	bool bCanMove = false; //이 컴포넌트가 속한 액터가 움직일 수 있는지 여부

	//물리 상태
	float GravityAcceleration = 980.0f; //중력 가속도
	PointF Velocity = PointF(0.0f, 0.0f); //현재 속도
	bool IsOnGround = false; //바닥에 접촉했는지 여부

	//콜백 함수
	std::function<void()> OnGroundHitCallback = nullptr;
	std::function<void()> OnGroundLeftCallback = nullptr;

	//물리 계산
	void ApplyGravity(float InDeltaTime);
	void CheckFloorCollision(float InDeltaTime);
};

