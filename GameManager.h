#pragma once
#include <Windows.h>
#include <vector>
#include <set>
#include <map>
#include "Common.h"
#include "PhysicsComponent.h"
#include "Actor.h"
#include "Player.h"
#include "Singleton.h"
#include "TimerUI.h"
#include "InteractSpawner.h"
#include "ItemInfo.h"

class GameManager:public Singleton<GameManager>
{
	// friend : �ٸ� Ŭ������ �ڽ��� private/protected �׸� �����ϴ� ���� ������ش�.
	// Singleton<GameManager>�� GameManager�� private�� ���� ����������.
	friend class Singleton<GameManager>;

public:
	//�ʱ�ȭ
	void Initialize();
	//����
	void Destroy();
	//ƽ �й�
	void Tick(float InDeltaTime);
	//����� ���� �׸���
	void Render();
	//�Է� ó��
	void HandleKeyState(WPARAM InKey, bool InIsPressed);

	//���͸� ���� �Ŵ����� ���(ƽ�� ���� ó���� �����ϰ� ����)
	void RegistActor(RenderLayer InLayer, Actor* InActor);
	//���͸� ���� ��û
	inline void RequestDestroyActor(Actor* DestroyTarget) { PendingDestroyActors.push_back(DestroyTarget); };

	//ȭ�� ũ��
	static constexpr unsigned int ScreenWidth = 1680;
	static constexpr unsigned int ScreenHeight = 960;
	//static constexpr unsigned int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	//static constexpr unsigned int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	//���� �⺻ ũ��(�ȼ� ������)
	static constexpr unsigned int ActorDefaultSize = 64;

	//Getter
	inline const HWND GetMainWindowHandle() const { return hMainWindow; }
	inline const Point& GetAppPosition() const { return AppPosition; }
	inline Gdiplus::Bitmap* GetBackBuffer() const { return BackBuffer; }

	//Setter
	inline void SetMainWindowHandle(HWND InWindowHandle) {
		if (hMainWindow == nullptr)
		{
			hMainWindow = InWindowHandle;  //���� 1ȸ�� ���� ����
		}
	}
	inline void SetGameState(GameState InState) { State = InState; }
protected:
private:
	GameManager() = default;
	virtual ~GameManager() = default;

	//============================================== ====================
	///								�Լ�
	//===================================================================
	void UnregisteActor(Actor* InActor);	//���͸� ���� �Ŵ������� ��� ����
	void ProcessCollisions();				//�浹 ó��
	void ProcessPendingDestroyActors();		//�ı� ���� ���� ó��

	//���� ���� �ε�
	void LoadNextLevel();
	//���� ���� �ٽ÷ε�
	void ReloadCurrentLevel();

	//=======================================================================
	///								����
	//============================================== ========================
	std::map<RenderLayer, std::set<Actor*>> Actors; //RenderLayer���� Actor���� �����ϴ� ��
	std::vector<Actor*> PendingDestroyActors;		//�ı� ���� ���͵��� �����ϴ� ����(�̹� �����ӿ� ���� ��û�� ���� ���͵�)
	std::map<PhysicsLayer, std::vector<PhysicsComponent*>> PhysicsComponents; //���� ������Ʈ ����Ʈ

	HWND hMainWindow = nullptr;				//���� ������ �ڵ�
	Point AppPosition = Point(0,0);		//�� ���� ��ġ
	Gdiplus::Bitmap* BackBuffer = nullptr;	//����� ��Ʈ��
	Gdiplus::Graphics* BackBufferGraphics = nullptr; //����ۿ� �׸��� ���� ����

	Player* MainPlayer = nullptr;			//�÷��̾� ĳ����
	InteractSpawner* ItemSpawner = nullptr;         //������ ������
	TimerUI* Timer = nullptr;                //Ÿ�̸� UI

	//TestGridActor* TestGrid = nullptr;      //��ġ Ȯ���� ���� �׽�Ʈ�� �׸���

	GameState State = GameState::Playing;   //���� ����

	bool IsLevelCompleted = false;  //���� �Ϸ� ����
	float InLevelTimer = 0.0f; //���� ���� �ð�
};

