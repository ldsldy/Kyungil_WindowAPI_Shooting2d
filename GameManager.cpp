#include "GameManager.h"
#include "Background.h"
#include "Player.h"
#include "ResourceManager.h"
#include "Factory.h"
#include "PhysicsComponent.h"
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

	Spawner = Factory::Get().SpawnActor<BombSpawner>(ResourceID::None);
	Timer = Factory::Get().SpawnActor<TimerUI>(ResourceID::None, RenderLayer::UI);
}

void GameManager::Destroy()
{
	//생성했던 모든 것들을 정리
	Timer = nullptr;
	Spawner = nullptr;
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
	for (auto& bomb : PhysicsComponents[PhysicsLayer::Bomb])
	{
		if (player->IsCollision(bomb)) //플레이어와 폭탄 간의 충돌 확인
		{
			//충돌 발생 시 플레이어와 폭탄의 OnOverlap 호출
			player->GetOwner()->OnOverlap(bomb->GetOwner());
			bomb->GetOwner()->OnOverlap(player->GetOwner());
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