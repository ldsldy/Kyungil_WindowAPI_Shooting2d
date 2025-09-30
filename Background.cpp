#include "Background.h"
#include <string>

Background::Background(const wchar_t* InPath)
{
	Image = new Gdiplus::Bitmap(InPath);
	if (!Image)
	{
		delete Image;
		Image = nullptr;
		OutputDebugString(L"��׶��� �̹��� ���� ����\n");
		MessageBox(g_hMainWindow, L"��׶��� �̹��� ���� ����", L"����", MB_OK | MB_ICONERROR);
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
	swprintf_s(buffer, L"��� ��ŸŸ�� ��: %.5f\n", DeltaTime);
	OutputDebugStringW(buffer);*/

	DrawPositionX -= 100.0f /** DeltaTime*/;
	if (DrawPositionX < 0)
	{
		DrawPositionX = g_ScreenSize.X;
	}
    if (Image)
    {
        InGraphics->DrawImage(
            Image,  //�׷��� �̹���
            static_cast<int>(DrawPositionX),          //�׷��� ��ġ
            static_cast<int>(0),       
			g_ScreenSize.X, g_ScreenSize.Y);  //�׷��� ������
		InGraphics->DrawImage(
			Image,
			static_cast<int>(DrawPositionX-g_ScreenSize.X),
			static_cast<int>(0),
			g_ScreenSize.X, g_ScreenSize.Y);
    }

}

//g_ScreenSize.X, g_ScreenSize.Y