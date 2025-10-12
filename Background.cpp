#include "Background.h"
#include "GameManager.h"

Background::Background(ResourceID InID)
	:Actor(InID) // 부모 클래스 생성자 호출
{
	//회전 중심은 왼쪽 위
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
		//맵을 유돟적으로 여러개 이어 붙이는 방법 생각해보기
		//현재는 고정적인 크기에서 2개 붙이기
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

	//constexpr int PixelGap = 1;	//각 타일은 3픽셀씩 겹침
	//if (Image)
	//{
	//	//Image가 로딩되어 있다.
	//	int NewY = static_cast<int>(Position.Y - Size * Pivot.Y + Offset);
	//	if (Offset > GameManager::ScreenHeight)
	//	{
	//		Offset = -static_cast<float>(Size);
	//	}

	//	int WidthCount = GameManager::ScreenWidth / Size + 1; // +1은 화면 밖까지 타일을 그리기 위해(스크롤 시 빈공간 채우는걸 보장)
	//	int HeightCount = GameManager::ScreenHeight / Size + 2;
	//	int TotalHeight = (Size - PixelGap) * HeightCount; //PixelGap만큼 겹치게 그리기 위해

	//	for (int y = -1; y < HeightCount; y++) //화면 위쪽 경계 밖부터 타일 배치
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

	//		//타일의 y위치가 전체 높이를 넘어가면 다시 위로 올림 
	//		if (NewY > TotalHeight)
	//		{
	//			NewY -= (TotalHeight + (Size - PixelGap));
	//		}
	//	}
	//}
}
