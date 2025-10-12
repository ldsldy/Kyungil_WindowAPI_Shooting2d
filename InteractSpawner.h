#pragma once
#include "Actor.h"
#include "ItemInfo.h"
#include <vector>

class InteractSpawner : public Actor
{
public:
	enum class LevelType
	{
		Level1 = 0,
		Level2
	};

	InteractSpawner() = delete;
	InteractSpawner(ResourceID resourceID) :Actor(resourceID) {};
	virtual ~InteractSpawner() = default;

	virtual void OnInitialize() override;
	virtual void OnTick(float deltaTime) override;
	virtual void OnRender(Gdiplus::Graphics* InGraphics) override {}

	//아이템 스폰 정보 불러오기 및 스폰
	//스폰 정보 불러오기(모든 레벨)
	void LoadSpawnData(const std::vector<ItemSpawnInfo>& InSpawnData);
	//아이템 스폰
	void SpawnAllItems();

	//맵 레벨 관리
	void LoadLevel(LevelType InLevel);
	LevelType GetCurrentLevel() const { return CurrentLevelType; }
	
	void ResetLevel();

private:
	//레벨에 따른 아이템 스폰 정보 반환
	std::vector<ItemSpawnInfo> GetLevelData(LevelType InLevel);

	//아이템 생성
	Actor* CreateItemActorWithFactory(const ItemSpawnInfo& InSpawnInfo);
	void SetupKey(Actor* actor, const ItemSpawnInfo& InSpawnInfo);
	/*void SetupGem(Actor* actor,const ItemSpawnInfo& InSpawnInfo);
	void SetupJumpBlock(Actor* actor, const ItemSpawnInfo& InSpawnInfo);
	void SetupKeyBlock(Actor* actor, const ItemSpawnInfo& InSpawnInfo);
	void SetupFloorBlock(Actor* acto, const ItemSpawnInfo& InSpawnInfo);
	void SetupExitDoor(Actor* actor, const ItemSpawnInfo& InSpawnInfo);*/

private:
	LevelType CurrentLevelType=LevelType::Level1; //현재 맵의 레벨
	std::vector<ItemSpawnInfo> ItemSpawnData; //현재 맵의 아이템 스폰 정보

	std::vector<Actor*> CreatedItems; //생성된 아이템들
};

