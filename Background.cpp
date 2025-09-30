#include "Background.h"
#include <string>

Background::Background(const wchar_t* InPath)
{
	Image = new Gdiplus::Bitmap(InPath);
	if (!Image)
	{
		delete Image;
		Image = nullptr;
		OutputDebugString(L"백그라운드 이미지 생성 실패\n");
		MessageBox(g_hMainWindow, L"백그라운드 이미지 생성 실패", L"오류", MB_OK | MB_ICONERROR);
	}
	int ImageWidth = Image->GetWidth();		//1024
	int ImageHeight = Image->GetHeight();	//1024
}

Background::~Background()
{
	if (Image)
	{
		delete Image;
		Image = nullptr;
	}
}

void Background::Render(Gdiplus::Graphics* InGraphics)
{
	/*WCHAR buffer[256];
	swprintf_s(buffer, L"배경 델타타임 값: %.5f\n", DeltaTime);
	OutputDebugStringW(buffer);*/

	DrawPositionX -= 100.0f /** DeltaTime*/;
	if (DrawPositionX < 0)
	{
		DrawPositionX = g_ScreenSize.X;
	}
    if (Image)
    {
        InGraphics->DrawImage(
            Image,  //그려질 이미지
            static_cast<int>(DrawPositionX),          //그려질 위치
            static_cast<int>(0),       
			g_ScreenSize.X, g_ScreenSize.Y);  //그려질 사이즈
		InGraphics->DrawImage(
			Image,
			static_cast<int>(DrawPositionX-g_ScreenSize.X),
			static_cast<int>(0),
			g_ScreenSize.X, g_ScreenSize.Y);
    }

}

//g_ScreenSize.X, g_ScreenSize.Y