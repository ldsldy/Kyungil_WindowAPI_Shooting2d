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

	//�浹 �˻�
	bool IsCollision(PhysicsComponent* InOther) const;

	//Getter
	inline float GetRadius() const { return Data1; } //���� �浹�� ������ ��ȯ
	inline float GetWidth() const { return Data1; }  //�簢�� �浹�� �ʺ� ��ȯ
	inline float GetHeight() const { return Data2; } //�簢�� �浹�� ���� ��ȯ
	inline CollisionType GetCollisionType() const { return Collision; } //�浹 Ÿ�� ��ȯ
	inline PhysicsLayer GetLayer() const { return Layer; } //���� ���̾� ��ȯ

	//Setter
	inline void SetRadius(float InRadius) { Data1 = InRadius; } //���� �浹�� ������ ����
	inline void SetWidth(float InWidth) { Data1 = InWidth; } //�簢�� �浹�� �ʺ� ����
	inline void SetHeight(float InHeight) { Data2 = InHeight; } //�簢�� �浹�� ���� ����
	inline void SetCanMove(bool InCanMove) { bCanMove = InCanMove; } //�� ������Ʈ�� ���� ���Ͱ� ������ �� �ִ��� ����

	//�̵� ���� �Լ�
	void MoveHorizontal(float InVelocity);
	void AddVeriticalImpulse(float InImpulse);

	//�߷� ����
	inline void SetGravity(float InGravity) { GravityAcceleration = InGravity; }
	inline const float GetGravity() const { return GravityAcceleration; }

	//�ӵ�
	inline PointF GetVelocity() const { return Velocity; }
	inline void SetVelocity(const PointF& InVelocity) { Velocity = InVelocity; }
	inline void ResetVerticalVelocity() { Velocity.Y = 0.0f; }

	//�ٴ� ���� ����
	inline void SetIsOnGround(bool InIsOnGround) { IsOnGround = InIsOnGround; }
	inline bool GetIsOnGround() const { return IsOnGround; }

	//���� ��ġ ���
	PointF CalculateNextPosition(float InDeltaTime);

	//���� ������Ʈ
	void UpdatePhysics(float InDeltaTime);
private:
	static bool CheckCircleToCircleCollision(const PhysicsComponent* InFrom, const PhysicsComponent* InTo);
	static bool CheckRectToRectCollision(const PhysicsComponent* InFrom, const PhysicsComponent* InTo);
	static bool CheckCircleToRectCollision(const PhysicsComponent* InFrom, const PhysicsComponent* InTo);

	CollisionType Collision = CollisionType::Circle;
	PhysicsLayer Layer = PhysicsLayer::None;
	float Data1 = 0.0f;
	float Data2 = 0.0f;

	bool bCanMove = false; //�� ������Ʈ�� ���� ���Ͱ� ������ �� �ִ��� ����

	//���� ����
	float GravityAcceleration = 980.0f; //�߷� ���ӵ�
	PointF Velocity = PointF(0.0f, 0.0f); //���� �ӵ�
	bool IsOnGround = false; //�ٴڿ� �����ߴ��� ����

	//�ݹ� �Լ�
	std::function<void()> OnGroundHitCallback = nullptr;
	std::function<void()> OnGroundLeftCallback = nullptr;

	//���� ���
	void ApplyGravity(float InDeltaTime);
	void CheckFloorCollision(float InDeltaTime);
};

