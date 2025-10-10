#include "TimerUI.h"
#include "ResourceManager.h"
#include "GameManager.h"

void TimerUI::OnInitialize()
{
	Actor::OnInitialize(); //부모의 OnInitialize 호출
	
	SetSize(GameManager::ActorDefaultSize / 2); //크기 설정
	anchoredPosition = { -10,10 };			//앵커를 기준으로한 위치 설정
	anchor = { 1.0f,0.0f };					//앵커 포인트를 오른쪽 상단으로 설정
	Pivot = { 1.0f,0.0f };					//피벗 포인트를 오른쪽 상단으로 설정
	width = Size * 4;
	height = Size;
	UpdatePosition();

	digits.reserve(3);		 //최소 3개 자리 숫자를 저장할 수 있도록 예약
	digitImages.reserve(10); //0~9 10개 숫자 이미지를 저장할 수 있도록 예약

	//0~9까지의 숫자 이미지를 로드
	for (int i = 0; i < 10; i++)
	{
		digitImages.push_back(ResourceManager::Get().GetImage(static_cast<ResourceID>(static_cast<int>(ResourceID::Number0) + i)));
	}
	dotImage = ResourceManager::Get().GetImage(ResourceID::Dot);
}

void TimerUI::OnTick(float deltaTime)
{
	Actor::OnTick(deltaTime); //부모의 OnTick 호출
	elapsedTime += deltaTime; //경과 시간 갱신
	SetNumber(elapsedTime);   //숫자 설정
}

void TimerUI::OnRender(Gdiplus::Graphics* graphics)
{
	if (!graphics) return;

	int digitCount = static_cast<int>(digits.size());
	int indexOffset = digitCount;
	for (int i = 0; i < digitCount; ++i)
	{
		//if (i == 2)
		//{
		//	//소수점 그리기
		//	graphics->DrawImage(dotImage, static_cast<int>(Position.X) + indexOffset * Size, static_cast<int>(Position.Y), Size, Size);
		//	indexOffset--;
		//}
		RenderDigit(graphics, digits[i], indexOffset);
		indexOffset--;		//자리수에 맞게 오른쪽에서 왼쪽으로 렌더링
	}
}

void TimerUI::SetNumber(float number)
{
	if (targetNumber != number)
	{
		targetNumber = number; //목표 숫자 갱신

		digits.clear(); //기존 숫자들 제거
		int number = static_cast<int>(targetNumber);

		do
		{
			digits.push_back(number%100); //현재 자리 숫자를 추가

			//wchar_t buffer[256];
			//swprintf_s(buffer, 256, L"elapsedTime: %d\n", number);
			//OutputDebugString(buffer);
			
			number /= 10;			  //다음 자리로 이동
		} while (number > 0);

		//const int minimumDigits = 1; //최소 자리수
		//int paddingCount = minimumDigits - (int)digits.size();
		//for (int i = 0; i < paddingCount; i++)
		//{
		//	digits.push_back(0); //자리수가 부족하면 0으로 채움
		//}

		SetWidth(Size * (static_cast<int>(digits.size()) + 1)); //자리수에 맞게 너비 조정
	}
}

void TimerUI::RenderDigit(Gdiplus::Graphics* graphics, int digit, unsigned int index)
{
	if (digit < 0 || digit>9)
	{
		return; //유효하지 않은 숫자
	}

	//피봇은 왼쪽 상단으로 설정
	graphics->DrawImage(digitImages[digit], static_cast<int>(Position.X) + index * Size, static_cast<int>(Position.Y), Size, Size);
}