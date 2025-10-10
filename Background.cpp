#include "Background.h"
#include "GameManager.h"

Background::Background(ResourceID InID)
	:Actor(InID) // �θ� Ŭ���� ������ ȣ��
{
	//ȸ�� �߽��� ���� ��
	Pivot.X = 0;
	Pivot.Y = 0;
	MapWidth= Image->GetWidth();
	MapHeight = Image->GetHeight();
}

void Background::OnTick(float InDeltaTime)
{
	//Offset += ScrollSpeed * InDeltaTime;
}

void Background::OnRender(Gdiplus::Graphics* InGraphics)
{
	if (Image)
	{
		//���� ���������� ������ �̾� ���̴� ��� �����غ���
		//����� �������� ũ�⿡�� 2�� ���̱�
		int PositionX1 = static_cast<int>(Position.X - MapWidth * Pivot.X);
		int PositionY1 = static_cast<int>(Position.Y - MapHeight * Pivot.Y);
		InGraphics->DrawImage(
			Image,
			PositionX1,
			PositionY1,
			MapHeight, MapWidth);
		
		InGraphics->DrawImage(
			Image,
			PositionX1 + MapWidth,
			PositionY1,
			MapHeight, MapWidth);
	}

	//constexpr int PixelGap = 1;	//�� Ÿ���� 3�ȼ��� ��ħ
	//if (Image)
	//{
	//	//Image�� �ε��Ǿ� �ִ�.
	//	int NewY = static_cast<int>(Position.Y - Size * Pivot.Y + Offset);
	//	if (Offset > GameManager::ScreenHeight)
	//	{
	//		Offset = -static_cast<float>(Size);
	//	}

	//	int WidthCount = GameManager::ScreenWidth / Size + 1; // +1�� ȭ�� �۱��� Ÿ���� �׸��� ����(��ũ�� �� ����� ä��°� ����)
	//	int HeightCount = GameManager::ScreenHeight / Size + 2;
	//	int TotalHeight = (Size - PixelGap) * HeightCount; //PixelGap��ŭ ��ġ�� �׸��� ����

	//	for (int y = -1; y < HeightCount; y++) //ȭ�� ���� ��� �ۺ��� Ÿ�� ��ġ
	//	{
	//		for (int x = 0; x < WidthCount; x++)
	//		{
	//			int NewX = static_cast<int>(Position.X - Size * Pivot.X + (Size - PixelGap) * x);
	//			InGraphics->DrawImage(
	//				Image,
	//				NewX,
	//				NewY,
	//				Size, Size);
	//		}
	//		NewY += (Size - PixelGap);

	//		//Ÿ���� y��ġ�� ��ü ���̸� �Ѿ�� �ٽ� ���� �ø� 
	//		if (NewY > TotalHeight)
	//		{
	//			NewY -= (TotalHeight + (Size - PixelGap));
	//		}
	//	}
	//}
}
