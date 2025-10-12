#include "GameManager.h"
#include "Background.h"
#include "Player.h"
#include "ResourceManager.h"
#include "Factory.h"
#include "PhysicsComponent.h"
#include "InteractSpawner.h"

#include <string>
//#include "TestGridActor.h"

void GameManager::Initialize()
{
	//������ ��ü���� ��� ����
	BackBuffer = new Gdiplus::Bitmap(
		GameManager::ScreenWidth, GameManager::ScreenHeight, PixelFormat32bppARGB);
	BackBufferGraphics = Gdiplus::Graphics::FromImage(BackBuffer);
	if (!BackBufferGraphics)
	{
		//�ȸ���� ���� ��� ���� ���
		MessageBox(hMainWindow, L"Failed to create BackBufferGraphics", L"Error", MB_OK|MB_ICONERROR);
	}

	MainPlayer = Factory::Get().SpawnActor<Player>(ResourceID::Player, RenderLayer::Player);
	Factory::Get().SpawnActor<Background>(ResourceID::Background, RenderLayer::Background);
	//TestGrid = Factory::Get().SpawnActor<TestGridActor>(ResourceID::None, RenderLayer::Test);

	ItemSpawner = Factory::Get().SpawnActor<InteractSpawner>(ResourceID::None);
	KeyInventory = Factory::Get().SpawnActor<KeyInventoryUI>(ResourceID::None, RenderLayer::UI);
}

void GameManager::Destroy()
{
	//�����ߴ� ��� �͵��� ����
	KeyInventory = nullptr;
	ItemSpawner = nullptr;
	MainPlayer = nullptr;
	//TestGrid = nullptr;

	PhysicsComponents.clear(); //���� ������Ʈ ����Ʈ �ʱ�ȭ
	for (auto pair : Actors)
	{
		for (Actor* actor : pair.second)
		{
			actor->OnDestroy();
			delete actor;
		}
		pair.second.clear();
	}
	Actors.clear();

	delete BackBufferGraphics;
	BackBufferGraphics = nullptr;
	delete BackBuffer;
	BackBuffer = nullptr;
}

void GameManager::Tick(float InDeltaTime)
{
	if (State == GameState::Playing)
	{
		for (const auto& pair : Actors)
		{
			for (Actor* actor : pair.second)
			{
				actor->OnTick(InDeltaTime);
			}
		}
		ProcessCollisions();			//�浹 ó��
		ProcessPendingDestroyActors();	//���� ������ ���͵��� ��� ����
	}
}

void GameManager::Render()
{
	if (BackBufferGraphics)
	{
		BackBufferGraphics->Clear(Gdiplus::Color(255, 0, 0, 0)); //��� ���������� �����

		for (const auto& pair : Actors)
		{
			for (Actor* actor : pair.second)
			{
				actor->OnRender(BackBufferGraphics);
			}
		}
	}
}

void GameManager::HandleKeyState(WPARAM InKey, bool InIsPressed)
{
	MainPlayer->HandleKeyState(InKey, InIsPressed);

	//if (TestGrid)
	//{
	//    TestGrid->DestroyActor();
	//    TestGrid = nullptr;
	//}
}

void GameManager::RegistActor(RenderLayer InLayer, Actor* InActor)
{
	if (InActor)
	{
		Actors[InLayer].insert(InActor); 

		//���� ������Ʈ�� ������ ���� ������Ʈ ����Ʈ���� ���
		PhysicsComponent* physicsComponent = InActor->GetComponent<PhysicsComponent>();
		if (physicsComponent && physicsComponent->GetLayer() != PhysicsLayer::None)
		{
			PhysicsComponents[physicsComponent->GetLayer()].push_back(physicsComponent);
		}
	}
}

