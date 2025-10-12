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
	std::vector<int> CurrentKeys; //���� ������ ���� ID ����Ʈ
	float KeyIconSize = 32.0f; //���� ������ ũ��
	float KeyIconSpacing = 5.0f; //���� ������ ����

	//���� ID�� �ش��ϴ� ResourceID ��ȯ
	ResourceID GetKeyResourceID(int keyId); 
};

