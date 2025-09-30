#pragma once
#include <windows.h>
#include <gdiplus.h>

extern HWND g_hMainWindow;
extern Gdiplus::Point g_ScreenSize;
extern float DeltaTime;

class Background
{
public:
	Background() = delete;
	Background(const wchar_t* InPath);
	~Background();

	inline float GetDrawPositionX() const { return DrawPositionX; }
	inline void SetDrawPositionX(float InX) { DrawPositionX = InX; }

	void Render(Gdiplus::Graphics* InGraphics);
private:
	Gdiplus::Bitmap* Image = nullptr;
	int ImageWidth = 0;
	int ImageHeight = 0;

	float DrawPositionX = 0.0f;
};

