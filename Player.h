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

	void PickupKey(Key* key); //열쇠와 겹쳤을 때 호출되는 함수

	void AddKey(int KeyId);
	bool HasKey(int KeyId);
	void ResetKeys();

	//키 입력 상태 처리
	void HandleKeyState(WPARAM InKey, bool InIsPressed);

	//Getters and Setters
	inline const std::vector<int>& GetOwnedKeys() const { return OwnedKeys; }
	inline float GetSpeed() const { return Speed; }

	inline void SetSpeed(float InSpeed) { Speed = InSpeed; }

private:
	//플레이어 이동 속도
	float Speed = 200.0f;

	int WidthSize = 64;
	int HeightSize = 64;

	//플레이어 키 입력 상태
	std::unordered_map<InputDirection, bool> KeyWasPressedMap;

	std::vector<int> OwnedKeys; //플레이어가 소유한 열쇠 ID 리스트
};

