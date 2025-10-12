#include "GameManager.h"
#include "Background.h"
#include "Player.h"
#include "ResourceManager.h"
#include "Factory.h"
#include "PhysicsComponent.h"
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

	Spawner = Factory::Get().SpawnActor<BombSpawner>(ResourceID::None);
	Timer = Factory::Get().SpawnActor<TimerUI>(ResourceID::None, RenderLayer::UI);
}

void GameManager::Destroy()
{
	//�����ߴ� ��� �͵��� ����
	Timer = nullptr;
	Spawner = nullptr;
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
	for (auto& bomb : PhysicsComponents[PhysicsLayer::Bomb])
	{
		if (player->IsCollision(bomb)) //�÷��̾�� ��ź ���� �浹 Ȯ��
		{
			//�浹 �߻� �� �÷��̾�� ��ź�� OnOverlap ȣ��
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
			UnregisteActor(actor); //���� �Ŵ������� ��� ����
			actor->OnDestroy();
			delete actor;
		}
	}
	PendingDestroyActors.clear();
}