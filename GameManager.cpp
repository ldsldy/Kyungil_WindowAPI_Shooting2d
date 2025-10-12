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
	//관리할 객체들을 모두 생성
	BackBuffer = new Gdiplus::Bitmap(
		GameManager::ScreenWidth, GameManager::ScreenHeight, PixelFormat32bppARGB);
	BackBufferGraphics = Gdiplus::Graphics::FromImage(BackBuffer);
	if (!BackBufferGraphics)
	{
		//안만들어 졌을 경우 에러 출력
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
	//생성했던 모든 것들을 정리
	KeyInventory = nullptr;
	ItemSpawner = nullptr;
	MainPlayer = nullptr;
	//TestGrid = nullptr;

	PhysicsComponents.clear(); //물리 컴포넌트 리스트 초기화
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
		ProcessCollisions();			//충돌 처리
		ProcessPendingDestroyActors();	//삭제 예정인 액터들을 모두 삭제
	}
}

void GameManager::Render()
{
	if (BackBufferGraphics)
	{
		BackBufferGraphics->Clear(Gdiplus::Color(255, 0, 0, 0)); //배경 검정색으로 지우기

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

		//물리 컴포넌트가 있으면 물리 컴포넌트 리스트에도 등록
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
			//물리 컴포넌트 제거
			auto& physicsObjects = PhysicsComponents[physicsComponent->GetLayer()];
			auto it = std::find(physicsObjects.begin(), physicsObjects.end(), physicsComponent);
			if (it != physicsObjects.end())
			{
				//스왑 앤 팝
				std::swap(*it, physicsObjects.back());
				physicsObjects.pop_back();
			}
		}
	}
	actorSet.erase(InActor);
}

void GameManager::ProcessCollisions()
{
	PhysicsComponent* player = *(PhysicsComponents[PhysicsLayer::Player].begin()); //플레이어는 1명

	//플레이어가 모든 폭탄과 충돌하는지만 확인
	//확인 할 때는 콜리전 타입에 따라 처리(원과 원, 원과 사각형, 사각형과 사각형 총 3가지 케이스)
	for (auto& key : PhysicsComponents[PhysicsLayer::Key])
	{
		if (player->IsCollision(key)) //플레이어와 폭탄 간의 충돌 확인
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
			UnregisteActor(actor); //게임 매니저에서 등록 해제
			actor->OnDestroy();
			delete actor;
		}
	}
	PendingDestroyActors.clear();
}

bool GameManager::CheckFloorCollision(const PointF& NextPos, const PointF& velocity, float OutGroundY)
{
	PhysicsComponent* player = *(PhysicsComponents[PhysicsLayer::Player].begin()); //플레이어는 1명

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
			MainPlayer->SetPosition(100.0f, 100.0f); //플레이어 시작 위치로 이동
			MainPlayer->ResetKeys(); //플레이어가 가진 열쇠 초기화
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
			MainPlayer->SetPosition(100.0f, 100.0f); //플레이어 시작 위치로 이동
			MainPlayer->ResetKeys(); //플레이어가 가진 열쇠 초기화
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