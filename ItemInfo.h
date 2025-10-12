#pragma once
#include "Common.h"
#include "enums.h"
#include <vector>

struct ItemSpawnInfo
{
	int ItemID = 0; //������ ID
	ItemType itemType;
	PointF Position; //��ġ
	ResourceID resourceID;

	ItemSpawnInfo() = default;
	ItemSpawnInfo(int InID, ItemType InElement, float InPositionX, float InPositionY,
		ResourceID InResID)
		: ItemID(InID), itemType(InElement), Position(InPositionX, InPositionY), resourceID(InResID) 
	{}
};

class ItemSpawnData
{
public:
	static std::vector<ItemSpawnInfo> GetLevel1SpawnInfo()
	{
		return{
			//������ ID, �� ���, ��ġ X, ��ġ Y, ���ҽ� ID
			//Key
			{ 1, ItemType::Key, 150.0f, 200.0f, ResourceID::Key_Blue},
			{ 2, ItemType::Key, 300.0f, 400.0f, ResourceID::Key_Red},
			{ 3, ItemType::Key, 500.0f, 600.0f, ResourceID::Key_Green },
			{ 4, ItemType::Key, 700.0f, 800.0f, ResourceID::Key_Yellow}
		};
	}

	static std::vector<ItemSpawnInfo> GetLevel2SpawnInfo()
	{
		return{
			//������ ID, �� ���, ��ġ X, ��ġ Y, ���ҽ� ID
			//Key
			{ 1, ItemType::Key, 150.0f, 200.0f, ResourceID::Key_Blue},
			{ 2, ItemType::Key, 300.0f, 400.0f, ResourceID::Key_Red},
			{ 3, ItemType::Key, 500.0f, 600.0f, ResourceID::Key_Green },
			{ 4, ItemType::Key, 700.0f, 800.0f, ResourceID::Key_Yellow}
		};
	}
};