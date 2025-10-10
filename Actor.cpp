#include "Actor.h"
#include "GameManager.h"
#include "ResourceManager.h"

Actor::Actor(ResourceID InID)
{
	Image = ResourceManager::Get().GetImage(InID);
}

void Actor::Actor::DestroyActor()
{
	if (!IsPendingDestroy)
	{
		IsPendingDestroy = true;
		GameManager::Get().RequestDestroyActor(this);  //GameManager�� �ı� ��û
	}
}

void Actor::OnDestroy()
{
	while (!Components.empty())
	{
		RemoveComponent(Components[0]);
	}
	Components.clear();  //������Ʈ ����Ʈ �ʱ�ȭ
}

void Actor::OnTick(float InDeltaTime)
{
	//������Ʈ ������Ʈ
	for (auto& component : Components)
	{
		component->OnTick(InDeltaTime);
	}
}

void Actor::OnRender(Gdiplus::Graphics* InGraphics)
{
	if (!InGraphics) return;
	if (!Image) return;

	//���� ��ȯ ���� ����
	Gdiplus::Matrix oldTransform;
	InGraphics->GetTransform(&oldTransform);
	
	//ȸ�� �߽����� ��ü�� �߽����� ����
	InGraphics->TranslateTransform(Position.X, Position.Y);

	//������ ������ŭ ȸ��
	InGraphics->RotateTransform(Angle);

	InGraphics->TranslateTransform(-Position.X, -Position.Y); //���� ��ġ�� �ٽ� �̵�

	//Image�� �ε��Ǿ� �ִ�.
	InGraphics->DrawImage(
		Image,
		static_cast<int>(Position.X - Size * Pivot.X), //�׷��� ��ġ
		static_cast<int>(Position.Y - Size * Pivot.Y),
		Size, Size); //�׷��� ������

	//���� ��ȯ ���·� ����
	InGraphics->SetTransform(&oldTransform);
}

void Actor::AddComponent(Component* InComponent)
{
	if (InComponent)
	{
		Components.push_back(InComponent);
		InComponent->OnInitialize();  //������Ʈ �ʱ�ȭ
	}
}

void Actor::RemoveComponent(Component* InComponent)
{
	if (InComponent)
	{
		auto it = std::find(Components.begin(), Components.end(), InComponent);
		if (it != Components.end())
		{
			(*it)->OnDestroy();  //������Ʈ �Ҹ�

			std::swap(*it, Components.back()); //������� ������Ʈ�� �� �ڷ� ����
			Components.pop_back();
			delete InComponent;
			InComponent = nullptr;
		}
	}
}