void GameManager::UnregisteActor(Actor* InActor)
{
	std::set<Actor*>& actorSet = Actors[InActor->GetRenderLayer()];

	if (actorSet.find(InActor) != actorSet.end())
	{
		PhysicsComponent* physicsComponent = InActor->GetComponent<PhysicsComponent>();
		if (physicsComponent)
		{
			//���� ������Ʈ ����
			auto& physicsObjects = PhysicsComponents[physicsComponent->GetLayer()];
			auto it = std::find(physicsObjects.begin(), physicsObjects.end(), physicsComponent);
			if (it != physicsObjects.end())
			{
				//���� �� ��
				std::swap(*it, physicsObjects.back());
				physicsObjects.pop_back();
			}
		}
	}
	actorSet.erase(InActor);
}

void GameManager::ProcessCollisions()
{
	PhysicsComponent* player = *(PhysicsComponents[PhysicsLayer::Player].begin()); //�÷��̾�� 1��

	//�÷��̾ ��� ��ź�� �浹�ϴ����� Ȯ��
	//Ȯ�� �� ���� �ݸ��� Ÿ�Կ� ���� ó��(���� ��, ���� �簢��, �簢���� �簢�� �� 3���� ���̽�)
	for (auto& key : PhysicsComponents[PhysicsLayer::Key])
	{
		if (player->IsCollision(key)) //�÷��̾�� ��ź ���� �浹 Ȯ��
		{
			Key* nkey = dynamic_cast<Key*>(key->GetOwner());
			if (nkey)
				MainPlayer->OnOverlap(nkey);
		}
	}
}

void GameManager::ProcessPendingDestroyActors()
{
	for (Actor* actor : PendingDestroyActors)
	{
		if (actor)
		{
			UnregisteActor(actor); //���� �Ŵ������� ��� ����
			actor->OnDestroy();
			delete actor;
		}
	}
	PendingDestroyActors.clear();
}

bool GameManager::CheckFloorCollision(const PointF& NextPos, const PointF& velocity, float OutGroundY)
{
	PhysicsComponent* player = *(PhysicsComponents[PhysicsLayer::Player].begin()); //�÷��̾�� 1��

	if (!player) return false;

	bool FoundGround = false;
	float HighestY = NextPos.Y;

	for (auto& floor : PhysicsComponents[PhysicsLayer::FloorBlock])
	{
		if (player->IsCollision(floor))
		{
			FloorBlock* floorBlock = dynamic_cast<FloorBlock*>(floor->GetOwner());
			if (floorBlock && floorBlock->CanLandOn(NextPos, velocity))
			{
				float GroundY = floorBlock->GetTopY();

				if (!FoundGround || GroundY < HighestY)
				{
					FoundGround = true;
					HighestY = GroundY;
				}
			}
		}
	}
	return FoundGround;
}

void GameManager::LoadNextLevel()
{
	if (ItemSpawner)
	{
		InteractSpawner::LevelType currentLevel = ItemSpawner->GetCurrentLevel();
		InteractSpawner::LevelType nextLevel;

		switch (currentLevel)
		{
		case InteractSpawner::LevelType::Level1:
			nextLevel = InteractSpawner::LevelType::Level2;
			break;
		default:
			nextLevel = InteractSpawner::LevelType::Level1;
			break;
		}

		ItemSpawner->LoadLevel(nextLevel);

		if (MainPlayer)
		{
			MainPlayer->SetPosition(100.0f, 100.0f); //�÷��̾� ���� ��ġ�� �̵�
			MainPlayer->ResetKeys(); //�÷��̾ ���� ���� �ʱ�ȭ
		}
	}
}

void GameManager::ReloadCurrentLevel()
{
	if (ItemSpawner)
	{
		ItemSpawner->ResetLevel();

		if (MainPlayer)
		{
			MainPlayer->SetPosition(100.0f, 100.0f); //�÷��̾� ���� ��ġ�� �̵�
			MainPlayer->ResetKeys(); //�÷��̾ ���� ���� �ʱ�ȭ
		}
	}
}

void GameManager::UpdatePalyerKeyInventory()
{
	if (MainPlayer && KeyInventory)
	{
		const std::vector<int>& playerKeys = MainPlayer->GetOwnedKeys();
		KeyInventory->UpdateInventory(playerKeys);
	}
}