#include "BombSpawner.h"
#include "Factory.h"
#include "Bomb.h"

void BombSpawner::OnInitialize()
{
	SetPosition(0, -100); //화면 밖에 위치 설정.
	
	timeSinceLastSpawn = 0.0f;
	hasInitialDelayPassed = false;
}

void BombSpawner::OnTick(float deltaTime)
{
	Actor::OnTick(deltaTime); //부모 클래스의 OnTick 호출

	//경과 시간 업데이트
	timeSinceLastSpawn += deltaTime;

	//초기 지연 시간이 지났는지 확인
	if (!hasInitialDelayPassed)
	{
		if(timeSinceLastSpawn >= initialDelay)
		{
			hasInitialDelayPassed = true;
			timeSinceLastSpawn = 0.0f; //초기 지연 후 시간 초기화
		}
		return;
	}

	//초기 지연 시간이 지난 후의 폭탄 생성 로직
	if(timeSinceLastSpawn >= spawnInterval)
	{
		//폭탄 생성
		Bomb* newBomb = Factory::Get().SpawnActor<Bomb>(ResourceID::Bomb, RenderLayer::Bomb);

		//타이머 리셋
		timeSinceLastSpawn -= spawnInterval;
	}
}
