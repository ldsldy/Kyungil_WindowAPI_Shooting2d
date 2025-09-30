#pragma once
#include <unordered_map>
#include <windows.h>
#include <gdiplus.h>
#include "enums.h"

//extern : �ڿ� �ִ� ������ �ٸ� ���Ͽ� ������ �Ǿ� �ִ�.
extern HWND g_hMainWindow;
extern Gdiplus::Point g_ScreenSize;

using PointF = Gdiplus::PointF;
class Player
{
public:
	Player() = delete;
	Player(const wchar_t* InImagePath);
	~Player();

	void Render(Gdiplus::Graphics* InGraphics);

	void HandleKeyState(WPARAM InKey, bool InIsPressed);

private:
	//�÷��̾� �̵� �ӵ�
	float Speed = 10.0f; 

	//�÷��̾� Ű �Է� ����
	std::unordered_map<InputDirection, bool> KeyWasPressedMap;
	
	//�÷��̾� �̹����� ������� ��Ʈ��
	Gdiplus::Bitmap* Image = nullptr;  //�÷��̾�� ����
	
	//�÷��̾� ũ��
	static constexpr int PixelWidth = 48;
	static constexpr int PixelHeight = (int)(PixelWidth * 1.5f);

	//�÷��̾��� �߽���
	PointF Pivot = {0.5f, 0.5f}; //Pivot �⺻ ���� �Ѱ��

	//�÷��̾� ��ġ
	PointF Position = { 0.0f, 0.0f };
};

