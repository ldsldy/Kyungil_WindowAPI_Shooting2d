#pragma once
#include "Common.h"
#include "enums.h"
#include <vector>

struct ItemSpawnInfo
{
	int ItemID = 0; //아이템 ID
	ItemType itemType;
	PointF Position; //위치
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
			//아이템 ID, 맵 요소, 위치 X, 위치 Y, 리소스 ID
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
			//아이템 ID, 맵 요소, 위치 X, 위치 Y, 리소스 ID
			//Key
			{ 1, ItemType::Key, 150.0f, 200.0f, ResourceID::Key_Blue},
			{ 2, ItemType::Key, 300.0f, 400.0f, ResourceID::Key_Red},
			{ 3, ItemType::Key, 500.0f, 600.0f, ResourceID::Key_Green },
			{ 4, ItemType::Key, 700.0f, 800.0f, ResourceID::Key_Yellow}
		};
	}
};