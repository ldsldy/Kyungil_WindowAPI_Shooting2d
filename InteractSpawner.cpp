#include "InteractSpawner.h"
#include "Key.h"
#include "Factory.h"

void InteractSpawner::OnInitialize()
{
	//초기 레벨 로드
	CurrentLevelType = LevelType::Level1;
	LoadLevel(CurrentLevelType);
}

void InteractSpawner::OnTick(float deltaTime)
{
	Actor::OnTick(deltaTime);
}

void InteractSpawner::LoadSpawnData(const std::vector<ItemSpawnInfo>& InSpawnData)
{
	ItemSpawnData = InSpawnData;
}

void InteractSpawner::SpawnAllItems()
{
	for (const auto& spawnInfo : ItemSpawnData)
	{
		Actor* newActor = CreateItemActorWithFactory(spawnInfo);
		if (newActor)
		{
			CreatedItems.push_back(newActor);
		}
	}
}

Actor* InteractSpawner::CreateItemActorWithFactory(const ItemSpawnInfo& InSpawnInfo)
{
	Actor* NewActor = nullptr;
	RenderLayer TargetLayer = RenderLayer::Misc;

	switch (InSpawnInfo.itemType)
	{
	case ItemType::Key:
		TargetLayer = RenderLayer::Key;
		NewActor = Factory::Get().SpawnActor<Key>(InSpawnInfo.resourceID, TargetLayer);
		if (NewActor)
		{
			SetupKey(NewActor, InSpawnInfo);
		}
		break;

	default:
		OutputDebugString(L"Unknown item type in CreateItemActorWithFactory\n");
		break;
	}
	return NewActor;
}

void InteractSpawner::SetupKey(Actor* actor, const ItemSpawnInfo& InSpawnInfo)
{
	//Actor를 Key로 캐스팅
	Key* key = dynamic_cast<Key*>(actor);
	if (key)
	{
		key->SetPosition(InSpawnInfo.Position.X, InSpawnInfo.Position.Y);
		key->SetKeyId(InSpawnInfo.ItemID);
	}
}

void InteractSpawner::LoadLevel(LevelType InLevel)
{
	//기존 맵 정보 제거
	for (Actor* Item : CreatedItems)
	{
		if (Item)
		{
			Item->DestroyActor();
		}
	}
	CreatedItems.clear();

	//새 레벨의 정보 불러오기
	CurrentLevelType = InLevel;
	LoadSpawnData(GetLevelData(InLevel));
	SpawnAllItems();
}

void InteractSpawner::ResetLevel()
{
	//생성된 모든 액터들을 삭제
	for(Actor* item: CreatedItems)
	{
		if (item)
		{
			item->DestroyActor();
		}
	}
	CreatedItems.clear();

	//현재 레벨 다시 로드
	for(auto& spawnInfo: ItemSpawnData)
	{
		Actor* newActor = CreateItemActorWithFactory(spawnInfo);
		if (newActor)
		{
			CreatedItems.push_back(newActor);
		}
	}

	SpawnAllItems();
}

std::vector<ItemSpawnInfo> InteractSpawner::GetLevelData(LevelType InLevel)
{
	switch (InLevel)
	{
	case LevelType::Level1:
		return ItemSpawnData::GetLevel1SpawnInfo();
	case LevelType::Level2:
		return ItemSpawnData::GetLevel2SpawnInfo();
	default:
		return ItemSpawnData::GetLevel1SpawnInfo();
	}
}
