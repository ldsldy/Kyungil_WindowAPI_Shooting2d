#pragma once
#include <unordered_map>
#include "enums.h"
#include "Actor.h"
#include "Key.h"

class Player : public Actor
{
public:
	Player() = delete;
	Player(ResourceID InID) : Actor(InID) {};

	virtual void OnInitialize() override;
	virtual void OnTick(float InDeltaTime) override;
	virtual void OnRender(Gdiplus::Graphics* InGraphics) override;
	virtual void OnOverlap(Actor* InOther) override;

	void PickupKey(Key* key); //����� ������ �� ȣ��Ǵ� �Լ�

	void AddKey(int KeyId);
	bool HasKey(int KeyId);
	void ResetKeys();

	//Ű �Է� ���� ó��
	void HandleKeyState(WPARAM InKey, bool InIsPressed);

	//Getters and Setters
	inline const std::vector<int>& GetOwnedKeys() const { return OwnedKeys; }
	inline float GetSpeed() const { return Speed; }

	inline void SetSpeed(float InSpeed) { Speed = InSpeed; }

private:
	//�÷��̾� �̵� �ӵ�
	float Speed = 200.0f;

	int WidthSize = 64;
	int HeightSize = 64;

	//�÷��̾� Ű �Է� ����
	std::unordered_map<InputDirection, bool> KeyWasPressedMap;

	std::vector<int> OwnedKeys; //�÷��̾ ������ ���� ID ����Ʈ
};

