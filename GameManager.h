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
	// friend : 다른 클래스가 자신의 private/protected 항목에 접근하는 것을 허용해준다.
	// Singleton<GameManager>가 GameManager의 private에 접근 가능해진다.
	friend class Singleton<GameManager>;

public:
	//초기화
	void Initialize();
	//정리
	void Destroy();
	//틱 분배
	void Tick(float InDeltaTime);
	//백버퍼 내용 그리기
	void Render();
	//입력 처리
	void HandleKeyState(WPARAM InKey, bool InIsPressed);

	//액터를 게임 매니저에 등록(틱과 랜더 처리를 가능하게 해줌)
	void RegistActor(RenderLayer InLayer, Actor* InActor);
	//액터를 삭제 요청
	inline void RequestDestroyActor(Actor* DestroyTarget) { PendingDestroyActors.push_back(DestroyTarget); };

	//화면 크기
	static constexpr unsigned int ScreenWidth = 1680;
	static constexpr unsigned int ScreenHeight = 960;
	//static constexpr unsigned int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	//static constexpr unsigned int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	//액터 기본 크기(픽셀 사이즈)
	static constexpr unsigned int ActorDefaultSize = 64;

	//Getter
	inline const HWND GetMainWindowHandle() const { return hMainWindow; }
	inline const Point& GetAppPosition() const { return AppPosition; }
	inline Gdiplus::Bitmap* GetBackBuffer() const { return BackBuffer; }

	//Setter
	inline void SetMainWindowHandle(HWND InWindowHandle) {
		if (hMainWindow == nullptr)
		{
			hMainWindow = InWindowHandle;  //최초 1회만 설정 가능
		}
	}
	inline void SetGameState(GameState InState) { State = InState; }
protected:
private:
	GameManager() = default;
	virtual ~GameManager() = default;

	//============================================== ====================
	///								함수
	//===================================================================
	void UnregisteActor(Actor* InActor);	//액터를 게임 매니저에서 등록 해제
	void ProcessCollisions();				//충돌 처리
	void ProcessPendingDestroyActors();		//파괴 예정 액터 처리

	//다음 레벨 로드
	void LoadNextLevel();
	//현재 레벨 다시로드
	void ReloadCurrentLevel();

	//=======================================================================
	///								변수
	//============================================== ========================
	std::map<RenderLayer, std::set<Actor*>> Actors; //RenderLayer별로 Actor들을 관리하는 맵
	std::vector<Actor*> PendingDestroyActors;		//파괴 예정 액터들을 관리하는 벡터(이번 프레임에 삭제 요청이 들어온 액터들)
	std::map<PhysicsLayer, std::vector<PhysicsComponent*>> PhysicsComponents; //물리 컴포넌트 리스트

	HWND hMainWindow = nullptr;				//메인 윈도우 핸들
	Point AppPosition = Point(0,0);		//앱 시작 위치
	Gdiplus::Bitmap* BackBuffer = nullptr;	//백버퍼 비트맵
	Gdiplus::Graphics* BackBufferGraphics = nullptr; //백버퍼에 그리기 위한 도구

	Player* MainPlayer = nullptr;			//플레이어 캐릭터
	InteractSpawner* ItemSpawner = nullptr;         //아이템 생성기
	TimerUI* Timer = nullptr;                //타이머 UI

	//TestGridActor* TestGrid = nullptr;      //위치 확인을 위한 테스트용 그리드

	GameState State = GameState::Playing;   //게임 상태

	bool IsLevelCompleted = false;  //레벨 완료 여부
	float InLevelTimer = 0.0f; //레벨 진행 시간
};

