#include "TimerUI.h"
#include "ResourceManager.h"
#include "GameManager.h"

void TimerUI::OnInitialize()
{
	Actor::OnInitialize(); //�θ��� OnInitialize ȣ��
	
	SetSize(GameManager::ActorDefaultSize / 2); //ũ�� ����
	anchoredPosition = { -10,10 };			//��Ŀ�� ���������� ��ġ ����
	anchor = { 1.0f,0.0f };					//��Ŀ ����Ʈ�� ������ ������� ����
	Pivot = { 1.0f,0.0f };					//�ǹ� ����Ʈ�� ������ ������� ����
	width = Size * 4;
	height = Size;
	UpdatePosition();

	digits.reserve(3);		 //�ּ� 3�� �ڸ� ���ڸ� ������ �� �ֵ��� ����
	digitImages.reserve(10); //0~9 10�� ���� �̹����� ������ �� �ֵ��� ����

	//0~9������ ���� �̹����� �ε�
	for (int i = 0; i < 10; i++)
	{
		digitImages.push_back(ResourceManager::Get().GetImage(static_cast<ResourceID>(static_cast<int>(ResourceID::Number0) + i)));
	}
	dotImage = ResourceManager::Get().GetImage(ResourceID::Dot);
}

void TimerUI::OnTick(float deltaTime)
{
	Actor::OnTick(deltaTime); //�θ��� OnTick ȣ��
	elapsedTime += deltaTime; //��� �ð� ����
	SetNumber(elapsedTime);   //���� ����
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
		//	//�Ҽ��� �׸���
		//	graphics->DrawImage(dotImage, static_cast<int>(Position.X) + indexOffset * Size, static_cast<int>(Position.Y), Size, Size);
		//	indexOffset--;
		//}
		RenderDigit(graphics, digits[i], indexOffset);
		indexOffset--;		//�ڸ����� �°� �����ʿ��� �������� ������
	}
}

void TimerUI::SetNumber(float number)
{
	if (targetNumber != number)
	{
		targetNumber = number; //��ǥ ���� ����

		digits.clear(); //���� ���ڵ� ����
		int number = static_cast<int>(targetNumber);

		do
		{
			digits.push_back(number%100); //���� �ڸ� ���ڸ� �߰�

			//wchar_t buffer[256];
			//swprintf_s(buffer, 256, L"elapsedTime: %d\n", number);
			//OutputDebugString(buffer);
			
			number /= 10;			  //���� �ڸ��� �̵�
		} while (number > 0);

		//const int minimumDigits = 1; //�ּ� �ڸ���
		//int paddingCount = minimumDigits - (int)digits.size();
		//for (int i = 0; i < paddingCount; i++)
		//{
		//	digits.push_back(0); //�ڸ����� �����ϸ� 0���� ä��
		//}

		SetWidth(Size * (static_cast<int>(digits.size()) + 1)); //�ڸ����� �°� �ʺ� ����
	}
}

void TimerUI::RenderDigit(Gdiplus::Graphics* graphics, int digit, unsigned int index)
{
	if (digit < 0 || digit>9)
	{
		return; //��ȿ���� ���� ����
	}

	//�Ǻ��� ���� ������� ����
	graphics->DrawImage(digitImages[digit], static_cast<int>(Position.X) + index * Size, static_cast<int>(Position.Y), Size, Size);
}