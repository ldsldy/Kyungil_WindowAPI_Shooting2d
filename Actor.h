#pragma once
#include <type_traits>
#include <vector>
#include "Common.h"
#include "Component.h"

class Actor
{
public:
	Actor() = delete;
	Actor(ResourceID InID); //������ ResourceID �ʿ� -> ID�� ���ҽ� ����
	virtual ~Actor() = default;

	//�ڱ� �ڽ��� �����ϵ��� ��û�ϴ� �Լ�
	void DestroyActor();

	//�����Ǿ��� ���� ����
	virtual void OnInitialize() {};
	//���ŵ� ���� ����
	virtual void OnDestroy();
	//ƽ���� ����
	virtual void OnTick(float InDeltaTime);
	//�׸� ������ ����
	virtual void OnRender(Gdiplus::Graphics* InGraphics);

	virtual void OnOverlap(Actor* InOther){} //�ٸ� ���Ϳ� ������ �� �ڵ�ȣ�� �Ǵ� �Լ�

	void AddComponent(Component* InComponent);
	void RemoveComponent(Component* InComponent);

	template<typename T>
	T* GetComponent() const
	{
		// T�� Component���� �Ļ��� Ŭ�������� Ȯ��
		static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
		for (auto comp : Components)
		{
			T* castComp = dynamic_cast<T*> (comp);
			if (castComp)
			{
				return castComp;
			}
		}
		return nullptr;
	}
	template<typename T>
	bool HasComponent() const
	{
		return GetComponent<T>() != nullptr;
	}

	//���� �浹 ó��
	virtual void OnGroundCollision() {}
	virtual void OnGroundLeft() {};

	//Getter
	inline const PointF& GetPosition() const { return Position; }
	inline int GetSize() const { return Size; }
	inline const PointF& GetPivot() const { return Pivot; }
	inline const RenderLayer GetRenderLayer() const { return TargetRenderLayer; }

	//Setter
	inline void SetPosition(float InX, float InY) { Position.X = InX; Position.Y = InY; }
	inline void SetSize(int InSize) { Size = InSize;}
	inline void SetPivot(float InX, float InY) { Pivot.X = InX; Pivot.Y = InY; }
	inline void SetRenderLayer(RenderLayer InLayer) { TargetRenderLayer = InLayer; }

protected:
	// ��ġ
	PointF Position = { 0.0f, 0.0f };

	//����
	float Angle = 0.0f;

	//ũ��
	int Size = 64;

	//�߽���
	PointF Pivot = { 0.5f, 0.5f }; //�Ѱ��

	//�̹����� ������� ��Ʈ��
	Gdiplus::Bitmap* Image = nullptr;	//�÷��̾ �׷��� ����
	
	//�̹����� �׷��� ���̾�
	RenderLayer TargetRenderLayer = RenderLayer::Misc; // Misc : �⺻��

	//�� ���Ϳ� ���� ������Ʈ��
	std::vector<Component*> Components;

private:
	bool IsPendingDestroy = false; //���� ������ �������� ǥ��
};

