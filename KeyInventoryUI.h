#pragma once
#include "UserInterface.h"
#include <vector>

class KeyInventoryUI :public UserInterface
{
public:
	KeyInventoryUI() = delete;
	KeyInventoryUI(ResourceID resourceID) : UserInterface(resourceID) {};
	virtual ~KeyInventoryUI() = default;
	
	virtual void OnInitialize() override;
	virtual void OnTick(float deltaTime) override;
	virtual void OnRender(Gdiplus::Graphics* graphics) override;
	
	void UpdateInventory(const std::vector<int>& ownedKeys);

private:
	std::vector<int> CurrentKeys; //현재 소유한 열쇠 ID 리스트
	float KeyIconSize = 32.0f; //열쇠 아이콘 크기
	float KeyIconSpacing = 5.0f; //열쇠 아이콘 간격

	//열쇠 ID에 해당하는 ResourceID 반환
	ResourceID GetKeyResourceID(int keyId); 
};

