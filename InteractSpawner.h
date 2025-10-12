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

	//������ ���� ���� �ҷ����� �� ����
	//���� ���� �ҷ�����(��� ����)
	void LoadSpawnData(const std::vector<ItemSpawnInfo>& InSpawnData);
	//������ ����
	void SpawnAllItems();

	//�� ���� ����
	void LoadLevel(LevelType InLevel);
	LevelType GetCurrentLevel() const { return CurrentLevelType; }
	
	void ResetLevel();

private:
	//������ ���� ������ ���� ���� ��ȯ
	std::vector<ItemSpawnInfo> GetLevelData(LevelType InLevel);

	//������ ����
	Actor* CreateItemActorWithFactory(const ItemSpawnInfo& InSpawnInfo);
	void SetupKey(Actor* actor, const ItemSpawnInfo& InSpawnInfo);
	/*void SetupGem(Actor* actor,const ItemSpawnInfo& InSpawnInfo);
	void SetupJumpBlock(Actor* actor, const ItemSpawnInfo& InSpawnInfo);
	void SetupKeyBlock(Actor* actor, const ItemSpawnInfo& InSpawnInfo);
	void SetupFloorBlock(Actor* acto, const ItemSpawnInfo& InSpawnInfo);
	void SetupExitDoor(Actor* actor, const ItemSpawnInfo& InSpawnInfo);*/

private:
	LevelType CurrentLevelType=LevelType::Level1; //���� ���� ����
	std::vector<ItemSpawnInfo> ItemSpawnData; //���� ���� ������ ���� ����

	std::vector<Actor*> CreatedItems; //������ �����۵�
};

